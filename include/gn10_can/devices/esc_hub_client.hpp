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
#include "gn10_can/devices/motor_driver_types.hpp"

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
     * @brief 各モータの設定を変更する関数
     *
     * @param motor_id モーターのid（0,1,2,3）
     * @param config モーターの設定
     */
    void set_init(const uint8_t motor_id, const MotorConfig& config);

    /**
     * @brief 各モータのゲインを設定する関数
     *
     * @param motor_id モーターのid（0,1,2,3）
     * @param kp Pゲイン
     * @param ki Iゲイン
     * @param kd Dゲイン
     * @param ff フィードフォワードゲイン
     */
    void set_gains(const uint8_t motor_id, float kp, float ki, float kd, float ff);

    /**
     * @brief　4つのモーターの目標値を送信する関数
     *
     * @param targets 4つ分のモーターの目標値の配列
     * @note init時に設定されるEncoderTypeによって単位が変化
     * IncrementalSpeed : [rad/s] (1秒あたりの角度変化（角速度）)
     * Absolute : [rad] (磁石を基準とした角度で、位置回転すると同じ値に戻る。値域:0~2pi)
     * IncrementalTotal : [rad] (init受信時からの変位)
     */
    void set_targets(const float targets[4]);

    /**
     * @brief 4つのモーターのフィードバックを取得する関数
     *
     * @param angular_velocity_feedbacks 4つ分のモーターのフィードバック
     * @return true 受け取ることができた
     * @return false 受け取ることができなかった。
     * @note init時に設定されるEncoderTypeによって単位が変化
     * IncrementalSpeed : [rad/s] (1秒あたりの角度変化（角速度）)
     * Absolute : [rad] (磁石を基準とした角度で、位置回転すると同じ値に戻る。値域:0~2pi)
     * IncrementalTotal : [rad] (init受信時からの変位)
     */
    bool get_feedbacks(float feedbacks[4]);

    /**
     * @brief データをprivate関数に格納してあげる関数
     */
    void on_receive(const FDCANFrame& frame) override;

private:
    // 角速度格納用構造体
    struct AngularVelocityFeedbacks {
        float angular_velocity_feedback[4];
    };

    std::optional<AngularVelocityFeedbacks> feedbacks_;
    std::optional<float> angle_feedback_;
};

}  // namespace devices
}  // namespace gn10_can