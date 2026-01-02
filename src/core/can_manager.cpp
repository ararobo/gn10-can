#include "gn10_can/core/can_manager.hpp"

#include "gn10_can/core/can_device.hpp"

namespace gn10_can {

CANManager::CANManager(drivers::DriverInterface& driver) : driver_(driver) {}
CANManager::~CANManager() {}

void CANManager::resister_device(CANDevice* device) {
    devices_.push_back(device);
}

}  // namespace gn10_can