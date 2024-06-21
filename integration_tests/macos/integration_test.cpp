#include <gtest/gtest.h>
#include <thread>

#include "core_audio_host.hpp"
#include "kurt.hpp"

using namespace kurt;

TEST(MacIntegration, AudioPlaysAndLoops) {
  auto kurt = std::make_unique<Kurt>();
  Kurt *kurt_ptr = kurt.get();
  CoreAudioHost host{std::move(kurt)};

  auto read_result = kurt_ptr->load_wav_file("resources/440Hz2s.wav");
  ASSERT_TRUE(read_result.has_value()) << read_result.error();

  host.start();

  kurt_ptr->play();

  ASSERT_TRUE(kurt_ptr->status().playing);

  std::this_thread::sleep_for(std::chrono::milliseconds(2100));

  // TODO: Check position of audio file has wrapped around

  kurt_ptr->stop();
  ASSERT_FALSE(kurt_ptr->status().playing);

  // TODO: Try to run this at teardown
  host.stop();
}
