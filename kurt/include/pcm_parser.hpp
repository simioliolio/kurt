#include <expected>
#include <memory>
#include <vector>

namespace kurt {

struct PCMAudioData {
  uint32_t sample_rate;
  uint16_t channels;
  uint32_t number_of_frames;
  std::vector<float> normalized_data; // Interleaved if multichannel
};

typedef std::expected<std::unique_ptr<PCMAudioData>, std::string> ParseResult;

class PCMParser final {
public:
  ParseResult parse(const std::string &file_path) const noexcept;
};

} // namespace kurt