#pragma once
#include <optional>

#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
class LauncherServer : public FDCANDevice
{
public:
    void get_init();

    bool get_fire_command(float& target_rpm_ratio) const;

    void send_release_point_speed(float release_speed);

    void send_initial_point_angle(float initial_angle);

    void send_feedback_velocity(float feedback_velocity);

    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<float> rpm_ratio_;
};
}  // namespace devices
}  // namespace gn10_can