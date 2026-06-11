#pragma once

#include <optional>

#include "gn10_can/core/fdcan_bus.hpp"
#include "gn10_can/core/fdcan_device.hpp"
#include "gn10_can/core/fdcan_frame.hpp"

namespace gn10_can {
namespace devices {

class ESCHubServer : public FDCANDevice
{
public:
    /**
     * @brief ESCHubServerのコンストラクタ
     * @details CANbusの登録とdevice_idの割り振りを行う
     */
    ESCHubServer(FDCANBus& bus, uint8_t device_id);

    bool get_vesc_command(bool& vesc_moving);

    /**
     * @brief client 側にencoderの値を送ってあげる関数
     */
    void set_encoder_feedbacks(int32_t encoder_value);

    /**
     * @brief データをprivate関数に格納してあげる関数
     */
    void on_receive(const FDCANFrame& frame) override;

private:
    std::optional<bool> vesc_command_ = false;
};

}  // namespace devices
}  // namespace gn10_can