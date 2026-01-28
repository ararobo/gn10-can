#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/core/can_device.hpp"
#include "gn10_can/core/can_frame.hpp"
#include "gn10_can/core/can_id.hpp"

namespace gn10_can {

CANBus::CANBus(drivers::DriverInterface& driver) : driver_(driver) {}

void CANBus::update() {
    CANFrame frame;
    while (driver_.receive(frame)) {
        // コマンド部は下位ビットにあるため、それを除外してデバイスを検索
        // pack仕様: Type(4) | ID(4) | Cmd(3) -> Total 11 bits
        // キーは Type(4) | ID(4) なので、右に3シフト
        uint32_t device_key = frame.id >> 3;
        
        auto it = devices_.find(device_key);
        if (it != devices_.end()) {
            it->second->on_receive(frame);
        }
    }
}

bool CANBus::send_raw(const CANFrame& frame) {
    return driver_.send(frame);
}

void CANBus::attach(CANDevice* device, uint32_t rx_id) {
    devices_[rx_id] = device;
}

void CANBus::detach(uint32_t rx_id) {
    devices_.erase(rx_id);
}

} // namespace gn10_can
