#include <gtest/gtest.h>

#include "kurt.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <ranges>
#include <thread>
#include <vector>

/**
 * @brief A benchmark to test the performance of the Kurt audio engine.
 *
 * A rough benchmark test which becomes useful during development to ensure
 * that the audio engine is not slowing down due to a change in the code.
 */
TEST(KurtBenchmark, KurtNextFrameBenchmark) {
  kurt::Kurt kurt;
  kurt.load_wav_file("resources/440Hz2s.wav");

  kurt.play();

  int num_of_tests = 100;
  std::vector<double> benchmarks{};

  for (int i = 0; i < num_of_tests; i++) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 48000; i++) {
      auto frame = kurt.next_frame();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    benchmarks.push_back(elapsed_seconds.count());
  }

  double average =
      std::accumulate(benchmarks.begin(), benchmarks.end(), 0.0) / num_of_tests;
  std::cout << "Average time to process 48000 frames: " << average << " seconds"
            << std::endl;
}
