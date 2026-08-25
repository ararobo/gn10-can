#pragma once
#include <cstdint>

namespace gn10_can {
namespace devices {
struct RGB {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} __attribute__((__packed__));

enum class ShowTypeLED : uint8_t {
    Normal    = 0,
    Spinning  = 1,
    Gradually = 2,
};
}  // namespace devices
}  // namespace gn10_can