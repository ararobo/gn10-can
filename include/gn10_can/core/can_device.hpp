/**
 * @file can_device.hpp
 * @author Gento Aiba (aiba-gento)
 * @brief デバイス（MotorDriverやServoDriverなど）の抽象化クラスのヘッダーファイル
 * @version 0.1.0
 * @date 2026-01-10
 *
 * @copyright Copyright (c) 2026 Gento Aiba
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <array>
#include <cstdint>

#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/core/can_device_base.hpp"
#include "gn10_can/core/can_frame.hpp"
#include "gn10_can/core/can_id.hpp"

namespace gn10_can {

using CANDevice = detail::CANDeviceBase<CANBus, CANFrame>;

}  // namespace gn10_can
