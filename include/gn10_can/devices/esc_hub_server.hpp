#pragma once

#include <optional>

#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/core/can_device.hpp"
#include "gn10_can/core/can_frame.hpp"

namespace gn10_can {
namespace devices {
class ESCHubServer : public CANDevice
{
public:
    /**
     * @brief ESCHubServerのコンストラクタ
     * @details CANbusの登録とdevice_idの割り振りを行う
     */
    ESCHubServer(CANBus& bus, uint8_t device_id);

    /**
     * @brief 受け取ったgainを保存する関数
     *
     * @param gain 受け取ったゲイン
     * @return true ゲインを受け取ることができた
     * @return false ゲインを受け取ることができなかった
     */
    bool get_new_init(float& gain);

    /**
     * @brief すべての角速度を受け取る関数
     *
     * @param ang_vel_1 受け取った角速度の1つ目
     * @param ang_vel_2 受け取った角速度の2つ目
     * @param ang_vel_3 受け取った角速度の3つ目
     * @param ang_vel_4 受け取った角速度の4つ目
     *
     */
    void get_angular_velocity_all(
        float& ang_vel_1, float& ang_vel_2, float& ang_vel_3, float& ang_vel_4
    );

    /**
     * @brief データをprivate関数に格納してあげる関数
     */
    void on_receive(const CANFrame& frame) override;

private:
    // 角速度格納用構造体
    struct angular_velocities {
        float angular_velocity[4];
    } __attribute__((__packed__));

    std::optional<angular_velocities> angular_velocity_;
    std::optional<float> gain_;
};

}  // namespace devices
}  // namespace gn10_can