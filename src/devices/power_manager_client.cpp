#include "gn10_can/devices/power_manager_client.hpp"

#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {

namespace devices {
PowerManagerClient::PowerManagerClient(FDCANBus& bus, uint8_t dev_id)
    : FDCANDevice(bus, id::DeviceType::PowerManager, dev_id)
{
}

void PowerManagerClient::set_init()
{
    std::array<uint8_t, 1> payload{};
    send(id::MsgTypePowerManager::Init, payload);
}

void PowerManagerClient::set_stop(bool enable_stop)
{
    std::array<uint8_t, 1> payload{};
    converter::pack(payload, 0, enable_stop);
    send(id::MsgTypePowerManager::Stop, payload);
}

bool PowerManagerClient::get_status(Status& status)
{
    if (status_.has_value()) {
        status = status_.value();
        status_.reset();
        return true;
    }
    return false;
}

bool PowerManagerClient::get_sensor(Sensor& sensor)
{
    if (sensor_.has_value()) {
        sensor = sensor_.value();
        sensor_.reset();
        return true;
    }
    return false;
}

void PowerManagerClient::on_receive(const FDCANFrame& frame)
{
    auto id_fields = id::unpack(frame.id);
    if (id_fields.is_command(id::MsgTypePowerManager::Status)) {
        bool emergency_stop_enabled;
        bool remote_emergency_stop_connected;
        bool remote_emergency_stop_enabled;
        bool over_current;
        if (converter::unpack(frame.data.data(), frame.dlc, 0, emergency_stop_enabled) &&
            converter::unpack(frame.data.data(), frame.dlc, 1, remote_emergency_stop_connected) &&
            converter::unpack(frame.data.data(), frame.dlc, 5, remote_emergency_stop_enabled) &&
            converter::unpack(frame.data.data(), frame.dlc, 3, over_current)) {
            status_.value().emergency_stop_enabled          = emergency_stop_enabled;
            status_.value().remote_emergency_stop_connected = remote_emergency_stop_connected;
            status_.value().remote_emergency_stop_enabled   = remote_emergency_stop_enabled;
            status_.value().over_current                    = over_current;
        }
    } else if (id_fields.is_command(id::MsgTypePowerManager::Sensor)) {
        float voltage;
        float current;
        if (converter::unpack(frame.data.data(), frame.dlc, 0, voltage) &&
            converter::unpack(frame.data.data(), frame.dlc, 4, current)) {
            sensor_.value().voltage = voltage;
            sensor_.value().current = current;
        }
    }
}

}  // namespace devices
}  // namespace gn10_can