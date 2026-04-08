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

#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/core/can_device.hpp"
#include "gn10_can/core/can_frame.hpp"

namespace gn10_can {
namespace devices {
class ESCHubClient : public CANDevice
{
public:
    /**
     * @brief ESCHubClientのコンストラクタ
     * @details CANbusの登録とdevice_idの割り振りを行う
     */
    ESCHubClient(CANBus& bus, uint8_t device_id);

    /**
     * @brief ゲインの設定
     *
     * @param gain モーターに送る係数
     */
    void set_new_init(float gain);

    /**
     * @brief　角速度を設定する変数
     *
     * @param ang_vel_1 設定する角速度の1つ目
     * @param ang_vel_2 設定する角速度の2つ目
     * @param ang_vel_3 設定する角速度の3つ目
     * @param ang_vel_4 設定する速度の4つ目
     */
    void set_angular_velocity(float ang_vel_1, float ang_vel_2, float ang_vel_3, float ang_vel_4);

    /**
     * @brief 角速度を受け取る関数
     *
     * @param angluar_velocity 受け取った角速度
     * @return true 角速度を受け取ることができた
     * @return false 角速度を受け取ることができなかった。
     */
    bool get_angular_velocity_feedbacks(float angular_velocity_feedbacks[4]);

    /**
     * @brief データをprivate関数に格納してあげる関数
     */
    void on_receive(const CANFrame& frame) override;

private:
    // 角速度格納用構造体
    struct angular_velocity_feedbacks {
        float angular_velocity_feedback[4];
    } __attribute__((__packed__));

    std::optional<angular_velocity_feedbacks> angular_velocity_feedback_;
};

}  // namespace devices
}  // namespace gn10_can