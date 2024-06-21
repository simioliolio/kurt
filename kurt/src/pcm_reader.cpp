#include "pcm_reader.hpp"

#include <cassert>
#include <fstream>

namespace kurt {

ReadResult PCMReader::read(const std::string &file_path) {

  std::lock_guard<std::mutex> lock(_pcm_data_mutex);

  std::ifstream file(file_path, std::ios::binary);
  if (!file.is_open()) {
    return std::unexpected("Failed to open file: " + file_path);
  }

  // Read RIFF header
  char riff_header[4];
  file.read(riff_header, 4);
  if (std::string(riff_header, 4) != "RIFF") {
    return std::unexpected("Invalid RIFF header");
  }

  // Read file size
  uint32_t file_size;
  file.read(reinterpret_cast<char *>(&file_size), 4);

  // Read WAVE header
  char wave_header[4];
  file.read(wave_header, 4);
  if (std::string(wave_header, 4) != "WAVE") {
    return std::unexpected("Invalid WAVE header");
  }

  // Read fmt subchunk
  char fmt_subchunk[4];
  file.read(fmt_subchunk, 4);
  if (std::string(fmt_subchunk, 4) != "fmt ") {
    return std::unexpected("Invalid fmt subchunk");
  }

  // Read fmt subchunk size
  uint32_t fmt_subchunk_size;
  file.read(reinterpret_cast<char *>(&fmt_subchunk_size), 4);

  // Read audio format
  uint16_t audio_format;
  file.read(reinterpret_cast<char *>(&audio_format), 2);
  if (audio_format != 1) {
    return std::unexpected("Unsupported audio format");
  }

  // Read number of channels
  file.read(reinterpret_cast<char *>(&_pcm_data.channels), 2);

  // Read sample rate
  file.read(reinterpret_cast<char *>(&_pcm_data.sample_rate), 4);

  // Read byte rate
  uint32_t byte_rate;
  file.read(reinterpret_cast<char *>(&byte_rate), 4);

  // Read block align
  uint16_t block_align;
  file.read(reinterpret_cast<char *>(&block_align), 2);

  // Read bits per sample
  file.read(reinterpret_cast<char *>(&_pcm_data.bits_per_sample), 2);

  // Read data subchunk
  char data_subchunk[4];
  file.read(data_subchunk, 4);
  if (std::string(data_subchunk, 4) != "data") {
    return std::unexpected("Invalid data subchunk");
  }

  // Read data subchunk size
  file.read(reinterpret_cast<char *>(&_pcm_data.data_size), 4);

  // Read data
  _pcm_data.data.resize(_pcm_data.data_size);
  file.read(_pcm_data.data.data(), _pcm_data.data_size);

  _pcm_data.number_of_frames =
      _pcm_data.data_size /
      (_pcm_data.channels * (_pcm_data.bits_per_sample / 8));

  return _pcm_data.data_size;
}

int32_t PCMReader::sample_at_frame(uint32_t frame,
                                   uint8_t channel) const noexcept {

  // TODO: Better thread handling. Prevent costly lock every sample?
  std::lock_guard<std::mutex> lock(_pcm_data_mutex);

  assert(frame < _pcm_data.number_of_frames);
  assert(channel < _pcm_data.channels);

  auto bytes_per_sample = _pcm_data.bits_per_sample / 8;
  auto sample_index = frame * _pcm_data.channels + channel;
  auto byte_index = sample_index * bytes_per_sample;

  int32_t sample = 0;

  // Collect sample bytes and shift to the correct position
  for (size_t i = 0; i < bytes_per_sample; i++) {
    sample |= static_cast<uint8_t>(_pcm_data.data[byte_index + i]) << (i * 8);
  }

  // For 32-bit, we are done (as our type is int32_t and honors twos-compliment)
  // For lower bitrates, we need to sign-extend the sample.
  if (_pcm_data.bits_per_sample < 32) {
    int shift = 32 - _pcm_data.bits_per_sample;
    // The left shift pushes the sign bit to the msb. The right shift is
    // arithmetic and preserves the sign bit as the bits return to their
    // original position.
    sample = (sample << shift) >> shift;
  }

  return sample;
}

const PCMData &PCMReader::pcm_data() const noexcept {
  std::lock_guard<std::mutex> lock(_pcm_data_mutex);
  return _pcm_data;
}

} // namespace kurt
