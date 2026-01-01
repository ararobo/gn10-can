#pragma once

#include <cstdint>
#include <array>

namespace gn10_can_models {

/**
 * @brief Standard CAN frame structure
 */
struct CanFrame {
    uint32_t id;                ///< CAN ID (Standard or Extended)
    std::array<uint8_t, 8> data;///< Data payload
    uint8_t len;                ///< Data length code (DLC), 0-8
};

} // namespace gn10_can_models
