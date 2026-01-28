/**
 * @file can_bus_controller.hpp
 * @description CAN通信を統括するクラス(Bus Controller)のヘッダーファイル
 * @version 0.1
 */
#pragma once

#include <map>
#include <cstddef>
#include <cstdint>

#include "gn10_can/drivers/driver_interface.hpp"
#include "gn10_can/core/ican_sender.hpp"
#include "gn10_can/core/ican_receiver.hpp"

namespace gn10_can {

/**
 * @brief CAN通信を統括するクラス (旧 CANManager)
 */
class CANBusController : public ICANSender {
  public:
    explicit CANBusController(drivers::DriverInterface& driver);

    void register_receiver(ICANReceiver* receiver);
    void update();
    bool send_frame(const CANFrame& frame) override;

  private:
    drivers::DriverInterface& driver_; 
    std::map<uint32_t, ICANReceiver*> router_;
};
}  // namespace gn10_can
