#include "gn10_can/devices/esc_hub_server.hpp"

#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
ESCHubServer::ESCHubServer(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::ESCHub, device_id)
{
}

bool ESCHubServer::get_init()
{
    if (init_.has_value()) {
        init_.reset();
        return true;
    }
    return false;
}

bool ESCHubServer::get_p_gain(float& p_gain, uint8_t motor_num)
{
    if (p_gain_[motor_num].has_value()) {
        p_gain = p_gain_[motor_num].value();
        return true;
    }
    return false;
}

bool ESCHubServer::get_i_gain(float& i_gain, uint8_t motor_num)
{
    if (i_gain_[motor_num].has_value()) {
        i_gain = i_gain_[motor_num].value();
        return true;
    }
    return false;
}

bool ESCHubServer::get_d_gain(float& d_gain, uint8_t motor_num)
{
    if (d_gain_[motor_num].has_value()) {
        d_gain = d_gain_[motor_num].value();
        return true;
    }
    return false;
}

bool ESCHubServer::get_ff_coefficient(float& ff_coefficient, uint8_t motor_num)
{
    if (ff_coefficient_[motor_num].has_value()) {
        ff_coefficient = ff_coefficient_[motor_num].value();
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

void ESCHubServer::on_receive(const FDCANFrame& frame)
{
    auto id_fields = id::unpack(frame.id);

    if (id_fields.is_command(id::MsgTypeESCHub::Init)) {
    } else if (id_fields.is_command(id::MsgTypeESCHub::AngularVelocities)) {
        uint8_t init;
        if (converter::unpack(frame.data, 0, init)) {
            init_ = init;
        }
    } else if (id_fields.is_command(id::MsgTypeESCHub::PGain)) {
        float p_gain;
        uint8_t motor_num = frame.data[4];

        if (converter::unpack(frame.data, 0, p_gain)) {
            p_gain_[motor_num] = p_gain;
        }
    } else if (id_fields.is_command(id::MsgTypeESCHub::IGain)) {
        float i_gain;
        uint8_t motor_num = frame.data[4];

        if (converter::unpack(frame.data, 0, i_gain)) {
            p_gain_[motor_num] = i_gain;
        }
    } else if (id_fields.is_command(id::MsgTypeESCHub::DGain)) {
        float d_gain;
        uint8_t motor_num = frame.data[4];
        if (converter::unpack(frame.data, 0, d_gain)) {
            p_gain_[motor_num] = d_gain;
        }
    } else if (id_fields.is_command(id::MsgTypeESCHub::FF)) {
        float ff;
        uint8_t motor_num = frame.data[4];
        if (converter::unpack(frame.data, 0, ff)) {
            p_gain_[motor_num] = ff;
        }
    }
}
}  // namespace devices
}  // namespace gn10_can