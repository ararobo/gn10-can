#include "gn10_can/devices/led_control_server.hpp"

#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {
namespace devices {

LEDControlServer::LEDControlServer(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::LED, device_id)
{
}

bool LEDControlServer::get_new_init(RGB& rgb)
{
    if (!rgb_.has_value()) {
        return false;
    }
    rgb = rgb_.value();
    rgb_.reset();
    return true;
}

bool LEDControlServer::get_pixel_id(LEDScopeSetting& led_setting, uint8_t& led_id)
{
    if (!led_scope_.has_value() || !led_id_.has_value()) {
        return false;
    }
    led_setting = led_scope_.value();
    led_id      = led_id_.value();
    led_scope_.reset();
    led_id_.reset();
    return true;
}

bool LEDControlServer::reset_pixels_id(uint8_t& led_id)
{
    if (!led_id_.has_value()) {
        return false;
    }
    led_id = led_id_.value();
    led_id_.reset();
    return true;
}

bool LEDControlServer::get_color_type_led_id(uint8_t& led_id, RGB& rgb, ShowTypeLED& show_type)
{
    if (!led_id_.has_value() || !rgb_.has_value() || !show_type_.has_value()) {
        return false;
    }
    led_id    = led_id_.value();
    rgb       = rgb_.value();
    show_type = show_type_.value();
    led_id_.reset();
    rgb_.reset();
    show_type_.reset();
    return true;
}

bool LEDControlServer::get_color_type_led_tape(uint8_t& led_tape, RGB& rgb, ShowTypeLED& show_type)
{
    if (!led_tape_.has_value() || !rgb_.has_value() || !show_type_.has_value()) {
        return false;
    }
    led_tape  = led_tape_.value();
    rgb       = rgb_.value();
    show_type = show_type_.value();
    led_tape_.reset();
    rgb_.reset();
    show_type_.reset();
    return true;
}

bool LEDControlServer::get_color_type_led_all(RGB& rgb, ShowTypeLED& show_type)
{
    if (!rgb_.has_value() || !show_type_.has_value()) {
        return false;
    }
    rgb       = rgb_.value();
    show_type = show_type_.value();
    rgb_.reset();
    show_type_.reset();
    return true;
}

bool LEDControlServer::get_show_all()
{
    if (!show_command_) {
        return false;
    }
    show_command_ = false;
    return true;
}

bool LEDControlServer::get_clear_all()
{
    if (!clear_command_) {
        return false;
    }
    clear_command_ = false;
    return true;
}

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
        bool ok = true;
        ok &= converter::unpack(frame.data.data(), frame.dlc, 0, led_scope);
        ok &= converter::unpack(frame.data.data(), frame.dlc, sizeof(LEDScopeSetting), led_id);
        if (ok) {
            led_scope_ = led_scope;
            led_id_    = led_id;
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
        bool ok = true;
        ok &= converter::unpack(frame.data.data(), frame.dlc, 0, led_id);
        ok &= converter::unpack(frame.data.data(), frame.dlc, sizeof(uint8_t), rgb);
        ok &= converter::unpack(
            frame.data.data(), frame.dlc, sizeof(uint8_t) + sizeof(RGB), show_type
        );
        if (ok) {
            led_id_    = led_id;
            rgb_       = rgb;
            show_type_ = show_type;
        }
    } else if (id_fields.is_command(id::MsgTypeLED::ColorTape)) {
        uint8_t led_tape;
        RGB rgb;
        ShowTypeLED show_type;
        bool ok = true;
        ok &= converter::unpack(frame.data.data(), frame.dlc, 0, led_tape);
        ok &= converter::unpack(frame.data.data(), frame.dlc, sizeof(uint8_t), rgb);
        ok &= converter::unpack(
            frame.data.data(), frame.dlc, sizeof(uint8_t) + sizeof(RGB), show_type
        );
        if (ok) {
            led_tape_  = led_tape;
            rgb_       = rgb;
            show_type_ = show_type;
        }
    } else if (id_fields.is_command(id::MsgTypeLED::ColorAll)) {
        RGB rgb;
        ShowTypeLED show_type;
        bool ok = true;
        // 修正: クライアント側は offset 0 に rgb, offset sizeof(RGB) に show_type を詰めている
        ok &= converter::unpack(frame.data.data(), frame.dlc, 0, rgb);
        ok &= converter::unpack(frame.data.data(), frame.dlc, sizeof(RGB), show_type);
        if (ok) {
            rgb_       = rgb;
            show_type_ = show_type;
        }
    } else if (id_fields.is_command(id::MsgTypeLED::Show)) {
        show_command_ = true;
    } else if (id_fields.is_command(id::MsgTypeLED::Clear)) {
        clear_command_ = true;
    }
}

}  // namespace devices
}  // namespace gn10_can