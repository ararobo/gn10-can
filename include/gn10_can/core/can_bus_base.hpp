#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace gn10_can {
namespace detail {

template <typename BusType, typename FrameType>
class CANDeviceBase;

}  // namespace detail

/**
 * @brief CAN/FDCAN 共通バス基底クラス
 *
 * @tparam FrameType CAN フレーム型
 * @tparam DriverInterface ドライバインターフェース型
 */
template <typename FrameType, typename DriverInterface>
class CANBusBase
{
public:
    static constexpr std::size_t MAX_DEVICES = 16;

    explicit CANBusBase(DriverInterface& driver) : driver_(driver), devices_{}, device_count_(0) {}

    void update()
    {
        FrameType frame;
        while (driver_.receive(frame)) {
            dispatch(frame);
        }
    }

    bool send_frame(const FrameType& frame)
    {
        return driver_.send(frame);
    }

private:
    using DeviceType = detail::CANDeviceBase<CANBusBase<FrameType, DriverInterface>, FrameType>;
    friend DeviceType;

    bool attach(DeviceType* device)
    {
        if (device_count_ < MAX_DEVICES && device != nullptr) {
            devices_[device_count_++] = device;
            return true;
        }
        return false;
    }

    void detach(DeviceType* device)
    {
        for (std::size_t i = 0; i < device_count_; ++i) {
            if (devices_[i] == device) {
                devices_[i]             = devices_[--device_count_];
                devices_[device_count_] = nullptr;
                return;
            }
        }
    }

    void dispatch(const FrameType& frame)
    {
        uint32_t routing_id = frame.get_routing_id();

        for (std::size_t i = 0; i < device_count_; ++i) {
            DeviceType* device = devices_[i];
            if (device == nullptr) {
                continue;
            }

            if (routing_id == device->get_routing_id()) {
                device->on_receive(frame);
            }
        }
    }

    DriverInterface& driver_;
    std::array<DeviceType*, MAX_DEVICES> devices_{};
    std::size_t device_count_;
};

}  // namespace gn10_can
