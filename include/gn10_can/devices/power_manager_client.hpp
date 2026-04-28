#pragma once
#include <optional>

#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {
namespace devices {

class PowerManagerClient : public FDCANDevice
{
public:
    PowerManagerClient(FDCANBus& bus, uint8_t dev_id);

    void set_init();

    void set_stop(bool enable_stop);

    void get_feedback(bool& enable_stop, float& voltage, float& current);

    void on_receive(const FDCANFrame& frame) override;

private:
    bool enable_stop_;
    float voltage_;
    float current_;
};
}  // namespace devices
}  // namespace gn10_can