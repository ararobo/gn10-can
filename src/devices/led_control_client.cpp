#include "gn10_can/devices/led_control_client.hpp"

#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
LEDControlClient::LEDControlClient(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::LED, device_id)
{
}

void LEDControlClient::set_init(RGB rgb) {}

uint16_t LEDControlClient::set_pixel_id(
    uint8_t led_tape, uint16_t led_num_min, uint16_t led_num_max
)
{
}

void LEDControlClient::reset_pixel_id(uint16_t led_id) {}

void LEDControlClient::set_color_type_led_id(uint8_t led_id, RGB rgb, ShowTypeLED show_type) {}

void LEDControlClient::set_color_type_led_tape(uint8_t led_tape, RGB rgb, ShowTypeLED show_type) {}

void LEDControlClient::set_color_type_led_all(RGB rgb, ShowTypeLED show_type) {}

void LEDControlClient::set_show_all() {}

void LEDControlClient::set_clear_all() {}

void LEDControlClient::on_receive(const FDCANFrame& frame) {}

}  // namespace devices
}  // namespace gn10_can
