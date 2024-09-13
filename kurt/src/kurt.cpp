#include "kurt.hpp"

#include <algorithm>
#include <iostream>

namespace kurt {

Kurt::Kurt()
    : _audio_buffer(std::make_shared<ThreadSafeAudioBuffer>()),
      _grain_store(GrainStore(
          std::make_unique<std::vector<Grain>>(5, Grain(_audio_buffer)))),
      _grain(_audio_buffer) {
  // Set a simple init sequence
  _sequencer.set(0, {{0, 44100, 10, 10}});
  _grain.set_start_frame(0);
  _grain.set_duration(44100);
  _grain.set_attack(0);
  _grain.set_decay(0);
  _grain.make_active();
}

Kurt::~Kurt() = default;

std::optional<std::string>
Kurt::load_wav_file(const std::string &path) noexcept {

  auto result = _pcm_parser.parse(path);
  if (!result.has_value()) {
    std::cout << "failed to load pcm data" << std::endl;
    return result.error();
  } else {
    _audio_buffer->acquire();
    _audio_buffer->set_audio_data(std::move(result.value()));

    // No need to check if audio data is set, as we just set it
    _sequencer.set(
        0, {{0, _audio_buffer->get_audio_data().number_of_frames, 10, 10}});
    _audio_buffer->release();

    return std::nullopt;
  }
}

void Kurt::play() noexcept { _status.playing = true; }

void Kurt::stop() noexcept { _status.playing = false; }

Kurt::Status Kurt::status() const { return _status; }

const std::span<const float> Kurt::next_frame() noexcept {

  if (!_audio_buffer->has_audio_data()) {
    std::cout << "kurt: Warning: No pcm data" << std::endl;
    return std::span<const float>(_empty_frame);
  }

  /**
   * Grains are unaware of threading aspect of the audio buffer.
   * We acquire and release in this function, granting thread
   * safety to the grain store indirectly. This is a bit of a
   * hack, but it's an optimisation for speed.
   */
  _audio_buffer->acquire();

  auto pcm_data = _audio_buffer->get_audio_data();
  // TODO: Needed? Can this be handled by Grain?
  if (pcm_data.number_of_frames == 0) {
    std::cout << "no pcm data to play, returning silence" << std::endl;
    return std::span<const float>(_empty_frame);
  }

  // Activate new grain if one has been sequenced
  // _conductor.next_frame();
  // std::optional<uint16_t> new_subdivision = _conductor.new_subdivision();
  // if (new_subdivision.has_value()) {
  //   std::cout << "New subdivision has_value" << std::endl;

  //   auto events = _sequencer.get(*new_subdivision);
  //   for (auto &event : events) {
  //     std::cout << "activating new grain" << std::endl;
  //     activate_new_grain(event);
  //   }
  // }

  // // Get output from grains
  // for (auto &grain : _grain_store.active_grains()) {

  //   auto frame = grain.next_frame();
  //   for (int i = 0; i < pcm_data.channels; i++) {
  //     _output_frame[i] += frame[i];
  //   }
  // }

  _output_frame = {0.0f, 0.0f};

  std::span<const float> grain_frame = _grain.next_frame();
  for (int i = 0; i < pcm_data.channels; i++) {
    _output_frame[i] = grain_frame[i];
  }

  _audio_buffer->release();

  for (auto &sample : _output_frame) {
    // TODO: Could manually clamp to avoid including <algorithm>
    sample = std::clamp(sample, -1.0f, 1.0f);
  }

  std::span<const float> frame(_output_frame);
  return frame;
}

void Kurt::set_sample_rate(uint32_t sample_rate) noexcept {
  _conductor.set_sample_rate(sample_rate);
}

void Kurt::activate_new_grain(GrainEvent event) noexcept {
  auto &grain = _grain_store.available_grain();
  grain.set_start_frame(event.start_frame);
  grain.set_duration(event.duration);
  grain.set_attack(event.attack);
  grain.set_decay(event.decay);
  grain.make_active();
}

} // namespace kurt
