#include "grain_store.hpp"

#include <ranges>
#include <stdexcept>

namespace kurt {

GrainStore::GrainStore(std::unique_ptr<std::vector<Grain>> grains) noexcept
    : _grains(std::move(grains)) {}

GrainStore::GrainStore() : _grains(std::make_unique<std::vector<Grain>>()) {}

Grain &GrainStore::available_grain() {
  for (auto &grain : *_grains) {
    if (grain.get_state() == Grain::State::INACTIVE) {
      return grain;
    }
  }
  // TODO: Construct new grain if no available grains
  throw std::runtime_error("No available grains");
}

} // namespace kurt