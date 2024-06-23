#include <gtest/gtest.h>
#include <thread>

#include "core_audio_host.hpp"
#include "kurt.hpp"

using namespace kurt;

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
  auto read_result = kurt_ptr->load_wav_file("resources/440Hz2s.wav");
  ASSERT_TRUE(read_result.has_value()) << read_result.error();

  kurt_ptr->play();

  ASSERT_TRUE(kurt_ptr->status().playing);

  std::this_thread::sleep_for(std::chrono::milliseconds(2050));

  ASSERT_LT(kurt_ptr->current_frame(), 5000);

  kurt_ptr->stop();
  ASSERT_FALSE(kurt_ptr->status().playing);
};
