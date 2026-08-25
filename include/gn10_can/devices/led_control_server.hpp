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
    bool get_new_init(uint16_t& led_num_max, uint16_t& led_num_sum);

    /**
     * @brief LEDの識別子を取得する関数
     *
     * @param led_id LED識別子
     * @param led_num_start 光らせるLEDのはじめの番号
     * @param led_num_end 光らせるLEDの終わりの番号
     *
     * @return true LEDの識別IDを個数含めて登録することができた
     * @return false LEDの識別IDを登録することができず、LEDの個数の情報も取得できなかった
     */
    bool get_new_led_id(uint8_t& led_id, uint16_t& led_num_start, uint16_t& led_num_end);

    /**
     * @brief LED識別子の登録解除をするか取得する関数
     *
     * @param led_id 解除するLED識別子
     *
     * @return ture LED識別子の登録を解除することができた
     * @return false LED識別子の登録を解除することができなかった
     */
    bool get_new_release_led_id(uint8_t& led_id);

    /**
     * @brief LEDの色の設定を受け取る関数
     *
     * @param led_id LED識別子
     * @param rgb 色の割合の設定
     *
     * @return true LEDの色の設定を取得することができた
     * @return false LEDの色の設定を取得することができなかった
     */
    bool get_new_led_color(uint8_t& led_id, RGB& pixel_rgb);

    /**
     * @brief LEDを光らせる処理を受け取る関数
     *
     * @param led_id LED識別子
     * @param flashtype LEDの光らせ方の設定
     *
     * @return true LEDを光らせる処理を受け取ることができた
     * @return false LEDを光らせる処理を受け取ることができなかった
     */
    bool get_new_flash(uint8_t& led_id, ShowTypeLED flashtype);

    /**
     * @brief LEDを消す処理を受け取る関数
     *
     * @param led_id LED識別子
     *
     * @return true LEDを消す処理を受け取ることができた
     * @return false LEDを消す処理を受け取ることができなかった
     */
    bool get_new_dark(uint8_t& led_id);

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