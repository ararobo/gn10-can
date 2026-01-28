/**
 * @file can_device.hpp
 * @author Gento Aiba (aiba-gento)
 * @brief デバイス（MotorDriverやServoDriverなど）の抽象化クラスのヘッダーファイル
 * @version 0.1
 * @date 2026-01-28
 *
 * @copyright Copyright (c) 2026 Gento Aiba
 * SPDX-License-Identifier: GPL-3.0
 */
#pragma once

#include <array>
#include <cstdint>
#include <initializer_list>

#include "gn10_can/core/can_frame.hpp"
#include "gn10_can/core/ican_sender.hpp"
#include "gn10_can/core/ican_receiver.hpp"

namespace gn10_can {

/**
 * @brief デバイス（MotorDriverやServoDriverなど）の抽象化クラス
 * @note 各デバイス毎に具体化クラスを継承して定義してください。
 *
 */
class CANDevice : public ICANReceiver {
  public:
    /**
     * @brief デバイス抽象化クラスのコンストラクタ
     *
     * @param sender
     * CANパケットを送信する為にICANSenderインターフェースのインスタンス参照を渡す
     * @param device_type デバイスの種類
     * @param device_id
     * デバイスのID（同じデバイスの種類のデバイスが複数あることを配慮して、0,1,2,..）
     */
    CANDevice(ICANSender& sender, id::DeviceType device_type, uint8_t device_id)
        : sender_(sender), device_type_(device_type), device_id_(device_id) {}
    
    virtual ~CANDevice() = default;

    /**
     * @brief CANパケット受信時の呼び出し関数
     *
     * @param frame 受信したCANパケット
     */
    virtual void on_receive(const CANFrame& frame) = 0;

    /**
     * @brief 受信対象のCAN-ID（基本部分）を取得
     * 
     * @return uint32_t Type + DeviceIDを含むID
     */
    uint32_t get_target_id() const override {
        // Type: bits 7-10, ID: bits 3-6
        // Command (0-2) is masked out by controller
        return (static_cast<uint32_t>(device_type_) & 0x0F) << 7 | 
               (static_cast<uint32_t>(device_id_) & 0x0F) << 3;
    }

    /**
     * @brief デバイスの種類を取得
     *
     * @return id::DeviceType デバイスの種類
     */
    id::DeviceType get_device_type() const { return device_type_; }

    uint8_t get_device_id() const { return device_id_; }

  protected:
    ICANSender& sender_; // Use Interface
    id::DeviceType device_type_;
    uint8_t device_id_;

    /**
     * @brief コマンド・データ・データ長からCANフレームを作成しSenderを使用して送信
     */
    template <typename CmdEnum>
    bool send(CmdEnum command, const uint8_t* data = nullptr, uint8_t len = 0) {
        auto frame = CANFrame::make(device_type_, device_id_, command, data, len);
        return sender_.send_frame(frame);
    }

    /**
     * @brief コマンド・データ(initializer list)からCANフレームを作成しSenderを使用して送信
     */
    template <typename CmdEnum>
    bool send(CmdEnum command, std::initializer_list<uint8_t> data) {
        return send(command, data.begin(), data.size());
    }

    /**
     * @brief コマンド・データ(array)からCANフレームを作成しSenderを使用して送信
     */
    template <typename CmdEnum, std::size_t N>
    bool send(CmdEnum command, const std::array<uint8_t, N>& data) {
        return send(command, data.data(), N);
    }
};
}  // namespace gn10_can
