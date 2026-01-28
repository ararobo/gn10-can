/**
 * @file can_device.hpp
 * @author Gento Aiba (aiba-gento)
 * @brief デバイス（MotorDriverやServoDriverなど）の抽象化クラス (Proposal 5: RAII & Modern API)
 * @version 0.2
 * @date 2026-01-28
 *
 * @copyright Copyright (c) 2026 Gento Aiba
 * SPDX-License-Identifier: GPL-3.0
 */
#pragma once

#include <vector>
#include <array>
#include <cstdint>
#include <initializer_list>
#include <cstring> /* for memcpy */

#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/core/can_frame.hpp"
#include "gn10_can/core/can_id.hpp"

namespace gn10_can {

class CANDevice {
  protected:
    CANBus& bus_;
    id::DeviceType type_;
    uint8_t id_;

  public:
    /**
     * @brief デバイス抽象化クラスのコンストラクタ (RAII)
     * コンストラクタで自動的にバスに登録されます。
     * 
     * @param bus CANバスへの参照
     * @param type デバイスタイプ
     * @param id デバイスID (0-15)
     */
    CANDevice(CANBus& bus, id::DeviceType type, uint8_t id) : bus_(bus), type_(type), id_(id) {
        // コマンド部を除いたベースID（上位8ビット相当）をキーとして登録
        // Layout: [Type(4)][ID(4)][Cmd(3)]
        // Key: Type << 4 | ID
        uint32_t key = (static_cast<uint32_t>(type) & 0x0F) << 4 | (id & 0x0F);
        bus_.attach(this, key);
    }

    /**
     * @brief デストラクタ (RAII)
     * デストラクタで自動的にバスから登録解除されます。
     */
    virtual ~CANDevice() {
        uint32_t key = (static_cast<uint32_t>(type_) & 0x0F) << 4 | (id_ & 0x0F);
        bus_.detach(key);
    }

    /**
     * @brief CANパケット受信時の呼び出し関数
     * @param frame 受信したCANパケット
     */
    virtual void on_receive(const CANFrame& frame) = 0;

    // --- Proposal 5 Modern APIs ---

    /**
     * @brief データ送信 (簡易API)
     * @param cmd コマンド(Enum or uint8_t)
     * @param data 送信データ
     */
    template <typename CmdEnum>
    bool send(CmdEnum cmd, const std::vector<uint8_t>& data) {
        CANFrame frame;
        // IDにコマンドを埋め込む
        frame.id = id::pack(type_, id_, cmd);
        
        frame.dlc = static_cast<uint8_t>(data.size());
        if(frame.dlc > 8) frame.dlc = 8;
        
        if(frame.dlc > 0) {
            std::memcpy(frame.data.data(), data.data(), frame.dlc);
        }
        return bus_.send_raw(frame);
    }


    // --- Legacy / Template Support (Updated to use bus_) ---

    template <typename CmdEnum>
    bool send(CmdEnum command, const uint8_t* data = nullptr, uint8_t len = 0) {
        std::vector<uint8_t> payload;
        if (data && len > 0) {
            payload.assign(data, data + len);
        }
        return send(static_cast<uint8_t>(command), payload);
    }
    
    template <typename CmdEnum>
    bool send(CmdEnum command, std::initializer_list<uint8_t> data) {
        return send(command, data.begin(), data.size());
    }

    template <typename CmdEnum, std::size_t N>
    bool send(CmdEnum command, const std::array<uint8_t, N>& data) {
        return send(command, data.data(), static_cast<uint8_t>(data.size()));
    }
};

}  // namespace gn10_can
