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
#include "gn10_can/devices/esc_hub_config.hpp"

namespace gn10_can {
namespace devices {

class ESCHubClient : public FDCANDevice
{
public:
    /**
     * @brief ESCHubClientのコンストラクタ
     *
     * @details CANbusの登録とdevice_idの割り振りを行う
     */
    ESCHubClient(FDCANBus& bus, uint8_t device_id);

    /**
     * @brief 初期化送信（処理はなし）
     */
    void set_init();

    /**
     * @brief Pゲインを設定する関数
     *
     * @param p_gain pゲイン
     */
    void set_p_gain(float p_gain, uint8_t motor_num);

    /**
     * @brief Iゲインを設定する関数
     *
     * @param i_gain iゲイン
     */
    void set_i_gain(float i_gain, uint8_t motor_num);

    /**
     * @brief Dゲインを設定する関数
     *
     * @param d_gain dゲイン
     */
    void set_d_gain(float d_gain, uint8_t motor_num);

    /**
     * @brief FF係数を設定する関数
     *
     * @param ff_coefficient フィードフォワードの係数
     */
    void set_ff_coefficient(float ff_coefficient, uint8_t motor_num);

    /**
     * @brief　角速度を設定する変数
     *
     * @param angular_velocities ４つ分のモーターの角速度の配列
     */
    void set_angular_velocities(float angular_velocities[4]);

    /**
     * @brief 角速度を受け取る関数
     *
     * @param angular_velocity_feedbacks フィードバックで受け取った角速度
     * @return true すべての角速度を受け取ることができた
     * @return false すべての角速度を受け取ることができなかった。
     */
    bool get_angular_velocity_feedbacks(float angular_velocity_feedbacks[4]);

    /**
     * @brief データをprivate関数に格納してあげる関数
     */
    void on_receive(const FDCANFrame& frame) override;

private:
    // 角速度格納用構造体
    struct AngularVelocityFeedbacks {
        float angular_velocity_feedback[4];
    };

    std::optional<AngularVelocityFeedbacks> angular_velocity_feedback_;
    std::optional<ESCHubConfig> motor_gain_[8];

    // gain
    float p_gain_[8];
    float i_gain_[8];
    float d_gain_[8];

    float ff_coefficient_[8];
};

}  // namespace devices
}  // namespace gn10_can