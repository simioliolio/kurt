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

const std::span<const float> Kurt::next_frame() noexcept {
  const auto &pcm_data = _pcm_reader.pcm_data();
  // TODO: Fix non-thread-safe use of pcm_data
  if (pcm_data.number_of_frames == 0) {
    std::cout << "no frames" << std::endl;
    return std::span<const float>(_empty_frame);
  }
  if (_current_frame >= pcm_data.number_of_frames) {
    std::cout << "resetting frame" << std::endl;
    _current_frame = 0;
  }

  auto start_index =
      pcm_data.normalized_data.data() + _current_frame * pcm_data.channels;
  std::span<const float> frame(start_index, pcm_data.channels);
  _current_frame++;
  return frame;
}

const uint32_t &Kurt::current_frame() const noexcept { return _current_frame; }

} // namespace kurt
