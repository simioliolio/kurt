#include "conductor.hpp"

#include <gtest/gtest.h>

using namespace kurt;

class ConductorTest : public ::testing::Test {
protected:
  uint32_t sample_rate = 48000;
  Conductor conductor{sample_rate};

  void SetUp() override {}
};

TEST_F(ConductorTest, NewSubdivisionAfterSomeFrames) {
  uint16_t bpm = 120;
  uint16_t subdivisions_per_beat = 4;
  conductor.set_bpm(bpm);
  conductor.set_subdivisions_per_beat(subdivisions_per_beat);

  auto beats_per_second = bpm / 60;
  auto frames_per_beat = sample_rate / beats_per_second;
  auto frames_per_subdivision = frames_per_beat / subdivisions_per_beat;

  ASSERT_EQ(conductor.new_subdivision(), 0);

  for (int i = 0; i < frames_per_subdivision - 1; i++) {
    conductor.next_frame();
    ASSERT_EQ(conductor.new_subdivision(), std::nullopt);
  }

  conductor.next_frame();
  ASSERT_EQ(conductor.new_subdivision(), 1);
}

TEST_F(ConductorTest, SubdivisionWrapsBasedOnNumberOfBeats) {
  uint16_t bpm = 120;
  uint16_t subdivisions_per_beat = 4;
  uint16_t number_of_beats = 4;

  conductor.set_bpm(bpm);
  conductor.set_subdivisions_per_beat(subdivisions_per_beat);
  conductor.set_number_of_beats(number_of_beats);

  auto beats_per_second = bpm / 60;
  auto frames_per_beat = sample_rate / beats_per_second;
  auto frames_per_loop = frames_per_beat * number_of_beats;

  // Advance to the last subdivision of the last beat
  for (int i = 0; i < frames_per_loop - 1; i++) {
    conductor.next_frame();
  }

  // Check that the playhead is at the last subdivision of the last beat
  ASSERT_EQ(conductor.new_subdivision(), std::nullopt);
  ASSERT_EQ(conductor.get_playhead(),
            (number_of_beats * subdivisions_per_beat) - 1);

  // Advance one more frame to wrap around to the first subdivision of the first
  // beat
  conductor.next_frame();
  ASSERT_EQ(conductor.new_subdivision(), 0);
  ASSERT_EQ(conductor.get_playhead(), 0);
}
