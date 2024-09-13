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
  virtual PCMAudioData &get_audio_data() const = 0;
};

} // namespace kurt
