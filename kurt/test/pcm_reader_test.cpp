#include "pcm_reader.hpp"

#include <gtest/gtest.h>

using namespace kurt;

TEST(PCMReaderTest, Load)
{
    PCMReader reader;
    reader.read("test.wav");
    // TODO: Finish test
}
