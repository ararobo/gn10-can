#include "gn10_can/devices/esc_hub_server.hpp"

#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
ESCHubServer::ESCHubServer(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::ESCHub, device_id)
{
}

bool ESCHubServer::get_init(const uint8_t motor_id, MotorConfig& config)
{
    if (motor_id > 3) return false;
    if (config_[motor_id].has_value()) {
        config = config_[motor_id].value();
        config_[motor_id].reset();
        return true;
    }
    return false;
}

bool ESCHubServer::get_gains(const uint8_t motor_id, float& kp, float& ki, float& kd, float& ff)
{
    if (motor_id > 3) return false;
    if (gains_[motor_id].has_value()) {
        Gains gains = gains_[motor_id].value();
        kp          = gains.kp;
        ki          = gains.ki;
        kd          = gains.kd;
        ff          = gains.ff;
        gains_[motor_id].reset();
        return true;
    }
    return false;
}

bool ESCHubServer::get_angular_velocities(float angular_velocities[4])
{
    if (angular_velocity_.has_value()) {
        for (int i = 0; i < 4; i++) {
            angular_velocities[i] = angular_velocity_->angular_velocity[i];
        }
        angular_velocity_.reset();
        return true;
    }
    return false;
}

void ESCHubServer::set_angular_velocity_feedbacks(float angular_velocity_feedbacks[4])
{
    FDCANFrame frame = FDCANFrame::make(
        id::DeviceType::ESCHub, device_id_, id::MsgTypeESCHub::AngularVelocitiesFeedbacks
    );
    for (int i = 0; i < 4; i++) {
        converter::pack(frame.data, i * sizeof(float), angular_velocity_feedbacks[i]);
    }
    frame.dlc = sizeof(float) * 4;
    bus_.send_frame(frame);
}

void ESCHubServer::on_receive(const FDCANFrame& frame)
{
    auto id_fields = id::unpack(frame.id);

    if (id_fields.is_command(id::MsgTypeESCHub::Init)) {
        if (frame.dlc < 1 + sizeof(MotorConfig)) return;
        MotorConfig config;
        uint8_t motor_id;
        bool success_unpack = true;
        success_unpack &= converter::unpack(frame.data, 0, motor_id);
        success_unpack &= converter::unpack(frame.data, 1, config);
        if (motor_id > 3 || !success_unpack) return;
        config_[motor_id] = config;

    } else if (id_fields.is_command(id::MsgTypeESCHub::Gain)) {
        if (frame.dlc < 1 + sizeof(float) * 4) return;
        Gains gains;
        uint8_t motor_id;
        bool success_unpack = true;
        success_unpack &= converter::unpack(frame.data, 0, motor_id);
        success_unpack &= converter::unpack(frame.data, 1, gains.kp);
        success_unpack &= converter::unpack(frame.data, 1 + sizeof(float) * 1, gains.ki);
        success_unpack &= converter::unpack(frame.data, 1 + sizeof(float) * 2, gains.kd);
        success_unpack &= converter::unpack(frame.data, 1 + sizeof(float) * 3, gains.ff);
        if (motor_id > 3 || !success_unpack) return;
        gains_[motor_id] = gains;

    } else if (id_fields.is_command(id::MsgTypeESCHub::AngularVelocities)) {
        if (frame.dlc < sizeof(AngularVelocities)) return;
        AngularVelocities config;
        if (converter::unpack(frame.data, 0, config)) {
            angular_velocity_ = config;
        }
    }
}
}  // namespace devices
}  // namespace gn10_can