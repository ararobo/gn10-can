#include "gn10_can/devices/launcher_client.hpp"

#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
LauncherClient::LauncherClient(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::Launcher, device_id)
{
}

void LauncherClient::set_init()
{
    send(id::MsgTypeLauncher::Init);
}

void LauncherClient::send_fire_command(float target_velocity)
{
    std::array<uint8_t, sizeof(float)> data{};
    converter::pack(data, 0, target_velocity);
    send(id::MsgTypeLauncher::Fire, data);
}

bool LauncherClient::get_release_point(float& release_speed)
{
    if (release_speed_.has_value()) {
        release_speed = release_speed_.value();
        release_speed_.reset();
        return true;
    }
    return false;
}

bool LauncherClient::get_initial_point(float& initial_angle)
{
    if (initial_angle_.has_value()) {
        initial_angle = initial_angle_.value();
        initial_angle_.reset();
        return true;
    }
    return false;
}

bool LauncherClient::get_velocity_feedback(float& feedback_velocity)
{
    if (feedback_velocity_.has_value()) {
        feedback_velocity = feedback_velocity_.value();
        feedback_velocity_.reset();
        return true;
    }
    return false;
}

void LauncherClient::on_receive(const FDCANFrame& frame)
{
    auto id_fields = id::unpack(frame.id);
    if (id_fields.is_command(id::MsgTypeLauncher::ReleasePoint)) {
        float release_speed;
        if (converter::unpack(frame.data, 0, release_speed)) {
            release_speed_ = release_speed;
        }
    } else if (id_fields.is_command(id::MsgTypeLauncher::InitialPoint)) {
        float initial_angle;
        if (converter::unpack(frame.data, 0, initial_angle)) {
            initial_angle_ = initial_angle;
        }
    } else if (id_fields.is_command(id::MsgTypeLauncher::FeedbackVel)) {
        float feedback_velocity;
        if (converter::unpack(frame.data, 0, feedback_velocity)) {
            feedback_velocity_ = feedback_velocity;
        }
    }
}

}  // namespace devices
}  // namespace gn10_can