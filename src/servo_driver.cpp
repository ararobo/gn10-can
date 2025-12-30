#include "gn10_can_models/servo_driver.hpp"
#include <cstring> // for std::memcpy

namespace gn10_can_models {

void ServoDriver::set_angle(float angle_deg) {
    // Simple protocol: Send float as 4 bytes
    send(reinterpret_cast<const uint8_t*>(&angle_deg), sizeof(angle_deg));
}

void ServoDriver::on_receive(const CanFrame& frame) {
    if (frame.len >= 4) {
        std::memcpy(&current_angle_, frame.data.data(), sizeof(float));
    }
}

} // namespace gn10_can_models
