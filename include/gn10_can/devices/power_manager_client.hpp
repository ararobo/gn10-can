#pragma once
#include <optional>

#include "gn10_can/core/fdcan_device.hpp"

namespace gn10_can {
namespace devices {

struct Status {
    bool enable_stop;
    float voltage;
    float current;
};

class PowerManagerClient : public FDCANDevice
{
public:
    PowerManagerClient(FDCANBus& bus, uint8_t dev_id);

    void set_init();

    void set_stop(bool enable_stop);

    bool get_status(Status& feedback);

    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<Status> feedback_;
};
}  // namespace devices
}  // namespace gn10_can