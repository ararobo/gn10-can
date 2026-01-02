#pragma once

#include <cstdint>

#include "gn10_can/core/can_frame.hpp"

namespace gn10_can {

class CANManager;

class CANDevice {
 public:
    CANDevice(CANManager& manager, id::DeviceType device_type, uint8_t device_id);
    virtual ~CANDevice() = default;

    virtual void on_receive(const CANFrame& frame) = 0;

    virtual void update() {}

    id::DeviceType get_device_type() const { return device_type_; }

    uint8_t get_device_id() const { return device_id_; }

 protected:
    bool send(uint8_t command, const uint8_t* data, uint8_t len);

    CANManager& manager_;
    id::DeviceType device_type_;
    uint8_t device_id_;
};

}  // namespace gn10_can