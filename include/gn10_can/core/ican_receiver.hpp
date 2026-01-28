#pragma once

#include "gn10_can/core/can_frame.hpp"
#include <cstdint>

namespace gn10_can {

class ICANReceiver {
public:
    virtual void on_receive(const CANFrame& frame) = 0;
    virtual uint32_t get_target_id() const = 0;
    virtual ~ICANReceiver() = default;
};

} // namespace gn10_can
