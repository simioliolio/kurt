#pragma once

#include "conductor.hpp"
#include "grain_event.hpp"
#include "grain_store.hpp"
#include "pcm_parser.hpp"
#include "sequencer.hpp"
#include "thread_safe_audio_buffer.hpp"

#include <memory>
#include <mutex>
#include <optional>
#include <span>

namespace kurt {

class Kurt {

  struct Status {
    bool playing = false;
  };

public:
  Kurt();
  ~Kurt();

  /**
   * @brief Load a WAV file from disk.
   *
   * @param path The path to the WAV file.
   * @return std::optional<std::string> An error message if the file could not
   * be loaded.
   * @note This function is thread-safe.
   */
  std::optional<std::string> load_wav_file(const std::string &path) noexcept;

  void play() noexcept;
  void stop() noexcept;

  Status status() const;

  /**
   * @brief Get the next frame of audio data.
   *
   * @return std::span<const float> The next frame of audio data.
   * @note This function is thread-safe.
   */
  const std::span<const float> next_frame() noexcept;

  /**
   * @brief Get the current frame number.
   *
   * @return const uint32_t& The current frame number.
   * @note This function is not thread-safe.
   */
  const uint32_t &current_frame() const noexcept;

  /**
   * @brief Set the sample rate of the audio hardware
   */
  void set_sample_rate(uint32_t sample_rate) noexcept;

private:
  PCMParser _pcm_parser;
  std::shared_ptr<ThreadSafeAudioBuffer> _audio_buffer;
  Status _status = {};
  std::vector<float> _empty_frame = {0.0f,
                                     0.0f}; // TODO: Just use _output_frame?
  std::vector<float> _output_frame = {0.0f, 0.0f};
  GrainStore _grain_store;
  Conductor _conductor;
  Sequencer<uint16_t, GrainEvent> _sequencer;
  void activate_new_grain(GrainEvent event) noexcept;
  Grain _grain;
};

} // namespace kurt
