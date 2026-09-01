#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace gn10_can {
namespace detail {

/**
 * @brief CAN FDのデータ長(0~64)を対応するDLCコード(0~15)に変換
 *
 * STM32の FDCAN_DLC_BYTES_xx 定数と完全互換の数値(0~15)を返します。
 */
[[nodiscard]] constexpr uint8_t data_length_to_dlc(std::size_t length) noexcept
{
    if (length <= 8) {
        return static_cast<uint8_t>(length);
    }
    if (length <= 12) {
        return 9;
    }  // FDCAN_DLC_BYTES_12
    if (length <= 16) {
        return 10;
    }  // FDCAN_DLC_BYTES_16
    if (length <= 20) {
        return 11;
    }  // FDCAN_DLC_BYTES_20
    if (length <= 24) {
        return 12;
    }  // FDCAN_DLC_BYTES_24
    if (length <= 32) {
        return 13;
    }  // FDCAN_DLC_BYTES_32
    if (length <= 48) {
        return 14;
    }  // FDCAN_DLC_BYTES_48
    return 15;  // FDCAN_DLC_BYTES_64
}

/**
 * @brief DLCコード(0~15)を実際のバイト数(0~64)に逆変換
 *
 * ROS2側でCANフレームを受信・パブリッシュする際の有効長計算に使用します。
 */
[[nodiscard]] constexpr std::size_t dlc_to_data_length(uint8_t dlc) noexcept
{
    constexpr std::array<uint8_t, 16> DLC_TO_LEN_TABLE = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64
    };
    return DLC_TO_LEN_TABLE[dlc & 0x0F];
}

/**
 * @brief 任意のデータ長をCAN FDで送信可能な有効バイト数に切り上げる
 */
[[nodiscard]] constexpr std::size_t pad_data_length(std::size_t length) noexcept
{
    return dlc_to_data_length(data_length_to_dlc(length));
}

}  // namespace detail
}  // namespace gn10_can