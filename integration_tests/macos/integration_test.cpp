#include <gtest/gtest.h>
#include <thread>

#include "core_audio_host.hpp"
#include "kurt.hpp"

using namespace kurt;

/**
 * @brief Integration tests for the CoreAudioHost class.
 *
 * These tests are designed to run on macOS. While there
 * are few assertions, the idea is to listen to the output
 * of the audio engine and check that it is working as expected.
 */
class MacIntegration : public ::testing::Test {
protected:
  std::shared_ptr<CoreAudioHost> host;
  Kurt *kurt_ptr;
  std::unique_ptr<Kurt> kurt;

  void SetUp() override {
    kurt = std::make_unique<Kurt>();
    kurt_ptr = kurt.get();
    host = std::make_shared<CoreAudioHost>(std::move(kurt));
    host->start();
  }

  void TearDown() override { host->stop(); }
};

TEST_F(MacIntegration, AudioPlaysAndLoops) {
  auto error = kurt_ptr->load_wav_file("resources/440Hz2s.wav");
  ASSERT_FALSE(error) << error.value();

  kurt_ptr->play();

  ASSERT_TRUE(kurt_ptr->status().playing);

  std::this_thread::sleep_for(std::chrono::milliseconds(2500));

  kurt_ptr->stop();
  ASSERT_FALSE(kurt_ptr->status().playing);
};
