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

    void send_controller_data(const ControllerData& controller_data)
    {
        FDCANFrame frame = FDCANFrame::make(
            id::DeviceType::CommunicationModule,
            device_id_,
            id::MsgTypeCommunicationModule::ControllerData
        );
        converter::pack(frame.data, 0, controller_data);
        frame.dlc = sizeof(ControllerData);
        bus_.send_frame(frame);
    }

private:
};

}  // namespace devices
}  // namespace gn10_can