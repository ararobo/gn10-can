/**
 * @file motor_driver_server.hpp
 * @author Gento Aiba (aiba-gento)
 * @brief モータードライバー用デバイスクラスのヘッダーファイル
 * @version 0.1.0
 * @date 2026-02-16
 *
 * @copyright Copyright (c) 2026 Gento Aiba
 * SPDX-License-Identifier: GPL-3.0
 */
#pragma once

#include "gn10_can/core/can_device.hpp"
#include "gn10_can/devices/motor_driver_types.hpp"

namespace gn10_can {
namespace devices {

/**
 * @brief モータードライバー用デバイスクラス
 *
 */
class MotorDriverServer : public CANDevice {
  public:
    /**
     * @brief モータードライバー用デバイスクラスのコンストラクタ
     *
     * @param bus CANBusクラスの参照
     * @param dev_id デバイスID
     */
    MotorDriverServer(CANBus& bus, uint8_t dev_id);

    /**
     * @brief モータードライバーフィードバック送信関数
     *
     * @param current_val 現在値（速度制御の場合は速度、位置制御の場合は位置）
     * @param limit_switch_state リミットスイッチ状態（ビットマップ形式）
     */
    void send_feedback(float current_val, uint8_t limit_switch_state);

    /**
     * @brief モータードライバー状態送信関数
     *
     * @param load_current 電流
     * @param temperature 温度
     */
    void send_status(float load_current, int8_t temperature);

    /**
     * @brief CANパケット受信時の呼び出し関数の実装
     *
     * @param frame 受信したCANパケット
     */
    void on_receive(const CANFrame& frame) override;

  private:
};
}  // namespace devices
}  // namespace gn10_can
