#include "esc_hub_client.hpp"

namespace gn10_can {
namespace devices {
ESCHubClient::ESCHubClient(FDCANBus& bus, uint8_t device_id)
    : FDCANDevice(bus, id::DeviceType::MotorDriver, device_id)
{
}

void ESCHubClient::set_gain_all(const ESCHubConfig& esc_hub_config) {}

void ESCHubClient::set_angular_velocities(float angular_velocities[4]) {}

bool ESCHubClient::get_angular_velocity_feedbacks(float angular_velocity_feedbacks[4]) {}

void ESCHubClient::on_receive(const FDCANFrame& frame) {}
}  // namespace devices
}  // namespace gn10_can