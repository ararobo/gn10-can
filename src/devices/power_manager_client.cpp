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

bool PowerManagerClient::get_status(Status& feedback)
{
    if (feedback_.has_value()) {
        feedback = feedback_.value();
        feedback_.reset();
        return true;
    }
    return false;
}

void PowerManagerClient::on_receive(const FDCANFrame& frame)
{
    auto id_fields = id::unpack(frame.id);
    if (id_fields.is_command(id::MsgTypePowerManager::Status)) {
        bool enable_stop;
        float voltage, current;
        if (converter::unpack(frame.data.data(), frame.dlc, 0, enable_stop) &&
            converter::unpack(frame.data.data(), frame.dlc, 1, voltage) &&
            converter::unpack(frame.data.data(), frame.dlc, 5, current)) {
            feedback_.value().enable_stop = enable_stop;
            feedback_.value().voltage     = voltage;
            feedback_.value().current     = current;
        }
    }
}

}  // namespace devices
}  // namespace gn10_can