#include "gn10_can/devices/servo_motor_server.hpp"
#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {
namespace devices {
ServoMotorServer::ServoMotorServer(CANBus& bus, uint8_t device_id)
    : CANDevice(bus, id::DeviceType::ServoMotor, device_id)
{
}

bool ServoMotorServer::get_new_init(uint16_t& min_us, uint16_t& max_us) {}

bool ServoMotorServer::get_new_angle_rad(float& angle_rad) {}
}  // namespace devices
}  // namespace gn10_can