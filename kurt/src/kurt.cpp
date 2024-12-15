#include "kurt.hpp"

#include <algorithm>
#include <iostream>

namespace kurt {

Kurt::Kurt() : _grain_store(std::array<Grain, GrainStore::MAX_GRAINS>()) {}

Kurt::~Kurt() = default;

std::optional<std::string>
Kurt::load_wav_file(const std::string &path) noexcept {

  auto result = _pcm_parser.parse(path);
  if (!result.has_value()) {
    std::cout << "failed to load pcm data" << std::endl;
    return result.error();
  } else {
    std::lock_guard<std::mutex> lock(_audio_buffer_mutex);
    _audio_buffer =
        std::make_shared<ThreadSafeAudioBuffer>(std::move(result.value()));

    return std::nullopt;
  }
}

void Kurt::set_sequence(uint32_t frame,
                        std::vector<GrainEvent> grains) noexcept {
  std::lock_guard<std::mutex> lock(_audio_buffer_mutex);
  _sequencer.set(frame, grains);
}

void Kurt::play() noexcept { _status.playing = true; }

void Kurt::stop() noexcept { _status.playing = false; }

Kurt::Status Kurt::status() const { return _status; }

const std::span<const float> Kurt::next_frame() noexcept {

  if (!_status.playing) {
    return std::span<const float>(_empty_frame);
  }

  std::lock_guard<std::mutex> lock(_audio_buffer_mutex);

  if (!_audio_buffer) {
    std::cout << "kurt: No audio buffer" << std::endl;
    return std::span<const float>(_empty_frame);
  }

  if (!_audio_buffer->has_audio_data()) {
    std::cout << "kurt: Warning: No pcm data" << std::endl;
    return std::span<const float>(_empty_frame);
  }

  // TODO: Move to private members when audio loaded
  auto channels = _audio_buffer->get_num_channels();
  auto frames = _audio_buffer->get_number_of_frames();
  auto data_ptr = _audio_buffer->get_audio_data_ptr();

  // TODO: Needed? Can this be handled by Grain?
  if (frames == 0) {
    std::cout << "no pcm data to play, returning silence" << std::endl;
    return std::span<const float>(_empty_frame);
  }

  // Activate new grain if one has been sequenced
  _conductor.next_frame();
  std::optional<uint16_t> new_subdivision = _conductor.new_subdivision();
  if (new_subdivision.has_value()) {

    auto events = _sequencer.get(*new_subdivision);
    for (auto &event : events) {
      activate_new_grain(event);
    }
  }

  _output_frame = {0.0f, 0.0f};

  // Get output from grains
  for (auto &grain : _grain_store.active_grains()) {

    auto frame = grain.next_frame();
    for (int i = 0; i < channels; i++) {
      _output_frame[i] += frame[i];
    }
  }

  for (auto &sample : _output_frame) {
    // TODO: Could manually clamp to avoid including <algorithm>
    sample = std::clamp(sample, -1.0f, 1.0f);
  }

  // TODO: Span needed here? Adds additional overhead?
  std::span<const float> frame(_output_frame);
  return frame;
}

void Kurt::set_sample_rate(uint32_t sample_rate) noexcept {
  _conductor.set_sample_rate(sample_rate);
}

void Kurt::activate_new_grain(GrainEvent event) noexcept {
  auto &grain = _grain_store.available_grain();

  // Ensure _audio_buffer is not being loaded
  grain.set_audio_buffer(_audio_buffer);

  grain.set_start_frame(event.start_frame);
  grain.set_duration(event.duration);
  grain.set_attack(event.attack);
  grain.set_decay(event.decay);

  grain.make_active();
}

} // namespace kurt
