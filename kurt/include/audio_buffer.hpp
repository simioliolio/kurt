#pragma once

#include "pcm_audio_data.hpp"

namespace kurt {

class AudioBuffer {
public:
  virtual ~AudioBuffer() = default;

  /**
   * @brief Check if audio data has been set.
   *
   * @return True if audio data has been set, false otherwise.
   */
  virtual bool has_audio_data() const noexcept = 0;

  /**
   * @brief Get the PCM audio data associated with the audio buffer.
   *
   * @note Throws an exception if audio has not been set. Check
   * if audio has been set with `has_audio_data`.
   */
  // TODO: Remove slow API
  virtual PCMAudioData &get_audio_data() const = 0;

  virtual float *get_audio_data_ptr() noexcept = 0;

  virtual uint32_t get_sample_rate() const noexcept = 0;

  virtual uint16_t get_num_channels() const noexcept = 0;

  virtual uint32_t get_number_of_frames() const noexcept = 0;
};

} // namespace kurt
