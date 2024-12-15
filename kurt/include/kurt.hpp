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

  /**
   * @brief Set the sequence of grains to play.
   *
   * @param frame The frame number to start the grain events.
   * @param sequence The grains to start.
   * @note This function is thread-safe.
   */
  void set_sequence(uint32_t frame, std::vector<GrainEvent> grains) noexcept;

  void play() noexcept;
  void stop() noexcept;

  Status status() const;

  /**
   * @brief Get the next frame of audio data.
   *
   * @return std::span<const float> The next frame of audio data.
   * @note This function is thread-safe.
   *
   * TODO: Might introduce an api here to get multiple frames at once
   * on a buffer-by-buffer basis.
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
   * @brief Set the sample rate of the audio hardware.
   * This allows Kurt to have a sense of time.
   *
   * @note This function is not thread-safe, and should be set
   * before calling play.
   */
  void set_sample_rate(uint32_t sample_rate) noexcept;

private:
  PCMParser _pcm_parser;

  // TODO: Make thread safe
  // Audio buffer should be passed to newly constructed grains
  // but can also be changed when loading a new file. These two
  // operations should both happen off the audio thread.
  std::shared_ptr<ThreadSafeAudioBuffer> _audio_buffer;
  std::mutex _audio_buffer_mutex;

  Status _status = {};
  std::vector<float> _empty_frame = {0.0f, 0.0f};
  std::vector<float> _output_frame = {0.0f, 0.0f};
  GrainStore _grain_store;
  Conductor _conductor;
  Sequencer<uint16_t, GrainEvent> _sequencer;
  void activate_new_grain(GrainEvent event) noexcept;
};

} // namespace kurt
