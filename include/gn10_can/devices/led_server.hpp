/**
 * @file led_server.hpp
 * @author Gento Aiba (aiba-gento)
 * @brief LEDに表示する情報を取得する
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
 * @brief LEDに反映させたい情報を得るLEDServerクラス
 *
 * @tparam DisplayInfo 表示する情報のデータ構造体
 */
template <typename DisplayInfo>
class LEDServer : public FDCANDevice
{
public:
    LEDServer(FDCANBus& bus, uint8_t dev_id) : FDCANDevice(bus, id::DeviceType::LED, dev_id)
    {
        static_assert(sizeof(DisplayInfo) <= 64, "DisplayInfo size exceeds FDCAN limit (64bytes)");
    }

    bool get_information(DisplayInfo& display_info)
    {
        if (display_info_.has_value()) {
            display_info = display_info_.value();
            display_info_.reset();
            return true;
        }
        return false;
    }

    void on_receive(const FDCANFrame& frame) override
    {
        auto id_fields = id::unpack(frame.id);
        if (id_fields.is_command(id::MsgTypeLED::DisplayInfo)) {
            if (frame.dlc != dlc::data_length_to_dlc(sizeof(DisplayInfo))) return;
            DisplayInfo display_info;
            if (converter::unpack(frame.data.data(), frame.dlc, 0, display_info)) {
                display_info_ = display_info;
            }
        }
    }

private:
    std::optional<DisplayInfo> display_info_;
};

}  // namespace devices
}  // namespace gn10_can