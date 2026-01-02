#pragma once

#include <array>
#include <cstdint>

namespace gn10_can {

/**
 * @brief CAN frame structure
 *
 */
struct CANFrame {
    static constexpr uint8_t MAX_DLC = 8;

    uint32_t id = 0;                      // CAN ID
    std::array<uint8_t, MAX_DLC> data{};  // Data payload
    uint8_t dlc = 0;                      // Data length code (DLC)

    // additional attribute
    bool is_extended = false;
    bool is_rtr      = false;
    bool is_error    = false;

    bool operator==(const CANFrame& other) const {
        if (id != other.id || dlc != other.dlc || is_extended != other.is_extended ||
            is_rtr != other.is_rtr || is_error != other.is_error) {
            return false;
        }

        size_t loop_limit = (dlc > MAX_DLC) ? MAX_DLC : dlc;

        for (size_t i = 0; i < loop_limit; i++) {
            if (data[i] != other.data[i]) return false;
        }
        return true;
    }

    bool operator!=(const CANFrame& other) const { return !(*this == other); }
};

}  // namespace gn10_can