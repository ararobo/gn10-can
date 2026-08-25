#pragma once

#include <optional>

#include "gn10_can/core/fdcan_bus.hpp"
#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/core/fdcan_frame.hpp"
#include "gn10_can/devices/led_control_tyoes.hpp"

namespace gn10_can {
namespace devices {
class LEDControlServer : public FDCANDevice
{
public:
    /**
     * @brief LEDControlClientのコンストラクタ
     * @details CANbusの登録とdevice_idの割り振りを行う
     */
    LEDControlServer(FDCANBus& bus, uint8_t device_id);

    /**
     * @brief 用いるLEDテープの中でのLEDの最大個数を受け取る関数
     *
     * @param led_num_max 用いるLEDテープの中でのLEDの最大個数
     * @return true LEDの最大個数を獲得することができた
     * @return false LEDの最大個数を獲得することができなかった
     *
     */
    bool get_new_init(RGB& rgb);

    bool get_pixel_id(uint8_t led_tape, uint16_t led_num_min, uint16_t led_num_max);

    bool reset_reset_id(uint16_t led_id);

    bool get_color_type_led_idset_color_type_led_id(uint8_t led_id, RGB rgb, ShowTypeLED show_type);

    bool get_color_type_led_tape(uint8_t led_tape, RGB rgb, ShowTypeLED show_type);

    bool get_color_type_led_all(RGB rgb, ShowTypeLED show_type);

    bool get_show_pixels(uint16_t led_id);

    bool get_show_tape(uint8_t led_tape_num);

    bool get_show_all();

    bool get_clear_pixels(uint16_t led_id);

    void get_clear_tape(uint8_t led_tape_num);

    void get_clear_all();

    /**
     * @brief データをprivate関数に格納してあげる関数
     */
    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<uint16_t> led_num_max_;
    std::optional<uint16_t> led_num_sum_;

    // uint16_t以上の場合led_idをfalseでかえす
};
}  // namespace devices
}  // namespace gn10_can