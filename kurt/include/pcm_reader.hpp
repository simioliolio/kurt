#pragma once

#include <expected>
#include <mutex>
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

typedef std::expected<uint32_t, std::string> ReadResult;

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
   * This method is thread-safe.
   *
   * @param file_path The path to the PCM file.
   * @return An expected object containing the number of bytes read on success,
   * or an error message on failure.
   */
  ReadResult read(const std::string &file_path);

  /**
   * @brief Retrieves the sample value at the specified frame and channel.
   *
   * This function returns the sample value at the specified frame and channel.
   * If no channel is specified, it defaults to channel 0.
   * This method is thread-safe.
   *
   * @param frame The frame index.
   * @param channel The channel index. For stereo, 0 is left, 1 is right.
   * @return The twos-compliment sample value at the specified frame and
   * channel.
   */
  int32_t sample_at_frame(uint32_t frame, uint8_t channel) const noexcept;

  /**
   * @brief Get the PCM data.
   *
   * This function returns the PCMData object representing the PCM data.
   * If no audio is loaded, the PCMData object will have all fields set to 0.
   * This method is thread-safe, and returns a copy.
   *
   * @return PCMData The PCMData object.
   */
  const PCMData &pcm_data() const noexcept;

private:
  PCMData _pcm_data = {0, 0, 0, 0, {}};
  mutable std::mutex _pcm_data_mutex;
};

} // namespace kurt
