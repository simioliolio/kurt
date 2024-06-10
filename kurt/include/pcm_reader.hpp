#pragma once

#include <expected>
#include <string>
#include <vector>

namespace kurt {

struct PCMData {
  uint32_t sample_rate;
  uint8_t channels;
  uint16_t bits_per_sample;
  uint32_t data_size;
  uint32_t number_of_frames;
  std::vector<char> data;
};

/**
 * @class PCMReader
 * @brief Class for reading PCM data from a file.
 *
 * The PCMReader class provides functionality to read PCM data from a file.
 */
class PCMReader final {
public:
  /**
   * @brief Reads PCM data from the specified file.
   *
   * This function reads PCM data from the file specified by `file_path`.
   *
   * @param file_path The path to the PCM file.
   * @return An expected object containing the number of bytes read on success,
   * or an error message on failure.
   */
  std::expected<uint32_t, std::string> read(const std::string &file_path);

  /**
   * @brief Retrieves the sample value at the specified frame and channel.
   *
   * This function returns the sample value at the specified frame and channel.
   * If no channel is specified, it defaults to channel 0.
   *
   * @param frame The frame index.
   * @param channel The channel index. For stereo, 0 is left, 1 is right.
   * @return The twos-compliment sample value at the specified frame and
   * channel.
   */
  int32_t sample_at_frame(uint32_t frame, uint8_t channel);

private:
  PCMData _pcm_data = {0, 0, 0, 0, {}};
};

} // namespace kurt
