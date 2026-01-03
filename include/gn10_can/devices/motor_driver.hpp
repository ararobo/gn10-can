#pragma once

#include "gn10_can/core/can_device.hpp"
#include "gn10_can/devices/motor_driver_types.hpp"

namespace gn10_can {
namespace devices {

class MotorDriver : public CANDevice {
  public:
    MotorDriver(CANManager& manager, uint8_t dev_id);

    void send_init(const MotorConfig& config);

    void send_target(float target);

  private:
};
}  // namespace devices
}  // namespace gn10_can