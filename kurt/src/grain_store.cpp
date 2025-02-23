#include "grain_store.hpp"

#include <ranges>
#include <stdexcept>

namespace kurt {

GrainStore::GrainStore(
    std::array<Grain, GrainStore::MAX_GRAINS> &&grains) noexcept
    : _grains(grains) {}

GrainStore::GrainStore(GrainStore &&other) noexcept {
  std::move(other._grains.begin(), other._grains.end(), _grains.begin());
}

GrainStore &GrainStore::operator=(GrainStore &&other) noexcept {
  if (this != &other) {
    std::move(other._grains.begin(), other._grains.end(), _grains.begin());
  }
  return *this;
}

Grain &GrainStore::available_grain() {
  for (auto &grain : _grains) {
    if (grain.get_state() == Grain::State::INACTIVE) {
      return grain;
    }
  }
  throw std::runtime_error("No available grains");
}

} // namespace kurt
