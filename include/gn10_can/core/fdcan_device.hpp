/**
 * @file fdcan_device.hpp
 * @author Gento Aiba (aiba-gento)
 * @brief FDCAN対応デバイスの抽象化クラスのヘッダーファイル
 * @version 0.1.0
 * @date 2026-04-01
 *
 * @copyright Copyright (c) 2026 Gento Aiba
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <array>
#include <cstdint>

#include "gn10_can/core/can_device_base.hpp"
#include "gn10_can/core/can_id.hpp"
#include "gn10_can/core/fdcan_bus.hpp"
#include "gn10_can/core/fdcan_frame.hpp"

namespace gn10_can {

using FDCANDevice = detail::CANDeviceBase<FDCANBus, FDCANFrame>;

}  // namespace gn10_can
