#pragma once

#include <optional>

#include "gn10_can/core/fdcan_bus.hpp"
#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/core/fdcan_frame.hpp"
#include "gn10_can/devices/motor_driver_types.hpp"

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
     * @brief 各モータの設定を取得する関数
     *
     * @param motor_id モーターのid（0,1,2,3）
     * @param config モーターの設定
     * @return true 新たな設定に更新した
     * @return false 新たな設定に更新できなかった
     */
    bool get_init(const uint8_t motor_id, MotorConfig& config);

    /**
     * @brief 各モータのゲインを取得する関数
     *
     * @param motor_id モーターのid（0,1,2,3）
     * @param kp Pゲイン
     * @param ki Iゲイン
     * @param kd Dゲイン
     * @param ff フィードフォワードゲイン
     * @return true 新たなゲインに更新した
     * @return false 新たなゲインに更新できなかった
     */
    bool get_gains(const uint8_t motor_id, float& kp, float& ki, float& kd, float& ff);

    /**
     * @brief 受け取った角速度にアクセスする関数（すべてできる）
     *
     * @param angular_velocities ４つ分のモーターの角速度の配列
     * @return true すべての角速度を受け取ることができた
     * @return false すべての角速度を受け取ることができなかった。
     */
    bool get_angular_velocities(float angular_velocities[4]);

    /**
     * @brief motorの角速度のfeedbackを送信する関数
     *
     * @param angular_velocity_feedbacks ４つ分のモーターの実際回っている角速度の配列
     */
    void set_angular_velocity_feedbacks(float angular_velocity_feedbacks[4]);

    /**
     * @brief データをprivate関数に格納してあげる関数
     */
    void on_receive(const FDCANFrame& frame) override;

private:
    // 角速度格納用構造体
    struct AngularVelocities {
        float angular_velocity[4];
    };
    struct Gains {
        float kp;
        float ki;
        float kd;
        float ff;
    };
    std::optional<AngularVelocities> angular_velocity_;
    std::optional<MotorConfig> config_[4];
    std::optional<Gains> gains_[4];
};

}  // namespace devices
}  // namespace gn10_can