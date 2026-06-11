#include "gn10_can/devices/esc_hub_server.hpp"

#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
ESCHubServer::ESCHubServer(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::ESCHub, device_id)
{
}

bool ESCHubServer::get_vesc_command(bool& vesc_moving)
{
    if (vesc_command_.has_value()) {
        vesc_moving = vesc_command_.value();
        vesc_command_.reset();
        return true;
    }
    return false;
}

void ESCHubServer::set_encoder_feedbacks(int32_t encoder_value)
{
    FDCANFrame frame =
        FDCANFrame::make(id::DeviceType::ESCHub, device_id_, id::MsgTypeESCHub::Encoder);
    converter::pack(frame.data, 0, encoder_value);
    frame.dlc = sizeof(int32_t);
    bus_.send_frame(frame);
}

void ESCHubServer::on_receive(const FDCANFrame& frame)
{
    auto id_fields = id::unpack(frame.id);

    if (id_fields.is_command(id::MsgTypeESCHub::Command)) {
        bool config;
        if (converter::unpack(frame.data.data(), frame.dlc, 0, config)) {
            vesc_command_ = config;
        }
    }
}
}  // namespace devices
}  // namespace gn10_can