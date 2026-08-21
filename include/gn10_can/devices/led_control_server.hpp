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
     * @brief LEDControlClientのコンストラクタ
     * @details CANbusの登録とdevice_idの割り振りを行う
     */
    LEDControlClient(FDCANBus& bus, uint8_t device_id);

    bool get_init(uint16_t led_num_max);

    bool get_led_id(uint8_t led_id, uint16_t led_num_start, uint16_t led_num_end);

    bool get_release_led_id(uint8_t led_id);

    bool get_pixel_color(uint8_t led_id, RGB pixel_rgb);

    bool get_flash(uint8_t led_id, FlashTypeLED flashtype);

    bool get_dark(uint8_t led_id);

    /**
     * @brief データをprivate関数に格納してあげる関数
     */
    void on_receive(const FDCANFrame& frame) override;

private:
};
}  // namespace devices
}  // namespace gn10_can