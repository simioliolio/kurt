#include <gtest/gtest.h>

#include "grain.hpp"
#include "grain_store.hpp"

#include "utilities/stub_audio_buffer.hpp"

using namespace kurt;

class GrainStoreTest : public ::testing::Test {
protected:
  std::shared_ptr<test::StubAudioBuffer> stub_audio_buffer =
      std::make_shared<test::StubAudioBuffer>();
  std::shared_ptr<GrainStore> grain_store;

  void SetUp() override {
    std::array<Grain, GrainStore::MAX_GRAINS> grains;
    for (auto &grain : grains) {
      std::cout << "Setting audio buffer" << std::endl;
      grain.set_audio_buffer(stub_audio_buffer);
    }
    // TODO: Cannot access grains after they have been moved in
    // to GrainStore. This is a problem for testing, and defeats
    // the intention of dependency injection. Wrapping in a
    // unique pointer or similar will incur a dereference cost,
    // so should be avoided.
    grain_store = std::make_shared<GrainStore>(std::move(grains));
  }
};

TEST_F(GrainStoreTest, ReturnsNextInactiveGrain) {
  auto &grains = grain_store->grains();
  grains[0].make_active();
  Grain &grain = grain_store->available_grain();
  Grain &expected_grain = grains[1];
  ASSERT_EQ(grain.get_state(), Grain::State::INACTIVE);
  ASSERT_EQ(&grain, &expected_grain);
}

TEST_F(GrainStoreTest, ThrowsIfNoGrainsAvailable) {
  auto &grains = grain_store->grains();
  for (auto &grain : grains) {
    grain.make_active();
  }
  ASSERT_THROW(grain_store->available_grain(), std::runtime_error);
}

TEST_F(GrainStoreTest, ReturnsAllActiveGrains) {
  auto &grains = grain_store->grains();
  grains[0].make_active();
  grains[1].make_active();
  auto all_active_grains = grain_store->active_grains();
  ASSERT_EQ(std::ranges::distance(all_active_grains), 2);
  auto first_active_grain = std::ranges::begin(all_active_grains);
  ASSERT_EQ(&(*first_active_grain), &grains[0]);
  auto second_active_grain = std::ranges::next(first_active_grain);
  ASSERT_EQ(&(*second_active_grain), &grains[1]);
}
