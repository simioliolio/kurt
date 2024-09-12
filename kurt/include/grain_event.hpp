#include <cstdint>
namespace kurt {

struct GrainEvent {
  uint32_t start_frame;
  uint32_t duration;
  uint32_t attack;
  uint32_t decay;
};

} // namespace kurt
