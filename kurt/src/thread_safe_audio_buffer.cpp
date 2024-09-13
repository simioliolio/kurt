#include "thread_safe_audio_buffer.hpp"

namespace kurt {

ThreadSafeAudioBuffer::ThreadSafeAudioBuffer(
    std::unique_ptr<PCMAudioData> pcm_data,
    std::unique_ptr<std::binary_semaphore> pcm_data_sem) noexcept
    : _pcm_data(std::move(pcm_data)), _pcm_data_sem(std::move(pcm_data_sem)) {};

ThreadSafeAudioBuffer::ThreadSafeAudioBuffer() noexcept {
  _pcm_data = nullptr;
  _pcm_data_sem = std::make_unique<std::binary_semaphore>(1);
}

void ThreadSafeAudioBuffer::acquire() noexcept { _pcm_data_sem->acquire(); }

void ThreadSafeAudioBuffer::release() noexcept { _pcm_data_sem->release(); }

bool ThreadSafeAudioBuffer::has_audio_data() const noexcept {
  return _pcm_data != nullptr;
}

PCMAudioData &ThreadSafeAudioBuffer::get_audio_data() const {
  if (!has_audio_data()) {
    throw std::runtime_error("get_audio_data when no audio data set");
  }
  return *_pcm_data.get();
}

void ThreadSafeAudioBuffer::set_audio_data(
    std::unique_ptr<PCMAudioData> pcm_data) noexcept {
  _pcm_data = std::move(pcm_data);
}

} // namespace kurt
