#include "gn10_can/devices/led_control_server.hpp"

#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
LEDControlServer::LEDControlServer(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::LED, device_id)
{
}
bool LEDControlServer::get_new_init(RGB& rgb) {}
bool LEDControlServer::get_pixel_id(uint8_t led_tape, uint16_t led_num_min, uint16_t led_num_max) {}
bool LEDControlServer::reset_reset_id(uint16_t led_id) {}
bool LEDControlServer::get_color_type_led_idset_color_type_led_id(
    uint8_t led_id, RGB rgb, ShowTypeLED show_type
)
{
}
bool LEDControlServer::get_color_type_led_tape(uint8_t led_tape, RGB rgb, ShowTypeLED show_type) {}
bool LEDControlServer::get_color_type_led_all(RGB rgb, ShowTypeLED show_type) {}
bool LEDControlServer::get_show_pixels(uint16_t led_id) {}
bool LEDControlServer::get_show_tape(uint8_t led_tape_num) {}
bool LEDControlServer::get_show_all() {}
bool LEDControlServer::get_clear_pixels(uint16_t led_id) {}
void LEDControlServer::get_clear_tape(uint8_t led_tape_num) {}
void LEDControlServer::get_clear_all() {}

}  // namespace devices
}  // namespace gn10_can
