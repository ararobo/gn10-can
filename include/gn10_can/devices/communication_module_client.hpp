/**
 * @file communication_module_server.hpp
 * @author Gento Aiba (aiba-gento)
 * @brief 通信モジュールを介してコントローラーの信号などをCAN通信で共有する
 * @version 0.1
 * @date 2026-07-13
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once
#include <optional>

#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {
namespace devices {

template <typename ControllerData>
class CommunicationModuleServer : public FDCANDevice
{
public:
    CommunicationModuleServer(FDCANBus& bus, uint8_t dev_id)
        : FDCANDevice(bus, id::DeviceType::CommunicationModule, dev_id)
    {
        static_assert(
            sizeof(ControllerData) <= 64, "ControllerData size exceeds FDCAN limit (64bytes)"
        );
    }

    void set_init()
    {
        FDCANFrame frame = FDCANFrame::make(
            id::DeviceType::CommunicationModule, device_id_, id::MsgTypeCommunicationModule::Init
        );
        frame.dlc = 1;
        bus_.send_frame(frame);
    }

    bool get_controller_data(ControllerData& controller_data)
    {
        if (controller_data_.has_value()) {
            controller_data = controller_data_.value();
            controller_data_.reset();
            return true;
        }
        return false;
    }

    void on_receive(const FDCANFrame& frame) override
    {
        auto id_fields = id::unpack(frame.id);
        if (id_fields.is_command(id::MsgTypeCommunicationModule::ControllerData)) {
            if (frame.dlc == sizeof(ControllerData)) {
                ControllerData controller_data;
                if (converter::unpack(frame.data.data(), frame.dlc, 0, controller_data)) {
                    controller_data_ = controller_data;
                }
            }
        }
    }

private:
    std::optional<ControllerData> controller_data_;
};

}  // namespace devices
}  // namespace gn10_can