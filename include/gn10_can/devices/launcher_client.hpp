#pragma once
#include <optional>

#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {
namespace devices {
class LauncherClient : public FDCANDevice
{
public:
    LauncherClient(FDCANBus& bus, uint8_t device_id);

    void set_init();

    void send_fire_command(float target_velocity);

    bool get_release_point(float& release_speed) const;

    bool get_initial_point(float& initial_angle) const;

    bool get_velocity_feedback(float& feedback_velocity) const;

    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<float> release_speed_;
    std::optional<float> initial_angle_;
    std::optional<float> feedback_velocity_;
};

}  // namespace devices
}  // namespace gn10_can