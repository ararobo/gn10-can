#include "gn10_can/devices/led_control_client.hpp"

#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
LEDControlClient::LEDControlClient(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::LED, device_id)
{
}

void LEDControlClient::set_init(uint16_t led_num_max, uint16_t led_num_sum)
{
    FDCANFrame frame = FDCANFrame::make(id::DeviceType::LED, device_id_, id::MsgTypeLED::Init);
    converter::pack(frame.data, 0, led_num_max);
    converter::pack(frame.data, 2, led_num_sum);
    frame.dlc = sizeof(uint16_t) * 2;
    bus_.send_frame(frame);
}

void LEDControlClient::set_led_id(uint8_t led_id, uint16_t led_num_start, uint16_t led_num_end)
{
    FDCANFrame frame = FDCANFrame::make(id::DeviceType::LED, device_id_, id::MsgTypeLED::LEDIDSet);
    converter::pack(frame.data, 0, led_id);
    converter::pack(frame.data, 2, led_num_start);
    converter::pack(frame.data, 4, led_num_end);
    frame.dlc = 40;
    bus_.send_frame(frame);
}

void LEDControlClient::set_release_led_id(uint8_t led_id)
{
    FDCANFrame frame =
        FDCANFrame::make(id::DeviceType::LED, device_id_, id::MsgTypeLED::LEDIDRelease);
    converter::pack(frame.data, 0, led_id);
    frame.dlc = 8;
    bus_.send_frame(frame);
}

void LEDControlClient::set_led_color(uint8_t led_id, RGB pixel_rgb)
{
    FDCANFrame frame = FDCANFrame::make(id::DeviceType::LED, device_id_, id::MsgTypeLED::Color);
    converter::pack(frame.data, 0, led_id);
    converter::pack(frame.data, 1, pixel_rgb);
    frame.dlc = 32;
    bus_.send_frame(frame);
}

void LEDControlClient::set_flash(uint8_t led_id, FlashTypeLED flashtype)
{
    FDCANFrame frame = FDCANFrame::make(id::DeviceType::LED, device_id_, id::MsgTypeLED::Flash);
    converter::pack(frame.data, 0, led_id);
    converter::pack(frame.data, 1, flashtype);
    frame.dlc = 16;
    bus_.send_frame(frame);
}

void LEDControlClient::set_dark(uint8_t led_id)
{
    FDCANFrame frame = FDCANFrame::make(id::DeviceType::LED, device_id_, id::MsgTypeLED::Dark);
    converter::pack(frame.data, 0, led_id);
    frame.dlc = 8;
    bus_.send_frame(frame);
}

void LEDControlClient::on_receive(const FDCANFrame& frame) {}

}  // namespace devices
}  // namespace gn10_can
