#include <gtest/gtest.h>

#include "kurt.hpp"

using namespace kurt;

TEST(KurtTest, SampleAtFrame) {
  Kurt kurt;
  auto result = kurt.load_wav_file("kurt/test/resources/16bit_stereo.wav");
  ASSERT_FALSE(result) << result.value();

  auto sample = kurt.next_frame();
  ASSERT_NEAR(sample[0], 0, 0.0001f) << "Sample value is not correct";
  ASSERT_NEAR(sample[1], 0, 0.0001f) << "Sample value is not correct";

  sample = kurt.next_frame();
  ASSERT_NEAR(sample[0], 0.5, 0.0001f) << "Sample value is not correct";
  ASSERT_NEAR(sample[1], 0.5, 0.0001f) << "Sample value is not correct";

  sample = kurt.next_frame();
  ASSERT_NEAR(sample[0], 1.0, 0.0001f) << "Sample value is not correct";
  ASSERT_NEAR(sample[1], 1.0, 0.0001f) << "Sample value is not correct";

  sample = kurt.next_frame();
  ASSERT_NEAR(sample[0], -1.0, 0.0001f) << "Sample value is not correct";
  ASSERT_NEAR(sample[1], -1.0, 0.0001f) << "Sample value is not correct";

  sample = kurt.next_frame();
  ASSERT_NEAR(sample[0], -0.5, 0.0001f) << "Sample value is not correct";
  ASSERT_NEAR(sample[1], -0.5, 0.0001f) << "Sample value is not correct";

  sample = kurt.next_frame();
  ASSERT_NEAR(sample[0], 0.0, 0.0001f) << "Sample value is not correct";
  ASSERT_NEAR(sample[1], 0.0, 0.0001f) << "Sample value is not correct";
}

TEST(KurtTest, SampleAtFrame24Bit) {
  Kurt kurt;
  auto result = kurt.load_wav_file("kurt/test/resources/24bit_stereo.wav");
  ASSERT_FALSE(result) << result.value();

  auto sample = kurt.next_frame();
  ASSERT_NEAR(sample[0], 0, 0.0001f) << "Sample value is not correct";
  ASSERT_NEAR(sample[1], 0, 0.0001f) << "Sample value is not correct";

  sample = kurt.next_frame();
  ASSERT_NEAR(sample[0], 0.5, 0.0001f) << "Sample value is not correct";
  ASSERT_NEAR(sample[1], 0.5, 0.0001f) << "Sample value is not correct";

  sample = kurt.next_frame();
  ASSERT_NEAR(sample[0], 1.0, 0.0001f) << "Sample value is not correct";
  ASSERT_NEAR(sample[1], 1.0, 0.0001f) << "Sample value is not correct";

  sample = kurt.next_frame();
  ASSERT_NEAR(sample[0], -1.0, 0.0001f) << "Sample value is not correct";
  ASSERT_NEAR(sample[1], -1.0, 0.0001f) << "Sample value is not correct";

  sample = kurt.next_frame();
  ASSERT_NEAR(sample[0], -0.5, 0.0001f) << "Sample value is not correct";
  ASSERT_NEAR(sample[1], -0.5, 0.0001f) << "Sample value is not correct";

  sample = kurt.next_frame();
  ASSERT_NEAR(sample[0], 0.0, 0.0001f) << "Sample value is not correct";
  ASSERT_NEAR(sample[1], 0.0, 0.0001f) << "Sample value is not correct";
}
