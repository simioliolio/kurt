#pragma once

#include <cstdint>
#include <vector>

namespace kurt {

struct PCMAudioData {
  uint32_t sample_rate;
  uint16_t channels;
  uint32_t number_of_frames;
  std::vector<float> normalized_data; // Interleaved if multichannel
};

} // namespace kurt
