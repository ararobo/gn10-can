#include "gn10_can/devices/servo_motor_server.hpp"

#include "array"
#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {
namespace devices {
ServoMotorServer::ServoMotorServer(CANBus& bus, uint8_t device_id)
    : CANDevice(bus, id::DeviceType::ServoMotor, device_id)
{
}

bool ServoMotorServer::get_new_init(uint16_t& min_us, uint16_t& max_us)
{
    if (pulse_set_.has_value()) {
        min_us = pulse_set_->min_us;
        max_us = pulse_set_->max_us;
        pulse_set_.reset();
        return true;
    }
    return false;
}
bool ServoMotorServer::get_new_angle_rad(std::array<float, 2>& angles_rad)
{
    if (angles_rad_.has_value()) {
        angles_rad = angles_rad_.value();
        angles_rad_.reset();
        return true;
    }
    return false;
}
void ServoMotorServer::on_receive(const CANFrame& frame)
{
    auto id_fields = id::unpack(frame.id);

    if (id_fields.is_command(id::MsgTypeServoMotor::Init)) {
        uint16_t min_us = 0;
        uint16_t max_us = 0;
        if (converter::unpack(frame.data.data(), frame.dlc, 0, min_us) &&
            converter::unpack(frame.data.data(), frame.dlc, 2, max_us)) {
            pulse_set_ = PulseSet{min_us, max_us};
        }
    } else if (id_fields.is_command(id::MsgTypeServoMotor::AngleRad)) {
        float angle1 = 0.0f;
        float angle2 = 0.0f;
        if (converter::unpack(frame.data.data(), frame.dlc, 0, angle1) &&
            converter::unpack(frame.data.data(), frame.dlc, 4, angle2)) {
            angles_rad_ = std::array<float, 2>{angle1, angle2};
        }
    }
}
}  // namespace devices
}  // namespace gn10_can