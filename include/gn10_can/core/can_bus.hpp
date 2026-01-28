/**
 * @file can_bus.hpp
 * @author Gento Aiba (aiba-gento)
 * @brief 物理的なCANバスをソフトウェア上で表現したクラス。デバイスの接続(Attach)と、メッセージのルーティング(Dispatch)を担当します。
 * @version 0.1
 * @date 2026-01-28
 *
 * @copyright Copyright (c) 2026 Gento Aiba
 * SPDX-License-Identifier: GPL-3.0
 */
#pragma once

#include <array>
#include <cstddef>

#include "gn10_can/drivers/driver_interface.hpp"

namespace gn10_can {

class CANDevice;

/**
 * @brief 物理的なCANバスをソフトウェア上で表現したクラス。デバイスの接続(Attach)と、メッセージのルーティング(Dispatch)を担当します。
 *
 */
class CANBus {
  public:
    static constexpr std::size_t MAX_DEVICES = 16;  // 最大登録デバイス数

    /**
     * @brief CANBusクラスのコンストラクタ
     *
     * @param driver CANドライバーインターフェースの参照
     */
    explicit CANBus(drivers::DriverInterface& driver);

    /**
     * @brief CANパケットの受信とデバイスへのルーティング処理
     *
     * 受信データを読み込み、適切なデバイスに渡します。
     */
    void update();

    /**
     * @brief CANフレーム送信関数
     *
     * @param frame 送信するCANフレーム
     * @return true 送信成功
     * @return false 送信失敗
     */
    bool send_frame(const CANFrame& frame);

  private:

    friend class CANDevice;

    void attach(CANDevice* device);

    void dispatch(const CANFrame& frame);

    drivers::DriverInterface& driver_;               // CANドライバーインターフェースの参照を保持
    std::array<CANDevice*, MAX_DEVICES> devices_{};  // 登録されているデバイスの配列
    std::size_t device_count_ = 0;                   // 登録されているデバイス数
};
}  // namespace gn10_can
