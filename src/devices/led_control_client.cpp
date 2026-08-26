#include "gn10_can/devices/led_control_client.hpp"

#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {
namespace devices {

LEDControlClient::LEDControlClient(FDCANBus& bus, uint8_t dev_id)
    : FDCANDevice(bus, id::DeviceType::LED, dev_id)
{
}

void LEDControlClient::set_init(const RGB& rgb)
{
    FDCANFrame frame = FDCANFrame::make(id::DeviceType::LED, device_id_, id::MsgTypeLED::Init);
    converter::pack(frame.data, 0, rgb);
    frame.dlc = sizeof(RGB);
    bus_.send_frame(frame);
}

bool LEDControlClient::set_pixel_id(const LEDScopeSetting& led_setting, uint8_t led_id)
{
    if (check_ids.test(led_id)) {
        return false;
    }
    check_ids.set(led_id);

    FDCANFrame frame = FDCANFrame::make(id::DeviceType::LED, device_id_, id::MsgTypeLED::LEDIDSet);
    converter::pack(frame.data, 0, led_setting);
    converter::pack(frame.data, sizeof(LEDScopeSetting), led_id);
    frame.dlc = sizeof(LEDScopeSetting) + sizeof(uint8_t);
    bus_.send_frame(frame);

    return true;
}

void LEDControlClient::reset_pixel_id(uint8_t led_id)
{
    check_ids.reset(led_id);
    FDCANFrame frame =
        FDCANFrame::make(id::DeviceType::LED, device_id_, id::MsgTypeLED::LEDIDReset);
    converter::pack(frame.data, 0, led_id);
    frame.dlc = sizeof(uint8_t);
    bus_.send_frame(frame);
}

void LEDControlClient::set_color_type_led_id(uint8_t led_id, const RGB& rgb, ShowTypeLED show_type)
{
    FDCANFrame frame = FDCANFrame::make(id::DeviceType::LED, device_id_, id::MsgTypeLED::ColorID);
    converter::pack(frame.data, 0, led_id);
    converter::pack(frame.data, sizeof(uint8_t), rgb);
    converter::pack(frame.data, sizeof(uint8_t) + sizeof(RGB), show_type);
    frame.dlc = sizeof(uint8_t) + sizeof(RGB) + sizeof(ShowTypeLED);
    bus_.send_frame(frame);
}

void LEDControlClient::set_color_type_led_tape(
    uint8_t led_tape, const RGB& rgb, ShowTypeLED show_type
)
{
    FDCANFrame frame = FDCANFrame::make(id::DeviceType::LED, device_id_, id::MsgTypeLED::ColorTape);
    converter::pack(frame.data, 0, led_tape);
    converter::pack(frame.data, sizeof(uint8_t), rgb);
    converter::pack(frame.data, sizeof(uint8_t) + sizeof(RGB), show_type);
    frame.dlc = sizeof(uint8_t) + sizeof(RGB) + sizeof(ShowTypeLED);
    bus_.send_frame(frame);
}

void LEDControlClient::set_color_type_led_all(const RGB& rgb, ShowTypeLED show_type)
{
    FDCANFrame frame = FDCANFrame::make(id::DeviceType::LED, device_id_, id::MsgTypeLED::ColorAll);
    converter::pack(frame.data, 0, rgb);
    converter::pack(frame.data, sizeof(RGB), show_type);
    frame.dlc = sizeof(RGB) + sizeof(ShowTypeLED);
    bus_.send_frame(frame);
}

void LEDControlClient::set_show()
{
    FDCANFrame frame = FDCANFrame::make(id::DeviceType::LED, device_id_, id::MsgTypeLED::Show);
    frame.dlc        = 0;
    bus_.send_frame(frame);
}

void LEDControlClient::set_clear()
{
    FDCANFrame frame = FDCANFrame::make(id::DeviceType::LED, device_id_, id::MsgTypeLED::Clear);
    frame.dlc        = 0;
    bus_.send_frame(frame);
}

void LEDControlClient::on_receive(const FDCANFrame& frame) {}

}  // namespace devices
}  // namespace gn10_can