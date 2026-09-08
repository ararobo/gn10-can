#pragma once
#include <optional>

#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {
namespace devices {
class LauncherClient : public FDCANDevice
{
public:
    LauncherClient(FDCANBus& bus, uint8_t device_id);

    /**
     * @brief 初期化処理用送信関数
     */
    void set_init();

    /**
     * @brief 射出司令を送る
     * @param target_velocity 射出時の目標速度
     */
    void send_fire_command(float target_velocity);

    /**
     * @brief 射出完了を受信する
     * @param release_velocity 射出点の速度
     */
    bool get_release_point(float& release_velocity);

    /**
     * @brief 初期化完了を受信する
     * @param inital_angle 初期化が完了したところの角度
     */
    bool get_initial_point(float& initial_angle);

    /**
     * @brief 速度を取得する
     * @param feedback_velocity 送信されてきた速度
     */
    bool get_velocity_feedback(float& feedback_velocity);

    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<float> release_velocity_;
    std::optional<float> initial_angle_;
    std::optional<float> feedback_velocity_;
};

}  // namespace devices
}  // namespace gn10_can