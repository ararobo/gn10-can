#pragma once
#include <optional>

#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {
namespace devices {
class LauncherClient
{
public:
    void init();

    void send_fire_command(float target_rpm_ratio);

    void get_release_point_speed(float& release_point_speed) const;

    void get_inital_point_angle(float& inital_point_angle) const;

    void get_feedback_velocity(float& feedback_velocity) const;

private:
    std::optional<float> angle_;
    std::optional<float> speed_;
    std::optional<float> velocity_;
};

}  // namespace devices
}  // namespace gn10_can