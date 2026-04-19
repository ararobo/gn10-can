#pragma once

namespace gn10_can {
namespace devices {
struct ESCHubConfig {
    float kp_ = 0.0f;
    float ki_ = 0.0f;
    float kd_ = 0.0f;
    float ff_ = 0.0f;

    void set_gain_kp(float kp)
    {
        kp_ = kp;
    }

    void set_gain_ki(float ki)
    {
        ki_ = ki;
    }

    void set_gain_kd(float kd)
    {
        kd_ = kd;
    }

    void set_gain_ff(float ff)
    {
        ff_ = ff;
    }
};
}  // namespace devices

}  // namespace gn10_can
