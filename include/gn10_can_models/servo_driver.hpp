#pragma once

#include "can_device.hpp"

namespace gn10_can_models {

/**
 * @brief Example Servo Driver implementation
 */
class ServoDriver : public CanDevice {
public:
    using CanDevice::CanDevice; // Inherit constructor

    /**
     * @brief Set the target angle
     * @param angle_deg Target angle in degrees
     */
    void set_angle(float angle_deg);

    /**
     * @brief Handle received CAN frames
     * @param frame Received frame
     */
    void on_receive(const CanFrame& frame) override;

    float get_current_angle() const { return current_angle_; }

private:
    float current_angle_ = 0.0f;
};

} // namespace gn10_can_models
