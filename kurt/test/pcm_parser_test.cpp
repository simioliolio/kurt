#include <gtest/gtest.h>

#include "pcm_parser.hpp"

TEST(PCMParserTest, Parse16bitStereo) {
  kurt::PCMParser parser;
  auto result = parser.parse("kurt/test/resources/16bit_stereo.wav");
  ASSERT_TRUE(result.has_value()) << result.error();
  auto data = std::move(result.value());
  ASSERT_EQ(data->sample_rate, 48000);
  ASSERT_EQ(data->channels, 2);
  ASSERT_EQ(data->number_of_frames, 6);
  ASSERT_EQ(data->normalized_data.size(), 12);
}

TEST(PCMParserTest, Parse24bitStereo) {
  kurt::PCMParser parser;
  auto result = parser.parse("kurt/test/resources/24bit_stereo.wav");
  ASSERT_TRUE(result.has_value()) << result.error();
  auto data = std::move(result.value());
  ASSERT_EQ(data->sample_rate, 48000);
  ASSERT_EQ(data->channels, 2);
  ASSERT_EQ(data->number_of_frames, 6);
  ASSERT_EQ(data->normalized_data.size(), 12);
}
