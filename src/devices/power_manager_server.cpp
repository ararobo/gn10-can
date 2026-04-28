#include "gn10_can/devices/power_manager_server.hpp"

#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {
namespace devices {

PowerManagerServer::PowerManagerServer(FDCANBus& bus, uint8_t dev_id)
    : FDCANDevice(bus, id::DeviceType::PowerManager, dev_id)
{
}

bool PowerManagerServer::get_init()
{
    if (init_.has_value()) {
        init_.reset();
        return true;
    }
    return false;
}

bool PowerManagerServer::get_stop(bool& enable_stop)
{
    if (enable_stop_.has_value()) {
        enable_stop = enable_stop_.value();
        enable_stop_.reset();
        return true;
    }
    return false;
}

void PowerManagerServer::set_feedback(Status feedback)
{
    std::array<uint8_t, sizeof(Status)> payload{};
    converter::pack(payload, 0, feedback);
    send(id::MsgTypePowerManager::Status, payload);
}

void PowerManagerServer::on_receive(const FDCANFrame& frame)
{
    auto id_fields = id::unpack(frame.id);
    if (id_fields.is_command(id::MsgTypePowerManager::Init)) {
        uint8_t init;
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