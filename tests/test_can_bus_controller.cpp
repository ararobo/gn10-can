#include <gtest/gtest.h>

#include "gn10_can/core/can_device.hpp"
#include "gn10_can/core/can_bus_controller.hpp"
#include "mock_driver.hpp"

using namespace gn10_can;

class MockDevice : public CANDevice {
  public:
    MockDevice(ICANSender& sender, id::DeviceType type, uint8_t id)
        : CANDevice(sender, type, id) {}

    void on_receive(const CANFrame& frame) override { received_frames.push_back(frame); }

    std::vector<CANFrame> received_frames;
};

class CANBusControllerTest : public ::testing::Test {
  protected:
    MockDriver driver;
    CANBusController controller{driver};
};

TEST_F(CANBusControllerTest, RegisterReceiver) {
    MockDevice device1(controller, id::DeviceType::MotorDriver, 1);
    controller.register_receiver(&device1);
}

TEST_F(CANBusControllerTest, UpdateReceivesFrames) {
    MockDevice device(controller, id::DeviceType::MotorDriver, 1);
    controller.register_receiver(&device);

    // Frame must match device ID (Type=MotorDriver(1), ID=1)
    CANFrame frame;
    // Construct exact ID matching device type and ID
    frame.id = (static_cast<uint32_t>(id::DeviceType::MotorDriver) << 7) | (1 << 3); 
    
    driver.push_receive_frame(frame);

    controller.update();

    ASSERT_EQ(device.received_frames.size(), 1);
    EXPECT_EQ(device.received_frames[0].id, frame.id);
}

TEST_F(CANBusControllerTest, SendFrame) {
    CANFrame frame;
    frame.id = 0x456;

    EXPECT_TRUE(controller.send_frame(frame));

    ASSERT_EQ(driver.sent_frames.size(), 1);
    EXPECT_EQ(driver.sent_frames[0].id, 0x456);
}
