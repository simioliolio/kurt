#include "grain_store.hpp"

#include <ranges>
#include <stdexcept>

namespace kurt {

GrainStore::GrainStore(
    std::array<Grain, GrainStore::MAX_GRAINS> &&grains) noexcept
    : _grains(grains) {}

Grain &GrainStore::available_grain() {
  for (auto &grain : _grains) {
    if (grain.get_state() == Grain::State::INACTIVE) {
      return grain;
    }
  }
  throw std::runtime_error("No available grains");
}

} // namespace kurt
