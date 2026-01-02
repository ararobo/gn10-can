#include "gn10_can/core/can_device.hpp"

#include "gn10_can/core/can_manager.hpp"

namespace gn10_can {

CANDevice::CANDevice(CANManager& manager, id::DeviceType device_type, uint8_t device_id)
    : manager_(manager), device_type_(device_type), device_id_(device_id) {}

bool CANDevice::send(uint8_t command, const uint8_t* data, uint8_t len) {
    auto frame = CANFrame::make(device_type_, device_id_, command, data, len);
    return manager_.send_frame(frame);
}
}  // namespace gn10_can