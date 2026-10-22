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

void LauncherClient::send_fire_command(float target_erpm)
{
    std::array<uint8_t, sizeof(float)> data{};
    converter::pack(data, 0, target_erpm);
    send(id::MsgTypeLauncher::Fire, data);
}

void LauncherClient::send_unjam_command(float rotate_erpm)
{
    std::array<uint8_t, sizeof(float)> data{};
    converter::pack(data, 0, rotate_erpm);
    send(id::MsgTypeLauncher::Unjam, data);
}

bool LauncherClient::get_release_point(float& release_erpm)
{
    if (release_erpm_.has_value()) {
        release_erpm = release_erpm_.value();
        release_erpm_.reset();
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

bool LauncherClient::get_feedbacks(launcher_feedbacks::Sensors sensors)
{
    if (sensors_.has_value()) {
        sensors = sensors_.value();
        sensors_.reset();
        return true;
    }
    return false;
}

void LauncherClient::on_receive(const FDCANFrame& frame)
{
    auto id_fields = id::unpack(frame.id);
    if (id_fields.is_command(id::MsgTypeLauncher::ReleasePoint)) {
        float release_erpm;
        if (converter::unpack(frame.data, 0, release_erpm)) {
            release_erpm_ = release_erpm;
        }
    } else if (id_fields.is_command(id::MsgTypeLauncher::InitialPoint)) {
        float initial_angle;
        if (converter::unpack(frame.data, 0, initial_angle)) {
            initial_angle_ = initial_angle;
        }
    } else if (id_fields.is_command(id::MsgTypeLauncher::Feedbacks)) {
        if (frame.dlc != dlc::data_length_to_dlc(sizeof(float) * 3)) return;
        float erpm;
        float voltage;
        float current;
        if (converter::unpack(frame.data, 0, erpm) &&
            converter::unpack(frame.data, sizeof(float), voltage) &&
            converter::unpack(frame.data, sizeof(float) * 2, current)) {
            sensors_ = launcher_feedbacks::Sensors{erpm, voltage, current};
        }
    }
}

}  // namespace devices
}  // namespace gn10_can