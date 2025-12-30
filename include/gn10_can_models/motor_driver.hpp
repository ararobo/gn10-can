#pragma once

#include "can_device.hpp"

namespace gn10_can_models {

/**
 * @brief Example Motor Driver implementation
 */
class MotorDriver : public CanDevice {
public:
    using CanDevice::CanDevice; // Inherit constructor

    /**
     * @brief Set the target velocity
     * @param velocity Target velocity
     */
    void set_velocity(float velocity);

    /**
     * @brief Handle received CAN frames
     * @param frame Received frame
     */
    void on_receive(const CanFrame& frame) override;

    // Getters for feedback data
    float get_current_velocity() const { return current_velocity_; }
    float get_temperature() const { return temperature_; }

private:
    float current_velocity_ = 0.0f;
    float temperature_ = 0.0f;
};

} // namespace gn10_can_models
