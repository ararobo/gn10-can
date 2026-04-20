#pragma once

#include <optional>

#include "gn10_can/core/fdcan_bus.hpp"
#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/core/fdcan_frame.hpp"
#include "gn10_can/devices/esc_hub_config.hpp"

namespace gn10_can {
namespace devices {

class ESCHubServer : public FDCANDevice
{
public:
    /**
     * @brief ESCHubServerのコンストラクタ
     * @details CANbusの登録とdevice_idの割り振りを行う
     */
    ESCHubServer(FDCANBus& bus, uint8_t device_id);

    /**
     * @brief　初期化受信
     */
    bool get_init();

    /**
     * @brief Pゲインにアクセスする関数
     *
     * @param p_gain pゲイン
     */
    bool get_p_gain(float p_gain, uint8_t motor_num);

    /**
     * @brief Iゲインにアクセスする関数
     *
     * @param i_gain iゲイン
     */
    bool get_i_gain(float i_gain, uint8_t motor_num);

    /**
     * @brief Dゲインにアクセスする関数
     *
     * @param d_gain dゲイン
     */
    bool get_d_gain(float d_gain, uint8_t motor_num);

    /**
     * @brief FF係数にアクセスする関数
     *
     * @param ff_coefficient フィードフォワードの係数
     */
    bool get_ff_coefficient(float ff_coefficient, uint8_t motor_num);

    /**
     * @brief 受け取った角速度にアクセスする関数（すべてできる）
     *
     * @param angular_velocities ４つ分のモーターの角速度の配列
     * @return true すべての角速度にアクセスすることができた
     * @return false すべての角速度にアクセスすることができなかった。
     */
    bool get_angular_velocities(float angular_velocities[4]);

    /**
     * @brief CANパケット受信時の呼び出し関数の実装
     *
     * @param frame 受信したCANパケット
     */
    void on_receive(const FDCANFrame& frame) override;

private:
    // 角速度格納用構造体
    struct AngularVelocities {
        float angular_velocity[4];
    };

    std::optional<AngularVelocities> angular_velocity_;
    std::optional<ESCHubConfig> motor_gain_[8];

    // gain
    float p_gain_[8];
    float i_gain_[8];
    float d_gain_[8];

    float ff_coefficient_[8];
};

}  // namespace devices
}  // namespace gn10_can