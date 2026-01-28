#include <gtest/gtest.h>

#include "gn10_can/core/can_id.hpp"
#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/devices/motor_driver.hpp"
#include "gn10_can/utils/can_converter.hpp"
#include "mock_driver.hpp"

using namespace gn10_can;
using namespace gn10_can::devices;

class MotorDriverTest : public ::testing::Test {
  protected:
    MockDriver driver;
    CANBus bus{driver};
    // MotorDriver(Type=1) + ID=1 -> (1<<7) | (1<<3) = 128 | 8 = 136 (0x88)
    MotorDriver motor{bus, 1}; 
};

TEST_F(MotorDriverTest, SendTarget) {
    float target = 123.45f;
    motor.send_target(target);

    ASSERT_EQ(driver.sent_frames.size(), 1);
    const auto& frame = driver.sent_frames[0];

    // Verify ID matches the constructor ID mechanism
    // id = pack(MotorDriver, 1, Target(1))
    // Target=1. Pack = (1<<7) | (1<<3) | 1 = 0x80 | 0x08 | 0x01 = 0x89
    uint32_t expected_id = id::pack(id::DeviceType::MotorDriver, 1, id::MsgTypeMotorDriver::Target);
    EXPECT_EQ(frame.id, expected_id);

    // Command is now embedded in ID, so payload should start with data directly?
    // CANDevice::send logic:
    // frame.id = pack(..., cmd)
    
    // Check Payload
    // frame.data should contain just the packed float?
    // Wait, the previous impl put cmd in data[0].
    // New impl puts cmd in ID.
    // So data starts at 0.
    
    // float is 4 bytes.
    EXPECT_EQ(frame.dlc, 4);
    
    // Verify Data (Payload starts at index 0 now)
    float unpacked_target = 0.0f;
    // unpack(buffer, len, offset, value)
    // Offset is 0
    converter::unpack(frame.data.data(), frame.dlc, 0, unpacked_target);
    EXPECT_FLOAT_EQ(target, unpacked_target);
}

TEST_F(MotorDriverTest, ReceiveFeedback) {
    float feedback   = 543.21f;
    uint8_t limit_sw = 1;

    // Construct a feedback frame
    // New protocol: Command in ID
    // ID = pack(MotorDriver, 1, Feedback)
    
    CANFrame frame;
    frame.id = id::pack(id::DeviceType::MotorDriver, 1, id::MsgTypeMotorDriver::Feedback);
    
    // Payload contains data directly from index 0
    std::array<uint8_t, 8> temp_payload{};
    converter::pack(temp_payload, 0, feedback);
    converter::pack(temp_payload, 4, limit_sw);
    
    frame.dlc = 5; // float(4) + uint8_t(1)
    std::memcpy(frame.data.data(), temp_payload.data(), 5);

    // In Proposal 5, `CANBus::update` calls `motor.on_receive`.
    // OR calling `motor.on_receive` directly for unit testing logic.
    motor.on_receive(frame);
    
    EXPECT_FLOAT_EQ(motor.get_feedback_value(), feedback);
    EXPECT_EQ(motor.get_limit_switch_state(), limit_sw);
}
