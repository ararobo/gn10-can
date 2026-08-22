#include "gn10_can/devices/led_control_server.hpp"

#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
LEDControlServer::LEDControlServer(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::LED, device_id)
{
}

}  // namespace devices
}  // namespace gn10_can
