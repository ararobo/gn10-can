#pragma once

#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/core/can_device.hpp"
#include "gn10_can/core/can_frame.hpp"

namespace gn10_can {
namespace devices {
class servo_moter_client : public CANDevice
{
public:
    servo_moter_client(CANBus& bus, uint16_t device_id);

    void set_init(float servo_init);

    void duty_adjust(uint16_t duty_raio);
};
}  // namespace devices
}  // namespace gn10_can