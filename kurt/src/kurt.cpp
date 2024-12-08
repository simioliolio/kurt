#include "kurt.hpp"

#include <algorithm>
#include <iostream>

namespace kurt {

Kurt::Kurt() : _grain_store(std::make_unique<std::vector<Grain>>()) {
  // TODO: Inject sequence. Setting simple init sequence for now.
  _sequencer.set(0, {{0, 44100, 10, 10}});
}

Kurt::~Kurt() = default;

std::optional<std::string>
Kurt::load_wav_file(const std::string &path) noexcept {

  auto result = _pcm_parser.parse(path);
  if (!result.has_value()) {
    std::cout << "failed to load pcm data" << std::endl;
    return result.error();
  } else {
    // TODO: Synchronise this with the creation of grains
    _audio_buffer = std::make_shared<ThreadSafeAudioBuffer>();
    _audio_buffer->acquire();
    _audio_buffer->set_audio_data(std::move(result.value()));
    _audio_buffer->release();

    return std::nullopt;
  }
}

void Kurt::play() noexcept { _status.playing = true; }

void Kurt::stop() noexcept { _status.playing = false; }

Kurt::Status Kurt::status() const { return _status; }

const std::span<const float> Kurt::next_frame() noexcept {

  if (!_status.playing) {
    return std::span<const float>(_empty_frame);
  }

  if (!_audio_buffer) {
    std::cout << "kurt: No audio buffer" << std::endl;
    return std::span<const float>(_empty_frame);
  }

  if (!_audio_buffer->has_audio_data()) {
    std::cout << "kurt: Warning: No pcm data" << std::endl;
    return std::span<const float>(_empty_frame);
  }

  // TODO: Remove this hack
  /**
   * Grains are unaware of threading aspect of the audio buffer.
   * We acquire and release in this function, granting thread
   * safety to the grain store indirectly. This is a bit of a
   * hack, but it's an optimisation for speed.
   */
  _audio_buffer->acquire();

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
    std::cout << "New subdivision! " << *new_subdivision << std::endl;

    auto events = _sequencer.get(*new_subdivision);
    for (auto &event : events) {
      std::cout << "activating new grain" << std::endl;
      // TODO: Optimisation: Ensure this does not happen on the audio thread, as
      // new grain is constructed here
      // Also pass on_finish callback to grain so it can be removed from store
      // when it finishes
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

  // TODO: Remove
  _audio_buffer->release();

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
  // TODO: Unsafe use of _audio_buffer, as UI could be changing it
  auto grain = Grain(_audio_buffer);
  grain.set_start_frame(event.start_frame);
  grain.set_duration(event.duration);
  grain.set_attack(event.attack);
  grain.set_decay(event.decay);
  grain.make_active();
  _grain_store.add_grain(std::move(grain));
}

} // namespace kurt
