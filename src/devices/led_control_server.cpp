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

void LEDControlServer::on_receive(const FDCANFrame& frame)
{
    auto id_fields = id::unpack(frame.id);
    if (id_fields.is_command(id::MsgTypeLED::Init)) {
        RGB rgb;
        if (converter::unpack(frame.data.data(), frame.dlc, 0, rgb)) {
            rgb_ = rgb;
        }
    } else if (id_fields.is_command(id::MsgTypeLED::LEDIDSet)) {
        LEDScopeSetting led_scope;
        uint8_t led_id;
        if (converter::unpack(frame.data.data(), frame.dlc, 0, led_scope)) {
            led_scope_ = led_scope;
        }
        if (converter::unpack(frame.data.data(), frame.dlc, sizeof(LEDScopeSetting), led_id)) {
            led_id_ = led_id;
        }
    } else if (id_fields.is_command(id::MsgTypeLED::LEDIDReset)) {
        uint8_t led_id;
        if (converter::unpack(frame.data.data(), frame.dlc, 0, led_id)) {
            led_id_ = led_id;
        }
    } else if (id_fields.is_command(id::MsgTypeLED::ColorID)) {
        uint8_t led_id;
        RGB rgb;
        ShowTypeLED show_type;
        if (converter::unpack(frame.data.data(), frame.dlc, 0, led_id)) {
            led_id_ = led_id;
        }
        if (converter::unpack(frame.data.data(), frame.dlc, sizeof(uint8_t), rgb)) {
            rgb_ = rgb;
        }
        if (converter::unpack(
                frame.data.data(), frame.dlc, sizeof(uint8_t) + sizeof(RGB), show_type
            )) {
            show_type_ = show_type;
        }
    } else if (id_fields.is_command(id::MsgTypeLED::ColorTape)) {
        uint8_t led_tape;
        RGB rgb;
        ShowTypeLED show_type;
        if (converter::unpack(frame.data.data(), frame.dlc, 0, led_tape)) {
            led_tape_ = led_tape;
        }
        if (converter::unpack(frame.data.data(), frame.dlc, sizeof(uint8_t), rgb)) {
            rgb_ = rgb;
        }
        if (converter::unpack(
                frame.data.data(), frame.dlc, sizeof(uint8_t) + sizeof(RGB), show_type
            )) {
            show_type_ = show_type;
        }
    } else if (id_fields.is_command(id::MsgTypeLED::ColorAll)) {
        RGB rgb;
        ShowTypeLED show_type;
        if (converter::unpack(frame.data.data(), frame.dlc, sizeof(uint8_t), rgb)) {
            rgb_ = rgb;
        }
        if (converter::unpack(
                frame.data.data(), frame.dlc, sizeof(uint8_t) + sizeof(RGB), show_type
            )) {
            show_type_ = show_type;
        }
    } else if (id_fields.is_command(id::MsgTypeLED::Show)) {
        if (!show_command_) {
            show_command_ = true;
        }
    } else if (id_fields.is_command(id::MsgTypeLED::Clear)) {
        if (!clear_command_) {
            clear_command_ = true;
        }
    }

}  // namespace devices
}  // namespace gn10_can
