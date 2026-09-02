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
    std::array<uint8_t, sizeof(MotorConfig) + 1> data;
    converter::pack(data, 0, motor_id);
    converter::pack(data, 1, config);
    send(id::MsgTypeESCHub::Init, data);
}

void ESCHubClient::set_gains(const uint8_t motor_id, float kp, float ki, float kd, float ff)
{
    if (motor_id > 3) return;
    std::array<uint8_t, sizeof(float) * 4 + 1> data;
    converter::pack(data, 0, motor_id);
    converter::pack(data, 1, kp);
    converter::pack(data, 1 + sizeof(float) * 1, ki);
    converter::pack(data, 1 + sizeof(float) * 2, kd);
    converter::pack(data, 1 + sizeof(float) * 3, ff);
    send(id::MsgTypeESCHub::Gains, data);
}

void ESCHubClient::set_targets(const float targets[4])
{
    std::array<uint8_t, sizeof(float) * 4> data;
    for (int i = 0; i < 4; i++) {
        converter::pack(data, i * sizeof(float), targets[i]);
    }
    send(id::MsgTypeESCHub::Targets, data);
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
        if (frame.dlc != dlc::data_length_to_dlc(sizeof(Feedbacks))) return;
        Feedbacks feedbacks;
        if (converter::unpack(frame.data, 0, feedbacks)) {
            feedbacks_ = feedbacks;
        }
    }
}
}  // namespace devices
}  // namespace gn10_can