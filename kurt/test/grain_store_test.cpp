#include <gtest/gtest.h>

#include "grain.hpp"
#include "grain_store.hpp"

using namespace kurt;

class GrainStoreTest : public ::testing::Test {
protected:
  std::vector<Grain> grains = {};
  std::shared_ptr<GrainStore> grain_store;

  void SetUp() override {
    grains.resize(5, Grain(std::make_shared<PCMAudioData>(stub_pcm_data())));
    grain_store = std::make_shared<GrainStore>(grains);
  }

  PCMAudioData stub_pcm_data() {
    return {44100,
            2,
            6,
            {0.0f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f, -1.0f, -1.0f, -0.5f, -0.5f,
             -0.2f, -0.2f}};
  }
};

TEST_F(GrainStoreTest, ReturnsNextInactiveGrain) {
  grains[0].make_active();
  Grain &grain = grain_store->available_grain();
  Grain &expected_grain = grains[1];
  ASSERT_EQ(grain.get_state(), Grain::State::INACTIVE);
  ASSERT_EQ(&grain, &expected_grain);
}

TEST_F(GrainStoreTest, ThrowsIfNoGrainsAvailable) {
  for (auto &grain : grains) {
    grain.make_active();
  }
  ASSERT_THROW(grain_store->available_grain(), std::runtime_error);
}

TEST_F(GrainStoreTest, ReturnsAllActiveGrains) {
  grains[0].make_active();
  grains[1].make_active();
  auto all_active_grains = grain_store->active_grains();
  ASSERT_EQ(std::ranges::distance(all_active_grains), 2);
  auto first_active_grain = std::ranges::begin(all_active_grains);
  ASSERT_EQ(&(*first_active_grain), &grains[0]);
  auto second_active_grain = std::ranges::next(first_active_grain);
  ASSERT_EQ(&(*second_active_grain), &grains[1]);
}
