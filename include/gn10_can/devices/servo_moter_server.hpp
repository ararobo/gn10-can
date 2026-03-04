#pragma once
#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/core/can_device.hpp"
#include "gn10_can/core/can_frame.hpp"

namespace gn10_can {
namespace devices {
class servo_moter_server : public CANDevice
{
public:
    servo_moter_server(CANBus& bus, uint8_t device_id);

    bool get_init(float& init);
    bool get_duty(uint16_t& duty);
    void on_receive(const CANFrame& frame) override;
};

}  // namespace devices
}  // namespace gn10_can