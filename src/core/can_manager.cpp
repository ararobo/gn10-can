#include "gn10_can/core/can_manager.hpp"

#include "gn10_can/core/can_device.hpp"

namespace gn10_can {

CANManager::CANManager(drivers::DriverInterface& driver) : driver_(driver) {}

void CANManager::resister_device(CANDevice* device) {
    if (device) {
        devices_.push_back(device);
    }
}

void CANManager::update() {
    CANFrame rx_frame;
    while (driver_.receive(rx_frame)) {
        for (auto* device : devices_) {
            device->on_receive(rx_frame);
        }
    }
}

bool CANManager::send_frame(const CANFrame& frame) {
    return driver_.send(frame);
}

}  // namespace gn10_can