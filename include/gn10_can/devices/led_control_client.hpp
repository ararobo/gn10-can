#pragma once

#include <optional>

#include "gn10_can/core/fdcan_bus.hpp"
#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/core/fdcan_frame.hpp"

namespace gn10_can {
namespace devices {
class LEDControlClient : public FDCANDevice
{
public:
    struct RGB {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    enum class FlashTypeLED : uint8_t {
        Normal    = 0,
        Spinning  = 1,
        Gradually = 2,
    };

    /**
     * @brief LEDContolClientのコンストラクタ
     * @details CANbusの登録とdevice_idの割り振りを行う
     */
    LEDControlClient(FDCANBus& bus, uint8_t device_id);

    /**
     * @brief LED初期化コマンド送信関数
     *
     * @param led_num_max 用いるLEDテープの中でのLEDの最大個数
     */
    void set_init(uint16_t led_num_max);

    /**
     * @brief LED識別子設定関数
     *
     * @details　光らせるLEDの範囲の決定をし、idの割り振りをする
     *
     * @param led_id LED識別子
     * @param led_num_start 光らせるLEDのはじめの番号
     * @param led_num_emd 光らせるLEDの終わりの番号
     */
    void led_id_set(uint8_t led_id, uint16_t led_num_start, uint16_t led_num_end);

    /**
     * @brief LED識別子開放関数
     *
     * @param led_id 開放するLED識別子
     */
    void led_id_release(uint8_t led_id);

    /**
     * @brief LEDの色を設定する関数
     *
     * @param led_id LED識別子
     * @param rgb 色の割合の設定
     */
    void set_color(uint8_t led_id, RGB rgb);

    /**
     * @brief LEDを光らせる関数
     *
     * @param led_id LED識別子
     * @param flashtype LEDの光らせ方の設定
     */
    void set_flash(uint8_t led_id, FlashTypeLED flashtype);

    /**
     * @brief LEDを消す関数
     *
     * @param led_id LED識別子
     */
    void set_dark(uint8_t led_id);

    /**
     * @brief データをprivate関数に格納してあげる関数
     */
    void on_receive(const FDCANFrame& frame) override;

private:
};

}  // namespace devices
}  // namespace gn10_can