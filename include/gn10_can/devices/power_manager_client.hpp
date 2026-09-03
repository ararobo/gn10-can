#pragma once
#include <array>
#include <optional>

#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/devices/power_manager_types.hpp"

namespace gn10_can {
namespace devices {

class PowerManagerClient : public FDCANDevice
{
public:
    PowerManagerClient(FDCANBus& bus, uint8_t dev_id);

    void set_init(power_manager::Config config);

    void set_stop(bool enable_stop);

    bool get_new_status(power_manager::Status& status);

    bool get_new_sensor(power_manager::Sensor& sensor);

    bool get_new_voltages(std::array<float, 4>& voltages);

    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<power_manager::Status> status_{};
    std::optional<power_manager::Sensor> sensor_{};
    std::optional<std::array<float, 4>> voltages_{};
};
}  // namespace devices
}  // namespace gn10_can