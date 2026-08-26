#include "gn10_can/devices/esc_hub_client.hpp"

#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
ESCHubClient::ESCHubClient(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::ESCHub, device_id)
{
}

void ESCHubClient::set_init(const uint8_t motor_id, const MotorConfig& config)
{
    if (motor_id > 3) return;
    FDCANFrame frame =
        FDCANFrame::make(id::DeviceType::ESCHub, device_id_, id::MsgTypeESCHub::Init);
    converter::pack(frame.data, 0, motor_id);
    converter::pack(frame.data, 1, config);
    frame.dlc = 16;
    bus_.send_frame(frame);
}

void ESCHubClient::set_gains(const uint8_t motor_id, float kp, float ki, float kd, float ff)
{
    if (motor_id > 3) return;
    FDCANFrame frame =
        FDCANFrame::make(id::DeviceType::ESCHub, device_id_, id::MsgTypeESCHub::Gain);
    converter::pack(frame.data, 0, motor_id);
    converter::pack(frame.data, 1, kp);
    converter::pack(frame.data, 1 + sizeof(float) * 1, ki);
    converter::pack(frame.data, 1 + sizeof(float) * 2, kd);
    converter::pack(frame.data, 1 + sizeof(float) * 3, ff);
    frame.dlc = 32;
    bus_.send_frame(frame);
}

void ESCHubClient::set_targets(const float targets[4])
{
    FDCANFrame frame =
        FDCANFrame::make(id::DeviceType::ESCHub, device_id_, id::MsgTypeESCHub::Targets);
    for (int i = 0; i < 4; i++) {
        converter::pack(frame.data, i * sizeof(float), targets[i]);
    }
    frame.dlc = sizeof(float) * 4;
    bus_.send_frame(frame);
}

bool ESCHubClient::get_feedbacks(float feedbacks[4])
{
    if (feedbacks_.has_value()) {
        for (int i = 0; i < 4; i++) {
            feedbacks[i] = feedbacks_->feedback[i];
        }
        feedbacks_.reset();
        return true;
    }
    return false;
}

void ESCHubClient::on_receive(const FDCANFrame& frame)
{
    auto id_fields = id::unpack(frame.id);
    if (id_fields.is_command(id::MsgTypeESCHub::Feedbacks)) {
        if (frame.dlc < sizeof(Feedbacks)) return;
        Feedbacks feedbacks;
        if (converter::unpack(frame.data.data(), frame.dlc, 0, feedbacks)) {
            feedbacks_ = feedbacks;
        }
    }
}
}  // namespace devices
}  // namespace gn10_can