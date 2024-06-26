#pragma once

#include "pcm_parser.hpp"

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

private:
  PCMParser _pcm_parser;
  std::unique_ptr<PCMAudioData> _pcm_data;
  std::mutex _pcm_data_mutex;
  Status _status = {};
  uint32_t _current_frame = 0;
  std::vector<float> _empty_frame = {0.0f, 0.0f};
};

} // namespace kurt
