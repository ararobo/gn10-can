#pragma once
#include <cstdint>

namespace gn10_can {
namespace devices {

namespace power_manager {
typedef struct Config {
    bool use_remote_emergency_stop = false;
    uint16_t sensor_rate_ms        = 1000;
};

typedef struct Status {
    bool emergency_stop_enabled;
    bool remote_emergency_stop_connected;
    bool remote_emergency_stop_enabled;
    bool over_current;
};

typedef struct Sensor {
    float voltage;
    float current;
};
}  // namespace power_manager

}  // namespace devices
}  // namespace gn10_can