#include "gn10_can/core/can_manager.hpp"
#include "gn10_can/core/can_device.hpp"

namespace gn10_can {

CANManager::CANManager(drivers::DriverInterface& driver) : driver_(driver) {}

void CANManager::register_device(CANDevice* device, uint32_t rx_id) {
    subscribers_[rx_id] = device;
}

void CANManager::update() {
    CANFrame frame;
    // Mask to extract Type + DeviceID (ignoring Command bits 0-2)
    // 0x7F8 = 0000 0111 1111 1000
    constexpr uint32_t ROUTING_MASK = 0x7F8;

    while (driver_.receive(frame)) {
        // Find subscriber by Base ID (Type + DeviceID)
        uint32_t base_id = frame.id & ROUTING_MASK;
        auto it = subscribers_.find(base_id);
        if (it != subscribers_.end()) {
            it->second->on_receive(frame);
        }
    }
}

bool CANManager::send_frame(const CANFrame& frame) {
    return driver_.send(frame);
}

}  // namespace gn10_can
