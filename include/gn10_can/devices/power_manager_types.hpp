#pragma once
#include <cstdint>

namespace gn10_can {
namespace devices {

namespace power_manager {
struct Config {
    bool use_remote_emergency_stop = false;
    uint16_t sensor_rate_ms        = 1000;
};

struct Status {
    bool emergency_stop_enabled;
    bool remote_emergency_stop_connected;
    bool remote_emergency_stop_enabled;
    bool over_current;

    bool operator==(const Status& other) const noexcept
    {
        return emergency_stop_enabled == other.emergency_stop_enabled &&
               remote_emergency_stop_connected == other.remote_emergency_stop_connected &&
               remote_emergency_stop_enabled == other.remote_emergency_stop_enabled &&
               over_current == other.over_current;
    }

    bool operator!=(const Status& other) const noexcept
    {
        return !(*this == other);
    }
};

struct Sensor {
    float voltage;
    float current;
};
}  // namespace power_manager

}  // namespace devices
}  // namespace gn10_can