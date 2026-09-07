#pragma once
#include <optional>

#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/utils/can_converter.hpp"
namespace gn10_can {
namespace devices {
class LauncherServer : public FDCANDevice
{
public:
    LauncherServer(FDCANBus& bus, uint8_t device_id);

    void get_init();

    bool get_fire_command(float& target_velocity) const;

    void send_release_point(float release_speed);

    void send_initial_point(float initial_angle);

    void send_velocity_feedback(float feedback_velocity);

    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<float> target_velocity_;
};
}  // namespace devices
}  // namespace gn10_can