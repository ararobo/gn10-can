#pragma once
#include <optional>

#include "gn10_can/core/fdcan_device.hpp"

namespace gn10_can {
namespace devices {

struct Status {
    bool emergency_stop_enabled;
    bool remote_emergency_stop_connected;
    bool remote_emergency_stop_enabled;
    bool over_current;
};

struct Sensor {
    float voltage;
    float current;
};

class PowerManagerClient : public FDCANDevice
{
public:
    PowerManagerClient(FDCANBus& bus, uint8_t dev_id);

    void set_init();

    void set_stop(bool enable_stop);

    bool get_status(Status& status);

    bool get_sensor(Sensor& sensor);

    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<Status> status_;
};
}  // namespace devices
}  // namespace gn10_can