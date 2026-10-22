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

bool LauncherServer::get_fire_command(float& target_erpm)
{
    if (target_erpm_.has_value()) {
        target_erpm = target_erpm_.value();
        target_erpm_.reset();
        return true;
    }
    return false;
}

bool LauncherServer::get_unjam_command(float& rotate_erpm)
{
    if (rotate_erpm_.has_value()) {
        rotate_erpm = rotate_erpm_.value();
        rotate_erpm_.reset();
        return true;
    }
    return false;
}

void LauncherServer::send_release_point(float release_erpm)
{
    std::array<uint8_t, sizeof(float)> data{};
    converter::pack(data, 0, release_erpm);
    send(id::MsgTypeLauncher::ReleasePoint, data);
}

void LauncherServer::send_initial_point(float initial_angle)
{
    std::array<uint8_t, sizeof(float)> data{};
    converter::pack(data, 0, initial_angle);
    send(id::MsgTypeLauncher::InitialPoint, data);
}

void LauncherServer::send_feedbacks(float erpm, float voltage, float current)
{
    std::array<uint8_t, sizeof(float) * 3> data{};
    converter::pack(data, 0, erpm);
    converter::pack(data, sizeof(float), voltage);
    converter::pack(data, sizeof(float) * 2, current);
    send(id::MsgTypeLauncher::Feedbacks, data);
}

void LauncherServer::on_receive(const FDCANFrame& frame)
{
    auto id_fields = id::unpack(frame.id);
    if (id_fields.is_command(id::MsgTypeLauncher::Init)) {
        init_received_ = true;
    } else if (id_fields.is_command(id::MsgTypeLauncher::Fire)) {
        float target_erpm;
        if (converter::unpack(frame.data, 0, target_erpm)) {
            target_erpm_ = target_erpm;
        }
    } else if (id_fields.is_command(id::MsgTypeLauncher::Unjam)) {
        float rotate_erpm;
        if (converter::unpack(frame.data, 0, rotate_erpm)) {
            rotate_erpm_ = rotate_erpm;
        }
    }
}
}  // namespace devices
}  // namespace gn10_can