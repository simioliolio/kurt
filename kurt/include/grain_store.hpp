#include <memory>
#include <ranges>
#include <vector>

#include "grain.hpp"

namespace kurt {

class GrainStore {

public:
  GrainStore(std::unique_ptr<std::vector<Grain>> grains) noexcept;
  GrainStore() = delete;
  GrainStore(GrainStore &&) = delete;
  GrainStore &operator=(GrainStore &&) = delete;
  GrainStore(const GrainStore &) = delete;
  GrainStore &operator=(const GrainStore &) = delete;

  /**
   * @brief Get the next available grain.
   *
   * @note Throws an exception if there are no available grains
   */
  // TODO: Remove
  Grain &available_grain();

  /**
   * @brief Add a grain to the store.
   */
  void add_grain(Grain grain);

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
    return *_grains | std::views::filter([](const Grain &grain) {
      return grain.get_state() == Grain::State::ACTIVE;
    });
  }

private:
  std::unique_ptr<std::vector<Grain>> _grains;
};

} // namespace kurt
