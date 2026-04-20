#include "gn10_can/devices/esc_hub_server.hpp"

#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
ESCHubServer::ESCHubServer(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::ESCHub, device_id)
{
}

bool ESCHubServer::get_init() {}

bool ESCHubServer::get_p_gain(float p_gain, uint8_t motor_num) {}

bool ESCHubServer::get_i_gain(float i_gain, uint8_t motor_num) {}

bool ESCHubServer::get_d_gain(float d_gain, uint8_t motor_num) {}

bool ESCHubServer::get_ff_coefficient(float ff_coefficient, uint8_t motor_num) {}

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
        AngularVelocities config;
        if (converter::unpack(frame.data, 0, config)) {
            angular_velocity_ = config;
        }
    }
}
}  // namespace devices
}  // namespace gn10_can