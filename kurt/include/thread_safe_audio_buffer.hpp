#pragma once

#include "audio_buffer.hpp"
#include "pcm_audio_data.hpp"

#include <memory>
#include <semaphore>

namespace kurt {
/**
 * @class ThreadSafeAudioBuffer
 * @brief A thread-safe audio buffer that extends the AudioBuffer class.
 *
 * This class provides a thread-safe implementation of an audio buffer. It
 * ensures that access to the underlying PCM audio data is synchronized between
 * multiple threads.
 *
 * @note This class is derived from the AudioBuffer class.
 */
class ThreadSafeAudioBuffer : public AudioBuffer {
public:
  ThreadSafeAudioBuffer(
      std::unique_ptr<PCMAudioData> pcm_data,
      std::unique_ptr<std::binary_semaphore> pcm_data_sem =
          std::make_unique<std::binary_semaphore>(1)) noexcept;
  ThreadSafeAudioBuffer() noexcept;

  ThreadSafeAudioBuffer(const ThreadSafeAudioBuffer &other) = delete;
  ThreadSafeAudioBuffer(ThreadSafeAudioBuffer &&other) = delete;
  ThreadSafeAudioBuffer &operator=(const ThreadSafeAudioBuffer &other) = delete;
  ThreadSafeAudioBuffer &operator=(ThreadSafeAudioBuffer &&other) = delete;

  ~ThreadSafeAudioBuffer() = default;

  /**
   * @brief Check if audio data has been set.
   *
   * @return True if audio data has been set, false otherwise.
   */
  bool has_audio_data() const noexcept override;

  /**
   * @brief Get the PCM audio data associated with the audio buffer.
   *
   * @note Throws an exception if audio has not been set. Check
   * if audio has been set with `has_audio_data`.
   *
   * @note Call `acquire` before calling get_audio_data(), then `release` after
   * you are done.
   */
  PCMAudioData &get_audio_data() const override;

  /**
   * @brief Set the PCM audio data associated with the audio buffer.
   *
   * @param pcm_data The PCM audio data to associate with the audio buffer.
   *
   * @note This function is thread-safe.
   */
  void set_audio_data(std::unique_ptr<PCMAudioData> pcm_data) noexcept;

  float *get_audio_data_ptr() noexcept override {
    return _pcm_data->normalized_data.data();
  }

  uint32_t get_sample_rate() const noexcept override {
    return _pcm_data->sample_rate;
  }

  uint16_t get_num_channels() const noexcept override {
    return _pcm_data->channels;
  }

  uint32_t get_number_of_frames() const noexcept override {
    return _pcm_data->number_of_frames;
  }

  /**
   * @brief Acquire and release the semaphore.
   * Incorrect use of these functions can lead to deadlocks.
   * TODO: Return a RAII object that acquires the semaphore in the constructor
   * and releases it in the destructor.
   */
  void acquire() noexcept;
  void release() noexcept;

private:
  std::unique_ptr<PCMAudioData> _pcm_data;
  std::unique_ptr<std::binary_semaphore> _pcm_data_sem;
};

} // namespace kurt
