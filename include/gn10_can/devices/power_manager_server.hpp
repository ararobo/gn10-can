#pragma once
#include <optional>

#include "gn10_can/core/fdcan_device.hpp"

namespace gn10_can {
namespace devices {

struct Feedback {
    bool enable_stop;
    float voltage;
    float current;
};

class PowerManagerServer : public FDCANDevice
{
public:
    PowerManagerServer(FDCANBus& bus, uint8_t dev_id);

    bool get_init();

    bool get_stop(bool& enable_stop);

    void set_feedback(Feedback feedback);

    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<uint8_t> init_;
    std::optional<bool> enable_stop_;
};
}  // namespace devices
}  // namespace gn10_can