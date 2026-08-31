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
     * @brief 設定されたLEDIDを指定された範囲と共に受けとる関数
     * @param led_setting LEDの範囲・テープ番号設定
     * @param led_id LEDの識別ID
     *
     * @return true IDと紐付けられた範囲を取得することができた
     * @return false IDと紐付けられた範囲を取得することができなかった
     */
    bool get_pixel_id(const LEDScopeSetting& led_setting, uint8_t led_id);

    /**
     * @brief LEDの識別番号設定を開放司令を取得する関数
     * @param led_id 開放したいLEDの識別番号
     *
     * @return true 指定した範囲とLEDIDを開放することができた
     * @return false  指定した範囲とLEDIDを開放することができた
     */
    bool reset_pixels_id(uint8_t led_id);

    /**
     * @brief LEDの色と光らせ方を取得する関数（範囲：指定LEDID)
     *
     * @param led_id 設定されたLEDの識別ID
     * @param rgb_config 色の設定
     * @param show_type 光らせ方の指定
     *
     * @return true led_idに対して光らせる色を取得することができた
     * @return false led_idに対して光らせる色を取得することができた
     */
    bool get_color_type_led_id(uint8_t led_id, const RGB& rgb, ShowTypeLED show_type);

    /**
     * @brief LEDの色と光らせ方を取得する関数（範囲：LEDテープ)
     *
     * @param led_tape LEDtape番号
     * @param rgb_config 色の設定
     * @param show_type 光らせ方の指定
     *
     * @return true led_tapeに対して光らせる色を取得することができた
     * @return false led_tapeに対して光らせる色を取得することができた
     */
    bool get_color_type_led_tape(uint8_t led_tape, const RGB& rgb, ShowTypeLED show_type);

    /**
     * @brief LEDの色と光らせ方を取得する関数（範囲：すべてのLED)
     *
     * @param rgb_config 色の設定
     * @param show_type 光らせ方の指定
     *
     * @return true すべてのLEDに対して光らせる色を取得することができた
     * @return false すべてのLEDに対して光らせる色を取得することができた
     */
    bool get_color_type_led_all(const RGB& rgb, ShowTypeLED show_type);

    /**
     * @brief LEDの点灯支持を受信する関数
     */
    bool get_show_all();

    /**
     * @brief LEDの消灯支持を受信する関数
     */
    bool get_clear_all();

    /**
     * @brief データをprivate関数に格納してあげる関数
     */
    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<RGB> rgb_;
};
}  // namespace devices
}  // namespace gn10_can