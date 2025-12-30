#pragma once

#include "can_frame.hpp"

namespace gn10_can_models {

/**
 * @brief Interface for CAN transceiver hardware abstraction.
 * Implement this interface for specific platforms (ROS 2, STM32 HAL, ESP32 TWAI, etc.)
 */
class ICanTransceiver {
public:
    virtual ~ICanTransceiver() = default;

    /**
     * @brief Send a CAN frame
     * @param frame The frame to send
     * @return true if sent successfully, false otherwise
     */
    virtual bool send(const CanFrame& frame) = 0;

    /**
     * @brief Receive a CAN frame (non-blocking)
     * @param out_frame [out] The received frame
     * @return true if a frame was received, false if queue is empty
     */
    virtual bool receive(CanFrame& out_frame) = 0;
};

} // namespace gn10_can_models
