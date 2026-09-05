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
    ESCHubServer(FDCANBus& bus, uint8_t dev_id);

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
     * @brief 4つのモーターの目標値を取得する関数
     *
     * @param targets 4つ分のモーターの目標値の配列
     * @return true 受け取ることができた
     * @return false 受け取ることができなかった。
     * @note init時に設定されるEncoderTypeによって単位が変化
     * IncrementalSpeed : [rad/s] (1秒あたりの角度変化（角速度）)
     * Absolute : [rad] (磁石を基準とした角度で、1回転すると同じ値に戻る。値域:0~2pi)
     * IncrementalTotal : [rad] (init受信時からの変位)
     */
    bool get_targets(float targets[4]);
    /**
     * @brief 4つのモーターのフィードバックを送信する関数
     *
     * @param feedbacks 4つ分のモーターのフィードバックの配列
     * @note init時に設定されるEncoderTypeによって単位が変化
     * IncrementalSpeed : [rad/s] (1秒あたりの角度変化（角速度）)
     * Absolute : [rad] (磁石を基準とした角度で、1回転すると同じ値に戻る。値域:0~2pi)
     * IncrementalTotal : [rad] (init受信時からの変位)
     */
    void set_feedbacks(const float feedbacks[4]);

    /**
     * @brief データをprivate関数に格納してあげる関数
     */
    void on_receive(const FDCANFrame& frame) override;

private:
    // 角速度格納用構造体
    struct Targets {
        float angular_velocity[4];
    };
    struct Gains {
        float kp;
        float ki;
        float kd;
        float ff;
    };
    std::optional<Targets> targets_;
    std::optional<MotorConfig> config_[4];
    std::optional<Gains> gains_[4];
    std::optional<float> target_;
    std::optional<EncoderType> encoder_type_;
};

}  // namespace devices
}  // namespace gn10_can