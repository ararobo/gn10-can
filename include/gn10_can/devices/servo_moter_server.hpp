#pragma once
#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/core/can_device.hpp"
#include "gn10_can/core/can_frame.hpp"

namespace gn10_can {
namespace devices {
class servo_moter_server : public CANDevice
{
    servo_moter_server(CANBus& bus)
};

}  // namespace devices
}  // namespace gn10_can