/**
 * @file esc_hub_client.hpp
 * @author Ayu Kanai
 * @brief ESCHubのサーバーのヘッダーファイル。
 * @version 0.1
 * @date 2026-04-08
 *
 * @copyright Copyright (c) 2026 ararobo
 *
 */
#pragma once

#include <optional>

#include "gn10_can/core/fdcan_bus.hpp"
#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/core/fdcan_frame.hpp"

namespace gn10_can {
namespace devices {

class ESCHubClient : public FDCANDevice
{
public:
    /**
     * @brief ESCHubClientのコンストラクタ
     * @details CANbusの登録とdevice_idの割り振りを行う
     */
    ESCHubClient(FDCANBus& bus, uint8_t device_id);

    /**
     * @brief vescのコマンドを受け取る関数
     *
     * @param vesc_moving true:動く false
     */
    void set_vesc_command(bool vesc_moving);

    /**
     * @brief encoderの読み取った値を受け取る関数
     *
     * @param encoder_value encoderのfeedbackで受け取った値
     * @return true encoderの値を受け取ることができた
     * @return false encoderの値を受け取ることができなかった
     */
    bool get_encoder_feedbacks(int32_t& encoder_value);

    /**
     * @brief データをprivate関数に格納してあげる関数
     */
    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<int32_t> encoder_value_;
};

}  // namespace devices
}  // namespace gn10_can