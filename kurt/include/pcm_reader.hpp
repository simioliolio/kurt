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
    std::vector<char> data;
};

class PCMReader final
{
public:
    PCMReader() = default;
    ~PCMReader() = default;

    std::expected<uint32_t, std::string> read(const std::string &file_path);

private:
    PCMData _pcm_data = {0, 0, 0, 0, {}};
};

} // namespace kurt
