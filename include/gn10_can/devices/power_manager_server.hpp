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

class PowerManagerServer : public FDCANDevice
{
public:
    PowerManagerServer(FDCANBus& bus, uint8_t dev_id);

    bool get_new_init();

    bool get_new_stop(bool& enable_stop);

    void set_status(Status status);

    void set_sensor(Sensor sensor);

    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<uint8_t> init_;
    std::optional<bool> enable_stop_;
};
}  // namespace devices
}  // namespace gn10_can