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
class ThreadSafeAudioBuffer {
public:
  ThreadSafeAudioBuffer(
      std::unique_ptr<PCMAudioData> pcm_data,
      std::unique_ptr<std::binary_semaphore> pcm_data_sem =
          std::make_unique<std::binary_semaphore>(1)) noexcept;

  ThreadSafeAudioBuffer(const ThreadSafeAudioBuffer &other) = delete;
  ThreadSafeAudioBuffer(ThreadSafeAudioBuffer &&other) = delete;
  ThreadSafeAudioBuffer &operator=(const ThreadSafeAudioBuffer &other) = delete;
  ThreadSafeAudioBuffer &operator=(ThreadSafeAudioBuffer &&other) = delete;

  ~ThreadSafeAudioBuffer() = default;

  /**
   * @note Call `acquire` before calling get_audio_data(), then `release` after
   * you are done.
   */
  PCMAudioData &get_audio_data() const noexcept;

  void set_audio_data(std::unique_ptr<PCMAudioData> pcm_data) noexcept;

  /**
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
