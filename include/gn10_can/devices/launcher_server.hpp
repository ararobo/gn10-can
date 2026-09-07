#pragma once
#include <optional>

#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
class LauncherServer : public FDCANDevice
{
public:
    LauncherServer(FDCANBus& bus, uint8_t device_id);

    /**
     * @brief 初期化処理用受信関数
     */
    bool get_init();

    /**
     * @brief 射出司令医を受け取る
     * @param target_velocity 射出時の目標角速度
     */
    bool get_fire_command(float& target_velocity);

    /**
     * @brief 射出完了を送信する
     * @param release_speed 射出点の速度
     */
    void send_release_point(float release_speed);

    /**
     * @brief 初期化完了を送信する
     * @param inital_angle 初期化が完了したところの角度
     */
    void send_initial_point(float initial_angle);

    /**
     * @brief 角速度を送信する
     * @param feedback_velocity 送信されてきた角速度
     */
    void send_velocity_feedback(float feedback_velocity);

    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<float> target_velocity_;
    bool init_received_;
};
}  // namespace devices
}  // namespace gn10_can