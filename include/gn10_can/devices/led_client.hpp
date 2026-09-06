/**
 * @file led_client.hpp
 * @author Gento Aiba (aiba-gento)
 * @brief LEDを制御する基板に表示させる情報を送る
 * @version 0.1
 * @date 2026-09-05
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once
#include <optional>

#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {
namespace devices {

/**
 * @brief LEDに反映させたい情報を送るLEDClientクラス
 *
 * @tparam DisplayInfo 指令値のデータ構造体
 */
template <typename DisplayInfo>
class LEDClient : public FDCANDevice
{
public:
    LEDClient(FDCANBus& bus, uint8_t dev_id) : FDCANDevice(bus, id::DeviceType::LED, dev_id)
    {
        static_assert(sizeof(DisplayInfo) <= 64, "DisplayInfo size exceeds FDCAN limit (64bytes)");
    }

    void send_display_info(const DisplayInfo& display_info)
    {
        std::array<uint8_t, sizeof(DisplayInfo)> data{};
        converter::pack(data, 0, display_info);
        send(id::MsgTypeLED::DisplayInfo, data);
    }

    void on_receive(const FDCANFrame& frame) override {}

private:
};

}  // namespace devices
}  // namespace gn10_can