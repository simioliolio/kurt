#include <gtest/gtest.h>
#include <thread>

#include "core_audio_host.hpp"
#include "kurt.hpp"

TEST(CoreAudioHostTest, AudioLoops) {
  Kurt kurt;
  CoreAudioHost host{kurt};
  host.start();

  kurt.load_wav_file("kurt_macos/test/resources/440Hz3s.wav");
  kurt.play();

  ASSERT_TRUE(kurt.playing());

  std::this_thread::sleep_for(std::chrono::seconds(2.1));

  kurt.stop();
  ASSERT_FALSE(kurt.playing());
}
