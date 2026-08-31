#include "gn10_can/devices/led_control_server.hpp"

#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
LEDControlServer::LEDControlServer(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::LED, device_id)
{
}
bool LEDControlServer::get_new_init(RGB& rgb) {}
bool LEDControlServer::get_pixel_id(const LEDScopeSetting& led_setting, uint8_t led_id) {}
bool LEDControlServer::reset_pixels_id(uint8_t led_id) {}
bool LEDControlServer::get_color_type_led_id(uint8_t led_id, const RGB& rgb, ShowTypeLED show_type)
{
}
bool LEDControlServer::get_color_type_led_tape(
    uint8_t led_tape, const RGB& rgb, ShowTypeLED show_type
)
{
}
bool LEDControlServer::get_color_type_led_all(const RGB& rgb, ShowTypeLED show_type) {}
bool LEDControlServer::get_show_all() {}
bool LEDControlServer::get_clear_all() {}

void LEDControlServer::on_receive(const FDCANFrame& frame) {}

}  // namespace devices
}  // namespace gn10_can
