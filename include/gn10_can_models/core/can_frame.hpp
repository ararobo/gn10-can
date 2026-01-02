#pragma once

#include <array>
#include <cstdint>

namespace gn10_can {

struct CANFrame {
    uint32_t id = 0;
    std::array<uint8_t, 8> data{};
    uint8_t len = 0;
};

}  // namespace gn10_can