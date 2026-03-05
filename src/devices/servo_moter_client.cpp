#include "gn10_can/devices/servo_moter_client.hpp"

namespace gn10_can {
namespace devices {

servo_moter_client::servo_moter_client(CANBus& bus, uint8_t dev_id)
    : CANDevice(bus, id::DeviceType::Servomoter, dev_id)
{
}

}  // namespace devices
}  // namespace gn10_can