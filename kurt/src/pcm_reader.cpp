#include "pcm_reader.hpp"

#include <fstream>

namespace kurt {

std::expected<uint32_t, std::string> PCMReader::read(const std::string &file_path)
{
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

    return _pcm_data.data_size;
}

} // namespace kurt
