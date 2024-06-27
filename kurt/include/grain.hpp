#pragma once

#include <memory>
#include <span>

#include "pcm_audio_data.hpp"

namespace kurt {

/**
 * @brief Represents a grain of audio data.
 *
 * A grain is a small piece of audio data that can be played back at different
 * speeds and positions. Grains are used in granular synthesis to create complex
 * sounds from simple audio data. This class provides a way to manipulate and
 * play back grains of audio data. Grains are typically used in granular
 * synthesis to create complex sounds from simple audio data.
 *
 * @note This class is not thread-safe.
 */
class Grain {
public:
  /**
   * @brief Constructs a Grain object with the specified PCM audio data.
   *
   * @param pcm_data The PCM audio data to associate with the grain.
   * Ie, one frame is a sample for each channel.
   */
  Grain(std::shared_ptr<PCMAudioData> pcm_data);

  /**
   * @brief Sets the position of the grain.
   *
   * @param position The new position of the grain, in frames.
   * Ie, one frame is a sample for each channel.
   */
  void set_position(int64_t frame) noexcept;

  /**
   * @brief Gets the current position of the grain.
   *
   * @return The current position of the grain, in frames.
   */
  int64_t get_position() const noexcept;

  /**
   * @brief Retrieves the next frame of audio data from the grain.
   *
   * @return A span containing the next frame of audio data.
   */
  const std::span<const float> next_frame() noexcept;

  /**
   * @brief Sets the attack value for the grain.
   *
   * This function sets the attack value for the grain. The attack value
   * determines how quickly the grain reaches its maximum amplitude.
   *
   * @param attack The attack value to set, in frames.
   */
  void set_attack(int32_t attack) noexcept;

  /**
   * @brief Sets the decay value for the grain.
   *
   * This function sets the decay value for the grain. The decay value
   * determines how quickly the grain fades out after reaching its maximum
   * amplitude.
   *
   * @param decay The decay value to set, in frames.
   */
  void set_decay(int32_t decay) noexcept;

private:
  std::shared_ptr<PCMAudioData>
      _pcm_data;            ///< The PCM audio data associated with the grain.
  int64_t _position = 0.0f; ///< The current position of the grain.
  int32_t _attack = 0;      ///< The attack value for the grain.
  int32_t _decay = 0;       ///< The decay value for the grain.
  std::vector<float> _output_frame = {
      0.0f, 0.0f}; ///< A buffer to store the output frame.
  float grain_amp_for_frame(int64_t frame) const noexcept;
};

} // namespace kurt
