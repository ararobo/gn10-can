#pragma once

#include <cstdint>
#include "can_frame.hpp"

namespace gn10_can_models {

class CanManager; // Forward declaration

/**
 * @brief Abstract base class for all CAN devices.
 */
class CanDevice {
public:
    /**
     * @brief Construct a new Can Device
     * @param manager Reference to the CanManager
     * @param rx_id CAN ID to listen for
     * @param tx_id CAN ID to transmit on
     */
    CanDevice(CanManager& manager, uint32_t rx_id, uint32_t tx_id);
    virtual ~CanDevice() = default;

    /**
     * @brief Callback when a frame with rx_id is received
     * @param frame The received frame
     */
    virtual void on_receive(const CanFrame& frame) = 0;

    /**
     * @brief Get the RX ID
     * @return uint32_t 
     */
    uint32_t get_rx_id() const { return rx_id_; }

protected:
    /**
     * @brief Helper to send data using the manager
     * @param data Pointer to data
     * @param len Length of data
     * @return true if sent successfully
     */
    bool send(const uint8_t* data, uint8_t len);

    CanManager& manager_;
    uint32_t rx_id_;
    uint32_t tx_id_;
};

} // namespace gn10_can_models
