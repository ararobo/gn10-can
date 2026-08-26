#pragma once

#include <optional>

#include "gn10_can/core/fdcan_bus.hpp"
#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/core/fdcan_frame.hpp"
#include "gn10_can/devices/led_control_types.hpp"  // タイポ修正

namespace gn10_can {
namespace devices {

class LEDControlClient : public FDCANDevice
{
public:
    /**
     * @brief LEDControlClientのコンストラクタ
     * @details CAN busへの登録と device_id の保持を行う
     */
    LEDControlClient(FDCANBus& bus, uint8_t dev_id);

    /**
     * @brief LED初期化コマンド送信関数
     * @param rgb normal color設定
     */
    void set_init(const RGB& rgb);

    /**
     * @brief 細かいLEDの設定を行い、それにIDを結びつける関数
     * @param led_setting LEDの範囲・テープ番号設定
     * @param led_id LEDの識別ID
     */
    void set_pixel_id(const LEDScopeSetting& led_setting, uint8_t led_id);

    /**
     * @brief LEDの識別番号設定を開放する関数
     * @param led_id 開放したいLEDの識別番号
     */
    void reset_pixel_id(uint8_t led_id);

    /**
     * @brief LEDの色と光らせ方を設定する関数（範囲：指定LED ID）
     */
    void set_color_type_led_id(uint8_t led_id, const RGB& rgb, ShowTypeLED show_type);

    /**
     * @brief LEDの色と光らせ方を設定する関数（範囲：指定LEDテープ）
     */
    void set_color_type_led_tape(uint8_t led_tape, const RGB& rgb, ShowTypeLED show_type);

    /**
     * @brief LEDの色と光らせ方を設定する関数（範囲：全体）
     */
    void set_color_type_led_all(const RGB& rgb, ShowTypeLED show_type);

    /**
     * @brief LEDを点灯指示するプログラム（範囲：すべてのLED）
     */
    void set_show_all();

    /**
     * @brief LEDを消灯指示するプログラム（範囲：すべてのLED）
     */
    void set_clear_all();

    /**
     * @brief 受信フレームの処理ハンドラ
     */
    void on_receive(const FDCANFrame& frame) override;

private:
};

}  // namespace devices
}  // namespace gn10_can