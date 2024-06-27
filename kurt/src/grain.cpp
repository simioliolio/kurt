#include "grain.hpp"

#include <algorithm>
#include <iostream>

namespace kurt {

Grain::Grain(std::shared_ptr<PCMAudioData> pcm_data) : _pcm_data(pcm_data) {
  _output_frame.resize(_pcm_data->channels);
}

void Grain::set_position(int64_t position) noexcept { _position = position; }

int64_t Grain::get_position() const noexcept { return _position; }

const std::span<const float> Grain::next_frame() noexcept {
  if (_position >= _pcm_data->number_of_frames) {
    std::cout << "Warning: Grain position exceeds number of frames, outputting "
                 "silence"
              << std::endl;
    _output_frame.resize(_pcm_data->channels, 0.0f);
    return std::span<const float>(_output_frame.data(), _output_frame.size());
  }
  auto start_index =
      _pcm_data->normalized_data.data() + _position * _pcm_data->channels;
  float amp = grain_amp_for_frame(_position);
  for (int i = 0; i < _pcm_data->channels; i++) {
    _output_frame[i] = start_index[i] * amp;
  }
  _position++;
  return std::span<const float>(_output_frame.data(), _output_frame.size());
}

void Grain::set_attack(int32_t attack) noexcept { _attack = attack; }

void Grain::set_decay(int32_t decay) noexcept { _decay = decay; }

float Grain::grain_amp_for_frame(int64_t frame) const noexcept {
  float amp = 1.0f;
  if (_attack >= 2 && frame < _attack) {
    amp = static_cast<float>(frame) / static_cast<float>(_attack);
  } else if (_decay >= 2 && frame >= _pcm_data->number_of_frames - _decay) {
    int32_t frames_remaining = _pcm_data->number_of_frames - frame;
    amp = static_cast<float>(frames_remaining - 1) / static_cast<float>(_decay);
  }
  // TODO: Could manually clamp to avoid including <algorithm>
  amp = std::clamp(amp, 0.0f, 1.0f);
  return amp;
}

} // namespace kurt
