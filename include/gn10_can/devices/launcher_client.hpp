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
     * @param target_erpm 射出時のERPM
     */
    void send_fire_command(float target_erpm);

    /**
     * @brief ジャム解消用司令を送る
     * @param rotate_erpm ジャム解消時に回すERPM
     */
    void send_unjam_command(float rotate_erpm);

    /**
     * @brief 射出完了を受信する
     * @param release_erpm 射出点のERPM
     */
    bool get_release_point(float& release_erpm);

    /**
     * @brief 初期化完了を受信する
     * @param initial_angle 初期化が完了したところの角度
     */
    bool get_initial_point(float& initial_angle);

    /**
     * @brief フィードバックデータを取得する
     * @param command_erpm 指令値
     * @param current_erpm 現在値
     * @param voltage 電圧
     * @param current 電流
     */
    bool get_feedbacks(float& command_erpm, float& current_erpm, float& voltage, float& current);

    void on_receive(const FDCANFrame& frame) override;

private:
    struct Feedbacks {
        float command_erpm;
        float current_erpm;
        float voltage;
        float current;
    };
    std::optional<float> release_erpm_;
    std::optional<float> initial_angle_;
    std::optional<Feedbacks> feedbacks_;
};

}  // namespace devices
}  // namespace gn10_can