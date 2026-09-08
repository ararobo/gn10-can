#include "gn10_can/devices/launcher_server.hpp"

#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
LauncherServer::LauncherServer(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::Launcher, device_id)
{
}

bool LauncherServer::get_init()
{
    if (init_received_) {
        init_received_ = false;
        return true;
    }
    return false;
}

bool LauncherServer::get_fire_command(float& target_velocity)
{
    if (target_velocity_.has_value()) {
        target_velocity = target_velocity_.value();
        target_velocity_.reset();
        return true;
    }
    return false;
}

void LauncherServer::send_release_point(float release_velocity)
{
    std::array<uint8_t, sizeof(float)> data{};
    converter::pack(data, 0, release_velocity);
    send(id::MsgTypeLauncher::ReleasePoint, data);
}

void LauncherServer::send_initial_point(float initial_angle)
{
    std::array<uint8_t, sizeof(float)> data{};
    converter::pack(data, 0, initial_angle);
    send(id::MsgTypeLauncher::InitialPoint, data);
}

void LauncherServer::send_velocity_feedback(float feedback_velocity)
{
    std::array<uint8_t, sizeof(float)> data{};
    converter::pack(data, 0, feedback_velocity);
    send(id::MsgTypeLauncher::FeedbackVel, data);
}

void LauncherServer::on_receive(const FDCANFrame& frame)
{
    auto id_fields = id::unpack(frame.id);
    if (id_fields.is_command(id::MsgTypeLauncher::Init)) {
        init_received_ = true;
    } else if (id_fields.is_command(id::MsgTypeLauncher::Fire)) {
        float target_velocity;
        if (converter::unpack(frame.data, 0, target_velocity)) {
            target_velocity_ = target_velocity;
        }
    }
}
}  // namespace devices
}  // namespace gn10_can