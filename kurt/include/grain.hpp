#pragma once

#include <memory>
#include <span>

#include "audio_buffer.hpp"

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
 * @note This class is not thread-safe. Try to set start frame, duration etc
 * then call next_frame. Changing parameters while calling next_frame may cause
 * unexpected behaviour.
 */
class Grain {
public:
  /**
   * @brief Represents the state of a grain.
   *
   * If inactive, the grain free to be reused
   */
  enum State { INACTIVE, ACTIVE };

  Grain() = default;
  Grain(Grain &&) = default;
  Grain &operator=(Grain &&) = delete;
  Grain(const Grain &) = default;
  Grain &operator=(const Grain &) = delete;

  /**
   * @brief Sets the PCM audio data associated with the grain.
   *
   * @param audio_buffer The PCM audio data to associate with the grain.
   *
   * @warning This is not thread-safe. Ensure that the audio buffer is not
   * being used by another thread when calling this function.
   */
  void set_audio_buffer(std::shared_ptr<AudioBuffer> audio_buffer) noexcept;

  /**
   * @brief Sets the starting frame of the grain.
   *
   * @param frame The new starting frame of the grain.
   * Ie, one frame is a sample for each channel.
   */
  void set_start_frame(int64_t start_position) noexcept;

  /**
   * @brief Gets the current position of the grain.
   *
   * @return The current position of the grain, in frames relative
   * to all pcm data.
   */
  int64_t get_pcm_data_position() const noexcept;

  /**
   * @brief Sets the duration of the grain.
   */
  void set_duration(int64_t duration) noexcept;

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

  /**
   * @brief Set the grain's state to ACTIVE
   *
   * Only when ACTIVE will the grain return non-silence.
   *
   * The grain will set itself to INACTIVE when it has finished playing.
   */
  void make_active() noexcept;

  /**
   * @brief Get the current state
   */
  State get_state() const noexcept;

private:
  std::shared_ptr<AudioBuffer>
      _audio_buffer;         // The PCM audio data associated with the grain.
  uint16_t _channels = 0;    // The number of channels in the audio data.
  uint32_t _frames = 0;      // The number of frames in the audio data.
  uint32_t _sample_rate = 0; // The sample rate of the audio data.
  float *_audio_data_ptr;    // Pointer to the audio data.

  uint64_t _start_frame = 0;         // The start position of the grain.
  float_t _pcm_data_position = 0.0f; // The current position of the grain.
  uint64_t _duration = 1.0f;         // The duration of the grain, in frames.
  uint32_t _attack = 0;              // The attack value for the grain.
  uint32_t _decay = 0;               // The decay value for the grain.
  std::vector<float> _output_frame = {
      0.0f, 0.0f}; // A buffer to store the output frame.
  std::vector<float> _silent_frame = {
      0.0f, 0.0f};                // A buffer to store a silent frame.
  State _state = State::INACTIVE; // The state of the grain.

  float grain_amp_for_frame(int64_t frame) const noexcept;
  void verify_start_frame_and_duration() noexcept;
  uint64_t grain_relative_position(uint64_t pcm_data_position) const noexcept;
};

} // namespace kurt
