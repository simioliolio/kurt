#include "pcm_reader.hpp"

#include <gtest/gtest.h>

using namespace kurt;

TEST(PCMReaderTest, Load)
{
    PCMReader reader;
    auto result = reader.read("kurt/test/resources/16bit_stereo.wav");
    ASSERT_TRUE(result.has_value()) << result.error();

    auto data_length = result.value();
    ASSERT_EQ(data_length, 16) << "Data length is not correct";

}
