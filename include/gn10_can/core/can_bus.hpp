/**
 * @file can_bus.hpp
 * @author Gento Aiba (aiba-gento)
 * @brief
 * 物理的なCANバスをソフトウェア上で表現したクラス。デバイスの接続(Attach)と、メッセージのルーティング(Dispatch)を担当します。
 * @version 0.1.0
 * @date 2026-01-28
 *
 * @copyright Copyright (c) 2026 Gento Aiba
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "gn10_can/core/can_bus_base.hpp"
#include "gn10_can/core/can_frame.hpp"
#include "gn10_can/drivers/can_driver_interface.hpp"

namespace gn10_can {

using CANBus = CANBusBase<CANFrame, drivers::ICANDriver>;

}  // namespace gn10_can
