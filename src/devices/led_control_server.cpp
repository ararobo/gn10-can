#include "gn10_can/devices/led_control_server.hpp"

#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
LEDControlServer::LEDControlServer(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::LED, device_id)
{
}

bool LEDControlServer::get_new_init(uint16_t& led_num_max, uint16_t& led_num_sum)
{
    if (led_num_max_.has_value() && led_num_max_.has_value()) {
        led_num_max = led_num_max_.value();
    }
}

bool LEDControlServer::get_new_led_id(
    uint8_t& led_id, uint16_t& led_num_start, uint16_t& led_num_end
)
{
}

bool LEDControlServer::get_new_release_led_id(uint8_t& led_id) {}

bool LEDControlServer::get_new_led_color(uint8_t& led_id, RGB& pixel_rgb) {}

bool LEDControlServer::get_new_flash(uint8_t& led_id, FlashTypeLED flashtype) {}

bool LEDControlServer::get_new_dark(uint8_t& led_id) {}

}  // namespace devices
}  // namespace gn10_can
