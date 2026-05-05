#pragma once
#include <optional>

#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/devices/power_manager_types.hpp"

namespace gn10_can {
namespace devices {

class PowerManagerServer : public FDCANDevice
{
public:
    PowerManagerServer(FDCANBus& bus, uint8_t dev_id);

    bool get_new_init(power_manager::Config& config);

    bool get_new_stop(bool& enable_stop);

    void set_status(power_manager::Status status);

    void set_sensor(power_manager::Sensor sensor);

    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<power_manager::Config> config_;
    std::optional<bool> enable_stop_;
};
}  // namespace devices
}  // namespace gn10_can