/**
 * @file can_bus.hpp
 * @author Gento Aiba (aiba-gento)
 * @brief
 * 物理的なFDCANバスをソフトウェア上で表現したクラス。デバイスの接続(Attach)と、メッセージのルーティング(Dispatch)を担当します。
 * @version 0.1.0
 * @date 2026-04-01
 *
 * @copyright Copyright (c) 2026 Gento Aiba
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "gn10_can/core/can_bus_base.hpp"
#include "gn10_can/core/can_frame.hpp"
#include "gn10_can/drivers/fdcan_driver_interface.hpp"

namespace gn10_can {

using FDCANBus = CANBusBase<FDCANFrame, drivers::IFDCANDriver>;

}  // namespace gn10_can
