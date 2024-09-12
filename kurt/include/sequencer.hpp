#include <unordered_map>
#include <vector>

namespace kurt {

/**
 * @brief A sequencer that stores an array of values for each key.
 */
template <typename K, typename V> class Sequencer {

public:
  Sequencer() = default;

  Sequencer(Sequencer const &) = delete;
  Sequencer(Sequencer &&) = delete;
  Sequencer &operator=(Sequencer const &) = delete;
  Sequencer &operator=(Sequencer &&) = delete;

  ~Sequencer() = default;

  void set(K key, std::vector<V> values) noexcept { _sequence[key] = values; }

  std::vector<V> get(K key) const noexcept {
    if (!contains(key)) {
      return {};
    }
    return _sequence.at(key);
  }

private:
  std::unordered_map<K, std::vector<V>> _sequence;
  bool contains(K key) const noexcept {
    return _sequence.find(key) != _sequence.end();
  }
};

} // namespace kurt
