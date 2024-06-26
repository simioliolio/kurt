#include "pcm_parser.hpp"

#include <fstream>
#include <iostream>

namespace kurt {

ParseResult PCMParser::parse(const std::string &file_path) const noexcept {

  std::ifstream file(file_path, std::ios::binary);

  PCMAudioData pcm_data;

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
  uint16_t channels;
  file.read(reinterpret_cast<char *>(&pcm_data.channels), 2);

  // Read sample rate
  uint32_t sample_rate;
  file.read(reinterpret_cast<char *>(&pcm_data.sample_rate), 4);

  // Read byte rate
  uint32_t byte_rate;
  file.read(reinterpret_cast<char *>(&byte_rate), 4);

  // Read block align
  uint16_t block_align;
  file.read(reinterpret_cast<char *>(&block_align), 2);

  // Read bits per sample
  uint16_t bits_per_sample;
  file.read(reinterpret_cast<char *>(&bits_per_sample), 2);

  // Read data subchunk
  char data_subchunk[4];
  file.read(data_subchunk, 4);
  if (std::string(data_subchunk, 4) != "data") {
    return std::unexpected("Invalid data subchunk");
  }

  // Read data subchunk size
  uint32_t data_size;
  file.read(reinterpret_cast<char *>(&data_size), 4);

  // Read data
  std::vector<float> data(data_size);
  uint16_t bytes_per_sample = bits_per_sample / 8;
  uint32_t number_of_samples = data_size / bytes_per_sample;
  pcm_data.normalized_data.resize(number_of_samples);
  uint32_t bytes_read = 0;
  for (uint32_t sample_index = 0; sample_index < number_of_samples;
       sample_index++) {
    char raw_sample[bytes_per_sample];
    file.read(raw_sample, bytes_per_sample);
    bytes_read += bytes_per_sample;
    // Collect sample bytes and shift to the correct position
    int32_t sample = 0;
    for (size_t byte_index = 0; byte_index < bytes_per_sample; byte_index++) {
      sample |= static_cast<uint8_t>(raw_sample[byte_index])
                << (byte_index * 8);
    }
    // For 32-bit, we are done (as our type is int32_t and honors
    // twos-compliment) For lower bitrates, we need to sign-extend the sample.
    if (bits_per_sample < 32) {
      int shift = 32 - bits_per_sample;
      // The left shift pushes the sign bit to the msb. The right shift is
      // arithmetic and preserves the sign bit as the bits return to their
      // original position.
      sample = (sample << shift) >> shift;
    }
    // Normalize the sample, so that it is in the range [-1, 1]
    pcm_data.normalized_data[sample_index] =
        static_cast<float>(sample) /
        static_cast<float>((1 << (bits_per_sample - 1)) - 1);
  }

  pcm_data.number_of_frames =
      data_size / (pcm_data.channels * bytes_per_sample);

  return std::make_unique<PCMAudioData>(pcm_data);
}
} // namespace kurt
