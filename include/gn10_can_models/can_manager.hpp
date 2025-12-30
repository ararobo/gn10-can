#pragma once

#include <vector>
#include "ican_transceiver.hpp"
#include "can_device.hpp"

namespace gn10_can_models {

/**
 * @brief Manages the CAN bus, dispatches received messages to registered devices.
 */
class CanManager {
public:
    /**
     * @brief Construct a new Can Manager
     * @param transceiver Reference to the hardware transceiver implementation
     */
    explicit CanManager(ICanTransceiver& transceiver);

    /**
     * @brief Register a device to be managed
     * @param device Pointer to the device
     */
    void register_device(CanDevice* device);

    /**
     * @brief Update loop. Should be called periodically.
     * Reads from transceiver and dispatches to devices.
     */
    void update();

    /**
     * @brief Send a frame via the transceiver
     * @param frame Frame to send
     * @return true if successful
     */
    bool send_frame(const CanFrame& frame);

private:
    ICanTransceiver& transceiver_;
    std::vector<CanDevice*> devices_;
};

} // namespace gn10_can_models
