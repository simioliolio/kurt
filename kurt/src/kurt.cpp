#include "kurt.hpp"

#include <iostream>

namespace kurt {

Kurt::Kurt() = default;

Kurt::~Kurt() = default;

std::optional<std::string>
Kurt::load_wav_file(const std::string &path) noexcept {

  std::lock_guard<std::mutex> lock(_pcm_data_mutex);

  auto result = _pcm_parser.parse(path);
  if (!result.has_value()) {
    return result.error();
  } else {
    _pcm_data = std::move(result.value());
    return std::nullopt;
  }
}

void Kurt::play() noexcept { _status.playing = true; }

void Kurt::stop() noexcept { _status.playing = false; }

Kurt::Status Kurt::status() const { return _status; }

const std::span<const float> Kurt::next_frame() noexcept {

  std::lock_guard<std::mutex> lock(_pcm_data_mutex);

  if (!_pcm_data) {
    std::cout << "Warning: No pcm data" << std::endl;
    return std::span<const float>(_empty_frame);
  }
  if (_pcm_data->number_of_frames == 0) {
    std::cout << "no frames" << std::endl;
    return std::span<const float>(_empty_frame);
  }
  if (_current_frame >= _pcm_data->number_of_frames) {
    std::cout << "resetting frame" << std::endl;
    _current_frame = 0;
  }

  auto start_index =
      _pcm_data->normalized_data.data() + _current_frame * _pcm_data->channels;
  std::span<const float> frame(start_index, _pcm_data->channels);
  _current_frame++;
  return frame;
}

const uint32_t &Kurt::current_frame() const noexcept { return _current_frame; }

} // namespace kurt
