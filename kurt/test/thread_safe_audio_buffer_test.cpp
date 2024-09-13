#include "thread_safe_audio_buffer.hpp"

#include <gtest/gtest.h>
#include <thread>

using namespace kurt;

TEST(ThreadSafeAudioBufferTest, TestConcurrency) {
  auto pcm_data = std::make_unique<PCMAudioData>();
  auto pcm_data_ptr = pcm_data.get();
  auto pcm_sem = std::make_unique<std::binary_semaphore>(1);
  auto pcm_sem_ptr = pcm_sem.get();
  auto buffer = ThreadSafeAudioBuffer(std::move(pcm_data), std::move(pcm_sem));

  const int num_threads = 10;
  std::vector<std::thread> threads;
  std::atomic<int> access_count(0);
  int operations = 100;

  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back([&buffer, &access_count, operations]() {
      for (int j = 0; j < operations; ++j) {
        // get
        buffer.acquire();
        auto &audio_data = buffer.get_audio_data();
        buffer.release();

        // set
        buffer.acquire();
        buffer.set_audio_data(std::make_unique<PCMAudioData>());
        buffer.release();

        ++access_count;
      }
    });
  }

  for (auto &thread : threads) {
    thread.join();
  }

  ASSERT_EQ(access_count, num_threads * operations);
};
