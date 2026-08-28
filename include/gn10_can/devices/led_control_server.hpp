#pragma once

#include <optional>

#include "gn10_can/core/fdcan_bus.hpp"
#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/core/fdcan_frame.hpp"
#include "gn10_can/devices/led_control_types.hpp"

namespace gn10_can {
namespace devices {
class LEDControlServer : public FDCANDevice
{
public:
    /**
     * @brief LEDControlClientのコンストラクタ
     * @details CANbusの登録とdevice_idの割り振りを行う
     */
    LEDControlServer(FDCANBus& bus, uint8_t dev_id);

    /**
     * @brief LED初期化コマンド受信関数
     * @param rgb normal color設定
     */
    bool get_new_init(RGB& rgb);

    /**
     * @brief 細かいLEDの設定を行い、それにIDを結びつける関数
     * @param led_setting LEDの範囲・テープ番号設定
     * @param led_id LEDの識別ID
     */
    bool get_pixel_id(const LEDScopeSetting& led_setting, uint8_t led_id);

    bool reset_pixels_id(uint8_t led_id);

    bool get_color_type_led_id(uint8_t led_id, const RGB& rgb, ShowTypeLED show_type);

    bool get_color_type_led_tape(uint8_t led_tape, const RGB& rgb, ShowTypeLED show_type);

    bool get_color_type_led_all(const RGB& rgb, ShowTypeLED show_type);

    bool get_show_all();

    bool get_clear_all();

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