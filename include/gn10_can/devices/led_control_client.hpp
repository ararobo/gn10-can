#pragma once

#include <optional>

#include "gn10_can/core/fdcan_bus.hpp"
#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/core/fdcan_frame.hpp"
#include "gn10_can/devices/led_control_tyoes.hpp"

namespace gn10_can {
namespace devices {
class LEDControlClient : public FDCANDevice
{
public:
    /**
     * @brief LEDContolClientのコンストラクタ
     * @details CANbusの登録とdevice_idの割り振りを行う
     */
    LEDControlClient(FDCANBus& bus, uint8_t device_id);

    /**
     * @brief LED初期化コマンド送信関数
     *
     * @param rgb normal color設定
     */
    void set_init(RGB rgb);

    /**
     * @brief 細かいLEDの設定を行い、それに自動でidを割り振る関数
     *
     * @param led_tape　用いるLEDテープの番号
     * @param led_num_min 光らせるLEDのはじめの番号
     * @param led_num_mix 光らせるLEDのおわりの番号
     *
     * @return LEDの識別番号 led_id
     */
    uint16_t set_pixel_id(uint8_t led_tape, uint16_t led_num_min, uint16_t led_num_max);

    /**
     * @brief LEDの識別番号設定を開放するプログラム
     *
     * @param led_id 開放したいLEDの識別番号
     */
    void reset_pixel_id(uint16_t led_id);

    /**
     * @brief LEDの色と光らせ方を設定する関数　（範囲：設定されたLEDID）
     *
     * @param led_id　光らせる設定されたLEDID
     * @param show_type LEDの光らせ方の設定
     */
    void set_color_type_led_id(uint8_t led_id, RGB rgb, ShowTypeLED show_type);

    /**
     * @brief LEDの色と光らせ方を設定する関数　（範囲：LEDテープ）
     *
     * @param led_tape 光らせるLEDテープの番号
     * @param show_type LEDの光らせ方の設定
     */
    void set_color_type_led_tape(uint8_t led_tape, RGB rgb, ShowTypeLED show_type);

    /**
     * @brief LEDの色と光らせ方を設定する関数　（範囲：全体
     *
     * @param show_type LEDの光らせ方の設定
     */
    void set_color_type_led_all(RGB rgb, ShowTypeLED show_type);

    /**
     * @brief LEDを光らせるプログラム　（範囲：すべてのLED）
     */
    void set_show_all();

    /**
     * @brief LEDを消すプログラム　（範囲：すべてのLED）
     */
    void set_clear_all();

    /**
     * @brief データをprivate関数に格納してあげる関数
     */
    void on_receive(const FDCANFrame& frame) override;

private:
};

}  // namespace devices
}  // namespace gn10_can