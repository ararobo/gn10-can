#include "gn10_can_models/can_manager.hpp"

namespace gn10_can_models {

CanManager::CanManager(ICanTransceiver& transceiver)
    : transceiver_(transceiver) {
}

void CanManager::register_device(CanDevice* device) {
    if (device) {
        devices_.push_back(device);
    }
}

void CanManager::update() {
    CanFrame frame;
    // Process all available frames in the receive queue
    while (transceiver_.receive(frame)) {
        for (auto* device : devices_) {
            if (device->get_rx_id() == frame.id) {
                device->on_receive(frame);
                // Assuming one device handles one ID. 
                // If multiple devices need the same ID, remove this break.
                break; 
            }
        }
    }
}

bool CanManager::send_frame(const CanFrame& frame) {
    return transceiver_.send(frame);
}

} // namespace gn10_can_models
