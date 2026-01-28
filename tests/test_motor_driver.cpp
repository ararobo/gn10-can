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
    // New protocol: ID=0x200, Data[0]=Cmd, Data[1..]=Payload
    CANFrame frame;
    frame.id = 0x200;
    
    // Payload length = 5 (float + uint8_t in pack)
    // Total dlc = 1 (cmd) + 5 = 6
    frame.dlc = 6;
    frame.data[0] = static_cast<uint8_t>(id::MsgTypeMotorDriver::Feedback);
    
    // Pack payload into data[1..]
    // Note: pack takes standard array or buffer. 
    // We can pack into a temp buffer then copy, or pack directly if pack supports offset pointers efficiently.
    // The utility `converter::pack` takes `std::array` or similar.
    std::array<uint8_t, 8> temp_payload{};
    converter::pack(temp_payload, 0, feedback);
    converter::pack(temp_payload, 4, limit_sw);
    
    std::memcpy(&frame.data[1], temp_payload.data(), 5);

    // In Proposal 5, `CANBus::update` calls `motor.on_receive`.
    // OR calling `motor.on_receive` directly for unit testing logic.
    motor.on_receive(frame);
    
    EXPECT_FLOAT_EQ(motor.get_feedback_value(), feedback);
    EXPECT_EQ(motor.get_limit_switch_state(), limit_sw);
}
