#include "grain.hpp"

#include <algorithm>
#include <iostream>

namespace kurt {

Grain::Grain(std::shared_ptr<AudioBuffer> audio_buffer)
    : _audio_buffer(audio_buffer) {
  if (_audio_buffer->has_audio_data()) {
    auto pcm_data = _audio_buffer->get_audio_data();
    _output_frame.resize(pcm_data.channels);
  }
}

void Grain::set_start_frame(int64_t start_frame) noexcept {
  _start_frame = start_frame;
  // TODO: Changing start frame will cause a click if grain is active
  _pcm_data_position = _start_frame;
  verify_start_frame_and_duration();
}

int64_t Grain::get_pcm_data_position() const noexcept {
  return _pcm_data_position;
}

void Grain::set_duration(int64_t duration) noexcept {
  _duration = duration;
  verify_start_frame_and_duration();
}

void Grain::verify_start_frame_and_duration() {
  if (!_audio_buffer->has_audio_data()) {
    return;
  }
  auto pcm_data = _audio_buffer->get_audio_data();
  if (_start_frame + _duration > pcm_data.number_of_frames) {
    std::cout << "Warning: Grain duration exceeds number of frames"
              << std::endl;
    std::cout << "Shortening grain to the end of pcm_data..." << std::endl;
    _duration = pcm_data.number_of_frames - _start_frame;
  }
}

const std::span<const float> Grain::next_frame() noexcept {
  if (!_audio_buffer->has_audio_data()) {
    std::cout << "grain: Warning: No pcm data" << std::endl;
    return std::span<const float>(_silent_frame.data(), _silent_frame.size());
  }

  auto channels = _audio_buffer->get_num_channels();
  auto frames = _audio_buffer->get_number_of_frames();
  auto data_ptr = _audio_buffer->get_audio_data_ptr();

  if (_pcm_data_position >= frames) {
    std::cout << "Warning: position beyond end of pcm_data, outputting silence"
              << std::endl;
    return std::span<const float>(_silent_frame.data(), _silent_frame.size());
  }
  if (_state == Grain::State::INACTIVE) {
    return std::span<const float>(_silent_frame.data(), _silent_frame.size());
  }
  auto start_index = data_ptr + _pcm_data_position * channels;
  float amp = grain_amp_for_frame(_pcm_data_position);
  for (int i = 0; i < channels; i++) {
    _output_frame[i] = start_index[i] * amp;
  }
  _pcm_data_position++;
  if (_pcm_data_position >= _start_frame + _duration) {
    // Grain has finished, and should be inactive for next frame
    _state = Grain::State::INACTIVE;
  }
  return std::span<const float>(_output_frame.data(), _output_frame.size());
}

void Grain::set_attack(int32_t attack) noexcept { _attack = attack; }

void Grain::set_decay(int32_t decay) noexcept { _decay = decay; }

void Grain::make_active() noexcept { _state = Grain::State::ACTIVE; }

Grain::State Grain::get_state() const noexcept { return _state; };

float Grain::grain_amp_for_frame(int64_t frame) const noexcept {
  float amp = 1.0f;
  auto grain_rel_pos = grain_relative_position(frame);
  if (_attack >= 2 && grain_rel_pos < _attack) {
    amp = static_cast<float>(grain_rel_pos) / static_cast<float>(_attack);
  } else if (_decay >= 2 && grain_rel_pos >= _duration - _decay) {
    int32_t frames_remaining = _duration - grain_rel_pos;
    amp = static_cast<float>(frames_remaining - 1) / static_cast<float>(_decay);
  }
  // TODO: Could manually clamp to avoid including <algorithm>
  amp = std::clamp(amp, -1.0f, 1.0f);
  return amp;
}

uint64_t
Grain::grain_relative_position(uint64_t pcm_data_position) const noexcept {
  return pcm_data_position - _start_frame;
}

} // namespace kurt
