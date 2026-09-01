/**
 * @file robot_control_hub_client.hpp
 * @author Gento Aiba (aiba-gento)
 * @brief ロボットを統括し、PCとの橋渡しを担うデバイスのクライアント(PC側)クラス
 * @version 0.1
 * @date 2026-04-12
 *
 * @copyright Copyright (c) 2026 ararobo
 *
 */
#pragma once
#include <optional>

#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/utils/can_converter.hpp"

namespace gn10_can {
namespace devices {

/**
 * @brief RobotControlHubのクライアント用デバイスクラス(PCなどを想定)
 *
 * @tparam Command 指令値のデータ構造体
 * @tparam Feedback フィードバックのデータ構造体
 */
template <typename Command, typename Feedback>
class RobotControlHubClient : public FDCANDevice
{
public:
    RobotControlHubClient(FDCANBus& bus, uint8_t dev_id)
        : FDCANDevice(bus, id::DeviceType::RobotControlHub, dev_id)
    {
        static_assert(sizeof(Command) <= 64, "Command size exceeds FDCAN limit (64bytes)");
        static_assert(sizeof(Feedback) <= 64, "Feedback size exceeds FDCAN limit (64bytes)");
    }

    void send_command(const Command& command)
    {
        std::array<uint8_t, sizeof(Command)> data{};
        converter::pack(data, 0, command);
        send(id::MsgTypeRobotControlHub::Command, data);
    }

    bool get_feedback(Feedback& feedback)
    {
        if (feedback_.has_value()) {
            feedback = feedback_.value();
            feedback_.reset();
            return true;
        }
        return false;
    }

    void on_receive(const FDCANFrame& frame) override
    {
        auto id_fields = id::unpack(frame.id);
        if (id_fields.is_command(id::MsgTypeRobotControlHub::Feedback)) {
            if (frame.dlc != dlc::data_length_to_dlc(sizeof(Feedback))) return;
            Feedback feedback;
            if (converter::unpack(frame.data.data(), frame.data_length, 0, feedback)) {
                feedback_ = feedback;
            }
        }
    }

private:
    std::optional<Feedback> feedback_;
};

}  // namespace devices
}  // namespace gn10_can