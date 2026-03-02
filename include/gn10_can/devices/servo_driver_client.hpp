/**
 * @file servo_driver_client.hpp
 * @author Gento Aiba (aiba-gento)
 * @brief サーボモータードライバーのクライアントクラス
 * @version 0.1
 * @date 2026-03-03
 *
 * @copyright Copyright (c) 2026
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "gn10_can/core/can_device.hpp"

namespace gn10_can {
class ServoDriverClient : public CANDevice
{
public:
    ServoDriverClient(CANBus& bus, uint8_t device_id);

    void set_init();

    void set_target();
};
}  // namespace gn10_can
