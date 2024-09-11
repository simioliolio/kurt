#include <cstdint>
#include <optional>

namespace kurt {

/**
 * @brief The Conductor class acts as a metronome for the system,
 * converting between frames (samples for a given audio clock tick)
 * and subdivisions (a unit of time based on the BPM)
 */
class Conductor {

public:
  Conductor(uint32_t sample_rate) noexcept;

  Conductor() = delete;
  Conductor(const Conductor &) = delete;
  Conductor(Conductor &&) = delete;
  Conductor &operator=(const Conductor &) = delete;
  Conductor &operator=(Conductor &&) = delete;

  ~Conductor() = default;

  /**
   * @brief Advance the conductor to the next frame
   */
  void next_frame() noexcept;

  /**
   * @brief Set the BPM of the conductor
   */
  void set_bpm(uint16_t bpm) noexcept;

  /**
   * @brief Set the number of subdivisions per beat
   */
  void set_subdivisions_per_beat(uint16_t subdivisions_per_beat) noexcept;

  /**
   * @brief Set the number of beats in a loop. Conductor will wrap
   * around to the beginning of the loop after this many beats
   */
  void set_number_of_beats(uint16_t number_of_beats) noexcept;

  /**
   * @brief Get the new subdivision if the conductor has advanced to a new one,
   * otherwise return nullopt
   */
  std::optional<uint16_t> new_subdivision() const noexcept;

  /**
   * @brief Get the current playhead position in subdivisions
   */
  uint16_t get_playhead() const noexcept { return _playhead; };

private:
  uint32_t _sample_rate;
  uint16_t _bpm = 120;
  uint16_t _subdivisions_per_beat = 4;
  uint32_t _number_of_beats = 4;
  uint32_t _frame_count = 0;
  uint16_t _playhead = 0; // in subdivisions
  uint32_t _frames_per_subdivision = 5500;
  std::optional<uint16_t> _new_subdivision = 0;
  void calculate_frames_per_subdivision() noexcept;
};

} // namespace kurt
