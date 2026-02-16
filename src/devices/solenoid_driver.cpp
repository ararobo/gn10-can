#include "solenoid_driver.hpp"

#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {
namespace devices {

SolenoidDriver::SolenoidDriver(CANBus& bus, uint8_t dev_id)
    : CANDevice(bus, id::DeviceType::SolenoidDriver, dev_id) {}

void SolenoidDriver::send_init() {
    // send(id::MsgTypeSolenoidDriver::Init,);
}

void SolenoidDriver::send_target(bool target) {
    std::array<uint8_t, 1> payload{};
    converter::pack(payload, 0, target);
    send(id::MsgTypeSolenoidDriver::Target, payload);
}

}  // namespace devices
}  // namespace gn10_can