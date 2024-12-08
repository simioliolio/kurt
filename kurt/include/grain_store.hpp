#include <array>
#include <memory>
#include <ranges>

#include "grain.hpp"

namespace kurt {

class GrainStore {

public:
  static constexpr size_t MAX_GRAINS = 5;
  GrainStore(std::array<Grain, MAX_GRAINS> &&grains) noexcept;
  GrainStore() = delete;
  GrainStore(GrainStore &&) = delete;
  GrainStore &operator=(GrainStore &&) = delete;
  GrainStore(const GrainStore &) = delete;
  GrainStore &operator=(const GrainStore &) = delete;

  /**
   * @brief Get the next available grain.
   *
   * @note Throws an exception if there are no available grains.
   * TODO: Handle this case more gracefully.
   */
  Grain &available_grain();

  /**
   * @brief Get all active grains.
   *
   * @return A ranges filter view of all active grains
   *
   * @note This is nice code and all, but it's potentially not very
   * efficient. This should be benchmarked against a more traditional
   * approach.
   */
  auto active_grains() {
    return _grains | std::views::filter([](const Grain &grain) {
             return grain.get_state() == Grain::State::ACTIVE;
           });
  }

  /**
   * @brief Get all grains.
   *
   * @return A reference to the array of grains
   *
   * @note TODO: It's not very safe to expose the array like this
   * but it's useful for testing. Improvement welcome here.
   */
  std::array<Grain, MAX_GRAINS> &grains() noexcept { return _grains; }

private:
  std::array<Grain, MAX_GRAINS> _grains;
};

} // namespace kurt
