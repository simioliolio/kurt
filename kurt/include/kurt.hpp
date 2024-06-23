#pragma once

#include "pcm_reader.hpp"

#include <span>

namespace kurt {

class Kurt {

  struct Status {
    bool playing = false;
  };

public:
  Kurt();
  ~Kurt();

  ReadResult load_wav_file(const std::string &path);

  void play() noexcept;
  void stop() noexcept;

  Status status() const;

  const std::span<const float> next_frame() noexcept;
  const uint32_t &current_frame() const noexcept;

private:
  PCMReader _pcm_reader;
  Status _status = {};
  uint32_t _current_frame = 0;
  std::vector<float> _empty_frame = {0.0f, 0.0f};
};

} // namespace kurt
