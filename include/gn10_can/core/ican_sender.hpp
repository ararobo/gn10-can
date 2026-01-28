#pragma once

#include "gn10_can/core/can_frame.hpp"

namespace gn10_can {

class ICANSender {
public:
    virtual bool send_frame(const CANFrame& frame) = 0;
    virtual ~ICANSender() = default;
};

} // namespace gn10_can
