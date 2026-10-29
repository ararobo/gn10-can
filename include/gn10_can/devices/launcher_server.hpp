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
     * @param target_erpm 射出時のERPM
     */
    bool get_fire_command(float& target_erpm);

    /**
     * @param のジャム解消用司令を受け取る
     * @param rotate_erpm ジャム解消時に回すERPM
     */
    bool get_unjam_command(float& rotate_erpm);

    /**
     * @brief 射出完了を送信する
     * @param release_erpm 射出点のERPM
     */
    void send_release_point(float release_erpm);

    /**
     * @brief 初期化完了を送信する
     * @param inital_angle 初期化が完了したところの角度
     */
    void send_initial_point(float initial_angle);

    /**
     * @brief フィードバックデータを送信する
     * @param command_erpm 指令値
     * @param current_erpm 現在値
     * @param voltage 電圧
     * @param current 電流
     */
    void send_feedbacks(
        const float& command_erpm,
        const float& current_erpm,
        const float& voltage,
        const float& current
    );

    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<float> target_erpm_;
    std::optional<float> rotate_erpm_;
    bool init_received_ = false;
};
}  // namespace devices
}  // namespace gn10_can