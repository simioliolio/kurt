#pragma once

#include "pcm_reader.hpp"

namespace kurt {

class Kurt {

  struct Status {
    bool playing = false;
  };

  // TODO: A better model for this? Could avoid the value copy somehow?
  // TODO: Don't assume double?
  struct Frame {
    Frame(double left, double right) : samples{left, right} {};
    double samples[2];
  };

public:
  Kurt();
  ~Kurt();

  ReadResult load_wav_file(const std::string &path);

  void play() noexcept;
  void stop() noexcept;

  Status status() const;

  Frame next_frame() noexcept;

private:
  PCMReader _pcm_reader;
  Status _status = {};
  uint32_t _current_frame = 0;
};

} // namespace kurt
