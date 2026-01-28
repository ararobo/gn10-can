#include "gn10_can/devices/motor_driver.hpp"

#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {
namespace devices {

MotorDriver::MotorDriver(CANBus& bus, uint8_t id)
    : CANDevice(bus, gn10_can::id::DeviceType::MotorDriver, id) {}

void MotorDriver::send_init(const MotorConfig& config) {
    // Assuming config.to_bytes() returns an array or vector compatible with send template
    send(id::MsgTypeMotorDriver::Init, config.to_bytes());
}

void MotorDriver::send_target(float target) {
    std::array<uint8_t, 4> payload{};
    converter::pack(payload, 0, target);
    send(id::MsgTypeMotorDriver::Target, payload);
}

void MotorDriver::send_gain(devices::GainType type, float value) {
    std::array<uint8_t, 5> payload{};
    payload[0] = static_cast<uint8_t>(type);
    converter::pack(payload, 1, value);
    send(id::MsgTypeMotorDriver::Gain, payload);
}

void MotorDriver::send_feedback(float current_val, uint8_t limit_switch_state) {
    std::array<uint8_t, 5> payload{};
    converter::pack(payload, 0, current_val);
    converter::pack(payload, 4, limit_switch_state);
    send(id::MsgTypeMotorDriver::Feedback, payload);
}

void MotorDriver::send_status(float load_current, int8_t temperature) {
    std::array<uint8_t, 5> payload{};
    converter::pack(payload, 0, load_current);
    converter::pack(payload, 4, temperature);
    send(id::MsgTypeMotorDriver::Status, payload);
}

void MotorDriver::on_receive(const CANFrame& frame) {
    // Protocol Update: Command is in ID, not Data[0].
    
    auto fields = id::unpack(frame.id);
    
    if (fields.is_command(id::MsgTypeMotorDriver::Feedback)) {
        // Payload starts at 0
        converter::unpack(frame.data.data(), frame.dlc, 0, feedback_val_);
        converter::unpack(frame.data.data(), frame.dlc, 4, limit_sw_state_);
    } else if (fields.is_command(id::MsgTypeMotorDriver::Status)) {
        converter::unpack(frame.data.data(), frame.dlc, 0, load_current_);
        converter::unpack(frame.data.data(), frame.dlc, 4, temperature_);
    }
}

}  // namespace devices
}  // namespace gn10_can
