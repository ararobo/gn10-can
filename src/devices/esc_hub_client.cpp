#include "gn10_can/devices/esc_hub_client.hpp"

#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
ESCHubClient::ESCHubClient(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::ESCHub, device_id)
{
}

bool ESCHubClient::get_encoder_feedbacks(int32_t& encoder_value)
{
    if (encoder_value_.has_value()) {
        encoder_value = encoder_value_.value();
        encoder_value_.reset();
        return true;
    }
    return false;
}

void ESCHubClient::set_vesc_command(bool vesc_moving)
{
    FDCANFrame frame =
        FDCANFrame::make(id::DeviceType::ESCHub, device_id_, id::MsgTypeESCHub::Command);
    converter::pack(frame.data, 0, vesc_moving);
    frame.dlc = sizeof(bool);
    bus_.send_frame(frame);
}

void ESCHubClient::on_receive(const FDCANFrame& frame)
{
    auto id_fields = id::unpack(frame.id);
    if (id_fields.is_command(id::MsgTypeESCHub::Encoder)) {
        int32_t config;
        if (converter::unpack(frame.data.data(), frame.dlc, 0, config)) {
            encoder_value_ = config;
        }
    }
}
}  // namespace devices
}  // namespace gn10_can