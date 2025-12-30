#include "gn10_can_models/can_device.hpp"
#include "gn10_can_models/can_manager.hpp"
#include <algorithm> // for std::copy

namespace gn10_can_models {

CanDevice::CanDevice(CanManager& manager, uint32_t rx_id, uint32_t tx_id)
    : manager_(manager), rx_id_(rx_id), tx_id_(tx_id) {
}

bool CanDevice::send(const uint8_t* data, uint8_t len) {
    CanFrame frame;
    frame.id = tx_id_;
    frame.len = (len > 8) ? 8 : len;
    
    // Clear data buffer
    frame.data.fill(0);
    
    if (data != nullptr && len > 0) {
        std::copy(data, data + frame.len, frame.data.begin());
    }
    
    return manager_.send_frame(frame);
}

} // namespace gn10_can_models
