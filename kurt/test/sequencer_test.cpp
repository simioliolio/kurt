#include "sequencer.hpp"

#include <array>
#include <gtest/gtest.h>

using namespace kurt;

class SequencerTest : public ::testing::Test {
protected:
  Sequencer<uint16_t, std::string> sequencer;
  void SetUp() override {}
};

TEST_F(SequencerTest, SetAndRecall) {
  sequencer.set(0, {"zero"});
  sequencer.set(1, {"one"});
  sequencer.set(2, {"two"});
  ASSERT_EQ(sequencer.get(0), std::vector<std::string>{"zero"});
  ASSERT_EQ(sequencer.get(1), std::vector<std::string>{"one"});
  ASSERT_EQ(sequencer.get(2), std::vector<std::string>{"two"});
}

TEST_F(SequencerTest, ReturnsEmptyVectorIfKeyNotFound) {
  ASSERT_EQ(sequencer.get(0), std::vector<std::string>{});
}
