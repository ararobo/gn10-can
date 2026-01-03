#include "gn10_can/devices/motor_driver.hpp"

namespace gn10_can {
namespace devices {

MotorDriver::MotorDriver(CANManager& manager, uint8_t dev_id)
    : CANDevice(manager, id::DeviceType::MotorDriver, dev_id) {}

}  // namespace devices
}  // namespace gn10_can
