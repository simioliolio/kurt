#include "thread_safe_audio_buffer.hpp"

namespace kurt {

ThreadSafeAudioBuffer::ThreadSafeAudioBuffer(
    std::unique_ptr<PCMAudioData> pcm_data,
    std::unique_ptr<std::binary_semaphore> pcm_data_sem) noexcept
    : _pcm_data(std::move(pcm_data)), _pcm_data_sem(std::move(pcm_data_sem)) {};

void ThreadSafeAudioBuffer::acquire() noexcept { _pcm_data_sem->acquire(); }

void ThreadSafeAudioBuffer::release() noexcept { _pcm_data_sem->release(); }

PCMAudioData &ThreadSafeAudioBuffer::get_audio_data() const noexcept {
  return *_pcm_data.get();
}

void ThreadSafeAudioBuffer::set_audio_data(
    std::unique_ptr<PCMAudioData> pcm_data) noexcept {
  acquire();
  _pcm_data = std::move(pcm_data);
  release();
}

} // namespace kurt
