#include "gn10_can/devices/led_control_client.hpp"

#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
LEDControlClient::LEDControlClient(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::LED, device_id)
{
}

void LEDControlClient::set_init(const RGB& rgb)
{
    FDCANFrame frame = FDCANFrame::make(id::DeviceType::LED, device_id_, id::MsgTypeLED::Init);
    converter::pack(frame.data, 0, rgb);
    frame.dlc = 24;
    bus_.send_frame(frame);
}

void LEDControlClient::set_pixel_id(const LEDScopeSetting& led_setting, uint8_t led_id)
{
    FDCANFrame frame = FDCANFrame::make(id::DeviceType::LED, device_id_, id::MsgTypeLED::LEDIDSet);
    converter::pack(frame.data, 0, led_setting);
    converter::pack(frame.data, sizeof(led_setting), led_id);
    frame.dlc = 48;
    bus_.send_frame(frame);
}

void LEDControlClient::reset_pixel_id(uint8_t led_id) {}

void LEDControlClient::set_color_type_led_id(uint8_t led_id, const RGB& rgb, ShowTypeLED show_type)
{
}

void LEDControlClient::set_color_type_led_tape(
    uint8_t led_tape, const RGB& rgb, ShowTypeLED show_type
)
{
}

void LEDControlClient::set_color_type_led_all(const RGB& rgb, ShowTypeLED show_type) {}

void LEDControlClient::set_show_all() {}

void LEDControlClient::set_clear_all() {}

void LEDControlClient::on_receive(const FDCANFrame& frame) {}

}  // namespace devices
}  // namespace gn10_can
