#pragma once

#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/core/can_device.hpp"
#include "gn10_can/core/can_frame.hpp"

namespace gn10_can {
namespace devices {
class ServoMotorClient : public CANDevice
{
public:
    ServoMotorClient(CANBus& bus, uint8_t device_id);

    void set_init(uint16_t min_us, uint16_t max_us);

    void set_angle_rad(float angle_rad);

    /**
     * @brief CANパケット受信時の呼び出し関数の実装
     *
     * @param frame 受信したCANパケット
     */
    void on_receive(const CANFrame& frame) override;
};
}  // namespace devices
}  // namespace gn10_can