#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "gn10_can/core/can_frame.hpp"
#include "gn10_can/core/can_id.hpp"

namespace gn10_can {
namespace detail {

template <typename BusType, typename FrameType>
class CANDeviceBase
{
public:
    CANDeviceBase(BusType& bus, id::DeviceType device_type, uint8_t device_id)
        : bus_(bus), device_type_(device_type), device_id_(device_id)
    {
        bus_.attach(this);
    }

    virtual ~CANDeviceBase()
    {
        bus_.detach(this);
    }

    CANDeviceBase(const CANDeviceBase&)            = delete;
    CANDeviceBase& operator=(const CANDeviceBase&) = delete;
    CANDeviceBase(CANDeviceBase&&)                 = delete;
    CANDeviceBase& operator=(CANDeviceBase&&)      = delete;

    virtual void on_receive(const FrameType& frame) = 0;

    uint32_t get_routing_id() const
    {
        return (static_cast<uint32_t>(device_type_) << id::BIT_WIDTH_DEV_ID) |
               static_cast<uint32_t>(device_id_);
    }

protected:
    template <typename CmdEnum>
    bool send(CmdEnum command, const uint8_t* data = nullptr, std::size_t len = 0)
    {
        auto frame = FrameType::make(device_type_, device_id_, command, data, len);
        return bus_.send_frame(frame);
    }

    template <typename CmdEnum, std::size_t N>
    bool send(CmdEnum command, const std::array<uint8_t, N>& data)
    {
        return send(command, data.data(), static_cast<std::size_t>(data.size()));
    }

    BusType& bus_;
    id::DeviceType device_type_;
    uint8_t device_id_;
};

}  // namespace detail
}  // namespace gn10_can
