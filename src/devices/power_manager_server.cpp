#include "gn10_can/devices/power_manager_server.hpp"

#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {
namespace devices {

PowerManagerServer::PowerManagerServer(FDCANBus& bus, uint8_t dev_id)
    : FDCANDevice(bus, id::DeviceType::PowerManager, dev_id)
{
}

bool PowerManagerServer::get_new_init(uint16_t& sensor_rate_ms)
{
    if (init_.has_value()) {
        sensor_rate_ms = init_.value();
        init_.reset();
        return true;
    }
    return false;
}

bool PowerManagerServer::get_new_stop(bool& enable_stop)
{
    if (enable_stop_.has_value()) {
        enable_stop = enable_stop_.value();
        enable_stop_.reset();
        return true;
    }
    return false;
}

void PowerManagerServer::set_status(Status status)
{
    std::array<uint8_t, 4> payload{};
    converter::pack(payload, 0, status.emergency_stop_enabled);
    converter::pack(payload, 1, status.remote_emergency_stop_connected);
    converter::pack(payload, 2, status.remote_emergency_stop_enabled);
    converter::pack(payload, 3, status.over_current);
    send(id::MsgTypePowerManager::Status, payload);
}

void PowerManagerServer::set_sensor(Sensor sensor)
{
    std::array<uint8_t, 8> payload{};
    converter::pack(payload, 0, sensor.voltage);
    converter::pack(payload, 4, sensor.current);
    send(id::MsgTypePowerManager::Sensor, payload);
}

void PowerManagerServer::on_receive(const FDCANFrame& frame)
{
    auto id_fields = id::unpack(frame.id);
    if (id_fields.is_command(id::MsgTypePowerManager::Init)) {
        uint16_t init;
        if (converter::unpack(frame.data.data(), frame.dlc, 0, init)) {
            init_.value() = init;
        }
    }
    if (id_fields.is_command(id::MsgTypePowerManager::Stop)) {
        bool enable_stop;
        if (converter::unpack(frame.data.data(), frame.dlc, 0, enable_stop)) {
            enable_stop_.value() = enable_stop;
        }
    }
}

}  // namespace devices
}  // namespace gn10_can