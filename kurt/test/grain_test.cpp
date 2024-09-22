#include <gtest/gtest.h>

#include "grain.hpp"
#include "pcm_audio_data.hpp"
#include "pcm_parser.hpp"
#include "utilities/stub_audio_buffer.hpp"

using namespace kurt;

class GrainTest : public ::testing::Test {
protected:
  std::shared_ptr<Grain> grain;
  std::shared_ptr<test::StubAudioBuffer> _stub_audio_buffer =
      std::make_shared<test::StubAudioBuffer>();

  void SetUp() override { grain = std::make_shared<Grain>(_stub_audio_buffer); }

  PCMAudioData stub_pcm_data() {
    return {44100,
            2,
            6,
            {0.0f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f, -1.0f, -1.0f, -0.5f, -0.5f,
             -0.2f, -0.2f}};
  }

  PCMAudioData stub_full_scale_pcm_data() {
    return {44100,
            2,
            5,
            {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}};
  }
};

TEST_F(GrainTest, PositionIsStartByDefault) {

  grain->set_start_frame(0);

  auto position = grain->get_pcm_data_position();
  ASSERT_EQ(position, 0);
}

TEST_F(GrainTest, GetsSamplesFromPosition) {

  grain->set_start_frame(2);
  grain->make_active();

  auto position = grain->get_pcm_data_position();
  ASSERT_EQ(position, 2);

  auto frame = grain->next_frame();
  ASSERT_EQ(frame[0], 1.0f);
  ASSERT_EQ(frame[1], 1.0f);
}

TEST_F(GrainTest, IncrementsPosition) {

  grain->set_start_frame(0);
  grain->make_active();

  auto position = grain->get_pcm_data_position();
  ASSERT_EQ(position, 0);

  grain->next_frame();
  position = grain->get_pcm_data_position();
  ASSERT_EQ(position, 1);
}

TEST_F(GrainTest, ReturnsCorrectNumberOfChannels) {

  grain->set_start_frame(0);

  auto frame = grain->next_frame();
  ASSERT_EQ(frame.size(), 2);
}

TEST_F(GrainTest, AppliesAttackBasedOnEnvelope) {
  _stub_audio_buffer->set_audio_data(stub_full_scale_pcm_data());
  grain->set_attack(4);
  grain->set_duration(5);
  grain->make_active();

  auto frame = grain->next_frame();
  ASSERT_NEAR(frame[0], 0.000f, 0.001f);
  ASSERT_NEAR(frame[1], 0.000f, 0.001f);
  frame = grain->next_frame();
  ASSERT_NEAR(frame[0], 0.25f, 0.001f);
  ASSERT_NEAR(frame[1], 0.25f, 0.001f);
  frame = grain->next_frame();
  ASSERT_NEAR(frame[0], 0.5f, 0.001f);
  ASSERT_NEAR(frame[1], 0.5f, 0.001f);
  frame = grain->next_frame();
  ASSERT_NEAR(frame[0], 0.75f, 0.001f);
  ASSERT_NEAR(frame[1], 0.75f, 0.001f);
  frame = grain->next_frame();
  ASSERT_NEAR(frame[0], 1.000f, 0.001f); // Unmodified
  ASSERT_NEAR(frame[1], 1.000f, 0.001f); //
  frame = grain->next_frame();
  ASSERT_NEAR(frame[0], 0.000f, 0.001f); // Beyond the end
  ASSERT_NEAR(frame[1], 0.000f, 0.001f); //
}

TEST_F(GrainTest, AppliesDecayBasedOnEnvelope) {
  _stub_audio_buffer->set_audio_data(stub_full_scale_pcm_data());
  grain->set_start_frame(0);
  grain->set_duration(5);
  grain->set_decay(4);
  grain->make_active();

  auto frame = grain->next_frame();
  ASSERT_NEAR(frame[0], 1.000f, 0.001f); // Unmodified
  ASSERT_NEAR(frame[1], 1.000f, 0.001f); //
  frame = grain->next_frame();
  ASSERT_NEAR(frame[0], 0.75f, 0.001f);
  ASSERT_NEAR(frame[1], 0.75f, 0.001f);
  frame = grain->next_frame();
  ASSERT_NEAR(frame[0], 0.5f, 0.001f);
  ASSERT_NEAR(frame[1], 0.5f, 0.001f);
  frame = grain->next_frame();
  ASSERT_NEAR(frame[0], 0.25f, 0.001f);
  ASSERT_NEAR(frame[1], 0.25f, 0.001f);
  frame = grain->next_frame();
  ASSERT_NEAR(frame[0], 0.000f, 0.001f);
  ASSERT_NEAR(frame[1], 0.000f, 0.001f);
  frame = grain->next_frame();
  ASSERT_NEAR(frame[0], 0.000f, 0.001f); // Beyond the end
  ASSERT_NEAR(frame[1], 0.000f, 0.001f); //
}

TEST_F(GrainTest, AttackAppliedWhenNonZeroPosition) {
  grain->set_attack(3);
  grain->set_start_frame(2);
  grain->set_duration(4);
  grain->make_active();

  auto frame = grain->next_frame();
  ASSERT_NEAR(frame[0], 0.0f, 0.001f);
  ASSERT_NEAR(frame[1], 0.0f, 0.001f);
  frame = grain->next_frame();
  ASSERT_NEAR(frame[0], -0.333333f, 0.001f);
  ASSERT_NEAR(frame[1], -0.333333f, 0.001f);
  frame = grain->next_frame();
  ASSERT_NEAR(frame[0], -0.33333f, 0.001f);
  ASSERT_NEAR(frame[1], -0.33333f, 0.001f);
  frame = grain->next_frame();
  ASSERT_NEAR(frame[0], -0.2f, 0.001f);
  ASSERT_NEAR(frame[1], -0.2f, 0.001f);
}

TEST_F(GrainTest, GrainBecomesActiveWhenMadeActive) {
  ASSERT_EQ(grain->get_state(), Grain::State::INACTIVE);
  grain->make_active();
  ASSERT_EQ(grain->get_state(), Grain::State::ACTIVE);
}

TEST_F(GrainTest, GrainBecomesInactiveWhenFinished) {
  grain->set_duration(3);
  grain->make_active();
  auto frame = grain->next_frame();
  frame = grain->next_frame();
  frame = grain->next_frame();
  ASSERT_EQ(grain->get_state(), Grain::State::INACTIVE);
}
