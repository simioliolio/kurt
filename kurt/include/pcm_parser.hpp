#include <expected>
#include <memory>
#include <vector>

#include "pcm_audio_data.hpp"

namespace kurt {

typedef std::expected<std::unique_ptr<PCMAudioData>, std::string> ParseResult;

class PCMParser final {
public:
  ParseResult parse(const std::string &file_path) const noexcept;
};

} // namespace kurt
