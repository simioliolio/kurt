#include "pcm_reader.hpp"

#include <gtest/gtest.h>

using namespace kurt;

TEST(PCMReaderTest, Load) {
  PCMReader reader;
  auto result = reader.read("kurt/test/resources/16bit_stereo.wav");
  ASSERT_TRUE(result.has_value()) << result.error();

  auto data_length = result.value();
  ASSERT_EQ(data_length, 24) << "Data length is not correct";
}

TEST(PCMReaderTest, SampleAtFrame) {
  PCMReader reader;
  auto result = reader.read("kurt/test/resources/16bit_stereo.wav");
  ASSERT_TRUE(result.has_value()) << result.error();

  auto sample = reader.sample_at_frame(0, 0);
  ASSERT_EQ(sample, 0) << "Sample value is not correct";
  sample = reader.sample_at_frame(0, 1);
  ASSERT_EQ(sample, 0) << "Sample value is not correct";

  sample = reader.sample_at_frame(1, 0);
  ASSERT_EQ(sample, 16384) << "Sample value is not correct";
  sample = reader.sample_at_frame(1, 1);
  ASSERT_EQ(sample, 16384) << "Sample value is not correct";

  sample = reader.sample_at_frame(2, 0);
  ASSERT_EQ(sample, 32767) << "Sample value is not correct";
  sample = reader.sample_at_frame(2, 1);
  ASSERT_EQ(sample, 32767) << "Sample value is not correct";

  sample = reader.sample_at_frame(3, 0);
  ASSERT_EQ(sample, -32768) << "Sample value is not correct";
  sample = reader.sample_at_frame(3, 1);
  ASSERT_EQ(sample, -32768) << "Sample value is not correct";

  sample = reader.sample_at_frame(4, 0);
  ASSERT_EQ(sample, -16384) << "Sample value is not correct";
  sample = reader.sample_at_frame(4, 1);
  ASSERT_EQ(sample, -16384) << "Sample value is not correct";

  sample = reader.sample_at_frame(5, 0);
  ASSERT_EQ(sample, 0) << "Sample value is not correct";
  sample = reader.sample_at_frame(5, 1);
  ASSERT_EQ(sample, 0) << "Sample value is not correct";
}

TEST(PCMReaderTest, SampleAtFrame24Bit) {
  PCMReader reader;
  auto result = reader.read("kurt/test/resources/24bit_stereo.wav");
  ASSERT_TRUE(result.has_value()) << result.error();

  auto sample = reader.sample_at_frame(0, 0);
  ASSERT_EQ(sample, 0) << "Sample value is not correct";
  sample = reader.sample_at_frame(0, 1);
  ASSERT_EQ(sample, 0) << "Sample value is not correct";

  sample = reader.sample_at_frame(1, 0);
  ASSERT_EQ(sample, 4194304) << "Sample value is not correct";
  sample = reader.sample_at_frame(1, 1);
  ASSERT_EQ(sample, 4194304) << "Sample value is not correct";

  sample = reader.sample_at_frame(2, 0);
  ASSERT_EQ(sample, 8388607) << "Sample value is not correct";
  sample = reader.sample_at_frame(2, 1);
  ASSERT_EQ(sample, 8388607) << "Sample value is not correct";

  sample = reader.sample_at_frame(3, 0);
  ASSERT_EQ(sample, -8388608) << "Sample value is not correct";
  sample = reader.sample_at_frame(3, 1);
  ASSERT_EQ(sample, -8388608) << "Sample value is not correct";

  sample = reader.sample_at_frame(4, 0);
  ASSERT_EQ(sample, -4194304) << "Sample value is not correct";
  sample = reader.sample_at_frame(4, 1);
  ASSERT_EQ(sample, -4194304) << "Sample value is not correct";

  sample = reader.sample_at_frame(5, 0);
  ASSERT_EQ(sample, 0) << "Sample value is not correct";
  sample = reader.sample_at_frame(5, 1);
  ASSERT_EQ(sample, 0) << "Sample value is not correct";
}
