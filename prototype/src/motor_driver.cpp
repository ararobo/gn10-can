#include "gn10_can_models/motor_driver.hpp"
#include <cstring> // for std::memcpy

namespace gn10_can_models {

void MotorDriver::set_velocity(float velocity) {
    // Simple protocol: Send float as 4 bytes
    // In a real application, you might want to use a specific endianness or scaling
    send(reinterpret_cast<const uint8_t*>(&velocity), sizeof(velocity));
}

void MotorDriver::on_receive(const CanFrame& frame) {
    if (frame.len >= 4) {
        // Assume first 4 bytes are velocity
        std::memcpy(&current_velocity_, frame.data.data(), sizeof(float));
    }
    if (frame.len >= 8) {
        // Assume next 4 bytes are temperature
        std::memcpy(&temperature_, frame.data.data() + 4, sizeof(float));
    }
}

} // namespace gn10_can_models
