#include <stdexcept>

#include "audio_buffer.hpp"

namespace kurt::test {

class StubAudioBuffer : public AudioBuffer {
public:
  bool has_audio_data() const noexcept override { return true; }

  PCMAudioData &get_audio_data() const override { return _pcm_data; }

  float *get_audio_data_ptr() noexcept override {
    return _pcm_data.normalized_data.data();
  }
  uint32_t get_sample_rate() const noexcept override {
    return _pcm_data.sample_rate;
  }
  uint16_t get_num_channels() const noexcept override {
    return _pcm_data.channels;
  }
  uint32_t get_number_of_frames() const noexcept override {
    return _pcm_data.normalized_data.size() / _pcm_data.channels;
  }

  void set_audio_data(PCMAudioData pcm_data) noexcept { _pcm_data = pcm_data; }

  PCMAudioData stub_pcm_data() {
    return {44100,
            2,
            6,
            {0.0f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f, -1.0f, -1.0f, -0.5f, -0.5f,
             -0.2f, -0.2f}};
  }

  PCMAudioData stub_full_scale_pcm_data() {
    return {44100,
            2,
            5,
            {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}};
  }

private:
  mutable PCMAudioData _pcm_data = stub_pcm_data();
};

} // namespace kurt::test