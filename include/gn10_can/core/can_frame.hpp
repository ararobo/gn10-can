/**
 * @file can_frame.hpp
 * @author Gento Aiba (aiba-gento)
 * @brief CANフレーム構造体のヘッダーファイル
 * @version 0.1.0
 * @date 2026-01-28
 *
 * @copyright Copyright (c) 2026 Gento Aiba
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

#include "gn10_can/core/can_dlc.hpp"
#include "gn10_can/core/can_id.hpp"

namespace gn10_can {

namespace detail {

/**
 * @brief CANフレーム構造体
 *
 */
template <std::size_t MaxDLC>
struct CANFrame {
    static constexpr std::size_t MAX_DLC = MaxDLC;

    uint32_t id = 0;                     // CAN ID
    std::array<uint8_t, MaxDLC> data{};  // データ配列
    uint8_t dlc      = 0;                // Data Length Code(データ長ではないことに注意)
    bool is_extended = false;

    CANFrame() = default;

    /**
     * @brief CANフレーム作成ヘルパー関数
     *
     * @tparam CmdEnum コマンドの列挙型
     * @param type デバイスの種類
     * @param dev_id デバイスのID
     * @param cmd コマンド
     * @param payload 送信データ
     * @param length 送信データの長さ
     * @return CANFrame 生成したCANフレーム
     */
    template <typename CmdEnum>
    static CANFrame make(
        id::DeviceType type,
        uint8_t dev_id,
        CmdEnum cmd,
        const uint8_t* payload = nullptr,
        std::size_t length     = 0
    )
    {
        CANFrame frame;
        frame.id = id::pack(type, dev_id, cmd);
        frame.set_data(payload, length);
        frame.dlc = dlc::data_length_to_dlc(length);
        return frame;
    }

    /**
     * @brief CANフレーム作成ヘルパー関数（initializer_list版）
     *
     * @tparam CmdEnum コマンドの列挙型
     * @param type デバイスの種類
     * @param dev_id デバイスのID
     * @param cmd コマンド
     * @param payload 送信データ（{データ配列}の様に関数呼び出し時に作成可能）
     * @return CANFrame 生成したCANフレーム
     */
    template <typename CmdEnum>
    static CANFrame make(
        id::DeviceType type, uint8_t dev_id, CmdEnum cmd, std::initializer_list<uint8_t> payload
    )
    {
        return make(type, dev_id, cmd, payload.begin(), payload.size());
    }

    /**
     * @brief CANフレームにデータを入れる関数
     *
     * @param payload 入れるデータ
     * @param length データの長さ[byte]
     */
    void set_data(const uint8_t* payload, std::size_t length)
    {
        // データサイズをMAX_DLC以下に制限
        std::size_t size;
        if (length < MAX_DLC) {
            size = length;
        } else {
            size = MAX_DLC;
        }
        // データをフレームのメンバ変数に入れる
        if (payload != nullptr && size > 0) {
            std::copy(payload, payload + size, data.begin());
        }
        // 書き込んだデータ以外を0で埋める
        if (size < MAX_DLC) {
            std::fill(data.begin() + size, data.end(), static_cast<uint8_t>(0));
        }

        set_data_length(static_cast<uint8_t>(size));
    }

    /**
     * @brief データ長をData Length Codeに変換して格納
     *
     * @param data_length データ長
     */
    void set_data_length(uint8_t data_length)
    {
        dlc = dlc::data_length_to_dlc(data_length);
    }

    /**
     * @brief ルーティング用のID（Command部を除外）を取得
     *
     * このIDはデバイスの特定に使用され、下位のコマンドビットは無視されます。
     * can_id.hpp の定義に基づいて計算されます。
     *
     * @return uint32_t ルーティングID (DeviceType + DeviceID)
     */
    uint32_t get_routing_id() const
    {
        // Commandビット幅分右シフトして切り捨てる
        return id >> id::BIT_WIDTH_COMMAND;
    }

    /**
     * @brief CANフレーム比較演算子
     *
     * @param other 比較対象のCANフレーム
     * @return true 等しい
     * @return false 等しくない
     */
    bool operator==(const CANFrame& other) const noexcept
    {
        if (id != other.id || dlc != other.dlc || is_extended != other.is_extended) {
            return false;
        }

        for (std::size_t i = 0; i < static_cast<std::size_t>(dlc); ++i) {
            if (data[i] != other.data[i]) return false;
        }
        return true;
    }

    /**
     * @brief CANフレーム不等比較演算子
     *
     * @param other 比較対象のCANフレーム
     * @return true 等しくない
     * @return false 等しい
     */
    bool operator!=(const CANFrame& other) const noexcept
    {
        return !(*this == other);
    }
};
}  // namespace detail

using CANFrame = detail::CANFrame<8>;

}  // namespace gn10_can
