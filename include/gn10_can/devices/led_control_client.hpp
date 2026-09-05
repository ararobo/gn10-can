#pragma once

#include <bitset>
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
     * @param led_id 設定するLEDの識別ID
     *
     * @return true 指定したLEDIDで指定した範囲を設定することができた
     * @return false 指定したLEDのIDで指定した範囲を設定することができなかった(ID使用済み)
     */
    bool set_pixel_id(const LEDScopeSetting& led_setting, uint8_t led_id);

    /**
     * @brief LEDの識別番号設定を開放する関数
     * @param led_id 開放したいLEDの識別番号
     */
    void reset_pixels_id(uint8_t led_id);

    /**
     * @brief LEDの色と光らせ方を設定する関数（範囲：指定LEDID)
     *
     * @param led_id 設定したLEDの識別ID
     * @param rgb_config 色の設定
     * @param show_type どのような光らせ方をするか
     */
    void set_color_type_led_id(uint8_t led_id, const RGB& rgb_config, ShowTypeLED show_type);

    /**
     * @brief LEDの色と光らせ方を設定する関数（範囲：指定LEDテープ）
     *
     * @param led_tape LEDテープの番号
     * @param rgb_config 色の設定
     * @param show_type どのような光らせ方をするか
     */
    void set_color_type_led_tape(uint8_t led_tape, const RGB& rgb_config, ShowTypeLED show_type);

    /**
     * @brief LEDの色と光らせ方を設定する関数（範囲：全体）
     *
     * @param rgb_config 色の設定
     * @param show_type どのような光らせ方をするか
     */
    void set_color_type_led_all(const RGB& rgb_config, ShowTypeLED show_type);

    /**
     * @brief LEDを点灯指示するプログラム（範囲：すべてのLED）
     */
    void set_show();

    /**
     * @brief LEDを消灯指示するプログラム（範囲：すべてのLED）
     */
    void set_clear();

    /**
     * @brief 受信フレームの処理ハンドラ
     */
    void on_receive(const FDCANFrame& frame) override;

private:
    std::bitset<256> check_ids{};  // ID（0~255）の使用状況フラグ（わずか32バイト）
};

}  // namespace devices
}  // namespace gn10_can