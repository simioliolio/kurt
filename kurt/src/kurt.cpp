#include "kurt.hpp"

#include <iostream>

namespace kurt {

Kurt::Kurt() = default;

Kurt::~Kurt() = default;

ReadResult Kurt::load_wav_file(const std::string &path) {
  return _pcm_reader.read(path);
}

void Kurt::play() noexcept { _status.playing = true; }

void Kurt::stop() noexcept { _status.playing = false; }

Kurt::Status Kurt::status() const { return _status; }

Kurt::Frame Kurt::next_frame() noexcept {
  const auto &pcm_data = _pcm_reader.pcm_data();
  if (pcm_data.number_of_frames == 0) {
    std::cout << "no frames" << std::endl;
    return Frame(0, 0);
  }
  if (_current_frame >= pcm_data.number_of_frames) {
    std::cout << "resetting frame" << std::endl;
    _current_frame = 0;
  }
  auto left = (double)_pcm_reader.sample_at_frame(_current_frame, 0) /
              (double)(pow(2, pcm_data.bits_per_sample) - 1);
  auto right = (double)_pcm_reader.sample_at_frame(_current_frame, 1) /
               (double)(pow(2, pcm_data.bits_per_sample) - 1);
  _current_frame++;
  return Frame(left, right);
}

} // namespace kurt
