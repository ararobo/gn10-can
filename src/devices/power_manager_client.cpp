#include "gn10_can/devices/power_manager_client.hpp"

#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {

namespace devices {
PowerManagerClient::PowerManagerClient(FDCANBus& bus, uint8_t dev_id)
    : FDCANDevice(bus, id::DeviceType::PowerManager, dev_id)
{
}

void PowerManagerClient::set_init(power_manager::Config config)
{
    std::array<uint8_t, 3> payload{};
    converter::pack(payload, 0, config.use_remote_emergency_stop);
    converter::pack(payload, 1, config.sensor_rate_ms);
    send(id::MsgTypePowerManager::Init, payload);
}

void PowerManagerClient::set_stop(bool enable_stop)
{
    std::array<uint8_t, 1> payload{};
    converter::pack(payload, 0, enable_stop);
    send(id::MsgTypePowerManager::Stop, payload);
}

bool PowerManagerClient::get_new_status(power_manager::Status& status)
{
    if (status_.has_value()) {
        status = status_.value();
        status_.reset();
        return true;
    }
    return false;
}

bool PowerManagerClient::get_new_sensor(power_manager::Sensor& sensor)
{
    if (sensor_.has_value()) {
        sensor = sensor_.value();
        sensor_.reset();
        return true;
    }
    return false;
}

bool PowerManagerClient::get_new_voltages(std::array<float, 4>& voltages)
{
    if (voltages_.has_value()) {
        voltages = voltages_.value();
        voltages_.reset();
        return true;
    }
    return false;
}

void PowerManagerClient::on_receive(const FDCANFrame& frame)
{
    auto id_fields = id::unpack(frame.id);
    if (id_fields.is_command(id::MsgTypePowerManager::Status)) {
        if (frame.dlc != dlc::data_length_to_dlc(sizeof(bool) * 4)) return;
        bool emergency_stop_enabled;
        bool remote_emergency_stop_connected;
        bool remote_emergency_stop_enabled;
        bool over_current;
        if (converter::unpack(frame.data, 0, emergency_stop_enabled) &&
            converter::unpack(frame.data, 1, remote_emergency_stop_connected) &&
            converter::unpack(frame.data, 2, remote_emergency_stop_enabled) &&
            converter::unpack(frame.data, 3, over_current)) {
            status_ = power_manager::Status{
                emergency_stop_enabled,
                remote_emergency_stop_connected,
                remote_emergency_stop_enabled,
                over_current
            };
        }
    } else if (id_fields.is_command(id::MsgTypePowerManager::Sensor)) {
        if (frame.dlc != dlc::data_length_to_dlc(sizeof(float) * 2)) return;
        float voltage;
        float current;
        if (converter::unpack(frame.data, 0, voltage) &&
            converter::unpack(frame.data, 4, current)) {
            sensor_ = power_manager::Sensor{voltage, current};
        }
    } else if (id_fields.is_command(id::MsgTypePowerManager::Voltages)) {
        if (frame.dlc != dlc::data_length_to_dlc(sizeof(std::array<float, 4>))) return;
        std::array<float, 4> voltages{};
        if (converter::unpack(frame.data, 0, voltages)) {
            voltages_ = voltages;
        }
    }
}

}  // namespace devices
}  // namespace gn10_can