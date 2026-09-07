#pragma once
#include <optional>

#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {
namespace devices {
class LauncherClient : public FDCANDevice
{
public:
    void set_init();

    void send_fire_command(float target_rpm_ratio);

    bool get_release_point_speed(float& release_speed) const;

    bool get_initial_point_angle(float& initial_angle) const;

    bool get_velocity_feedback(float& feedback_velocity) const;

    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<float> angle_;
    std::optional<float> speed_;
    std::optional<float> velocity_;
};

}  // namespace devices
}  // namespace gn10_can