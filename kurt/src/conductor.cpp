#include "conductor.hpp"

namespace kurt {

Conductor::Conductor(uint32_t sample_rate) noexcept
    : _sample_rate(sample_rate) {}

void Conductor::next_frame() noexcept {
  _frame_count++;

  if (_frame_count % _frames_per_subdivision == 0) {
    _playhead++;
    // TODO: Optimise by calculating number_of_subdivisions_per_loop once
    if (_playhead >= _subdivisions_per_beat * _number_of_beats) {
      _playhead = 0;
      _frame_count = 0;
    }
    _new_subdivision = _playhead;
  } else {
    if (_new_subdivision.has_value()) {
      _new_subdivision = std::nullopt;
    }
  }
}

void Conductor::set_bpm(uint16_t bpm) noexcept {
  _bpm = bpm;
  calculate_frames_per_subdivision();
}

void Conductor::set_subdivisions_per_beat(
    uint16_t subdivisions_per_beat) noexcept {
  _subdivisions_per_beat = subdivisions_per_beat;
  calculate_frames_per_subdivision();
}

void Conductor::set_number_of_beats(uint16_t number_of_beats) noexcept {
  _number_of_beats = number_of_beats;
}

void Conductor::calculate_frames_per_subdivision() noexcept {
  auto frames_per_beat = _sample_rate * 60 / _bpm;
  _frames_per_subdivision = frames_per_beat / _subdivisions_per_beat;
}

std::optional<uint16_t> Conductor::new_subdivision() const noexcept {
  return _new_subdivision;
}

} // namespace kurt
