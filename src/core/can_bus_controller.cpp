#include "gn10_can/core/can_bus_controller.hpp"

namespace gn10_can {

CANBusController::CANBusController(drivers::DriverInterface& driver) : driver_(driver) {}

void CANBusController::register_receiver(ICANReceiver* receiver) {
    if (receiver) {
        router_[receiver->get_target_id()] = receiver;
    }
}

void CANBusController::update() {
    CANFrame rx_frame;
    // Mask to extract Type + DeviceID (ignoring Command bits 0-2)
    // 0x7F8 = 0000 0111 1111 1000
    constexpr uint32_t ROUTING_MASK = 0x7F8;

    while (driver_.receive(rx_frame)) {
        // Find receiver for this frame's ID (masked)
        uint32_t target = rx_frame.id & ROUTING_MASK;
        auto it = router_.find(target);
        if (it != router_.end()) {
            it->second->on_receive(rx_frame);
        }
    }
}

bool CANBusController::send_frame(const CANFrame& frame) {
    return driver_.send(frame);
}

}  // namespace gn10_can
