#include <iostream>
#include <queue>
#include <iomanip>
#include "gn10_can_models/can_manager.hpp"
#include "gn10_can_models/motor_driver.hpp"
#include "gn10_can_models/servo_driver.hpp"

using namespace gn10_can_models;

// --- Mock Transceiver for Testing ---
class MockTransceiver : public ICanTransceiver {
public:
    // Simulate receiving a frame from the bus (Hardware -> Code)
    void push_rx_frame(const CanFrame& frame) {
        rx_queue_.push(frame);
    }

    // ICanTransceiver implementation
    bool send(const CanFrame& frame) override {
        std::cout << "[TX] ID: 0x" << std::hex << frame.id << std::dec 
                  << " DLC: " << (int)frame.len << " Data: ";
        for (int i = 0; i < frame.len; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                      << (int)frame.data[i] << " ";
        }
        std::cout << std::dec << std::endl;
        return true;
    }

    bool receive(CanFrame& out_frame) override {
        if (rx_queue_.empty()) {
            return false;
        }
        out_frame = rx_queue_.front();
        rx_queue_.pop();
        return true;
    }

private:
    std::queue<CanFrame> rx_queue_;
};

// --- Helper to create frames ---
CanFrame create_motor_feedback(uint32_t id, float velocity, float temp) {
    CanFrame frame;
    frame.id = id;
    frame.len = 8;
    // Simple memcpy for prototype test (assuming Little Endian)
    std::memcpy(frame.data.data(), &velocity, sizeof(float));
    std::memcpy(frame.data.data() + 4, &temp, sizeof(float));
    return frame;
}

int main() {
    std::cout << "=== GN10 CAN Models Prototype Test ===" << std::endl;

    // 1. Setup
    MockTransceiver transceiver;
    CanManager manager(transceiver);

    // 2. Create Devices
    // Motor: RX=0x100, TX=0x101
    MotorDriver motor(manager, 0x100, 0x101);
    manager.register_device(&motor);

    // Servo: RX=0x200, TX=0x201
    ServoDriver servo(manager, 0x200, 0x201);
    manager.register_device(&servo);

    // 3. Test Sending (Code -> Hardware)
    std::cout << "\n--- Testing Transmission ---" << std::endl;
    std::cout << "Setting Motor Velocity to 15.5..." << std::endl;
    motor.set_velocity(15.5f);

    std::cout << "Setting Servo Angle to 90.0..." << std::endl;
    servo.set_angle(90.0f);

    // 4. Test Receiving (Hardware -> Code)
    std::cout << "\n--- Testing Reception ---" << std::endl;
    
    // Simulate incoming frames
    transceiver.push_rx_frame(create_motor_feedback(0x100, 12.3f, 45.0f));
    
    // Update manager to process queue
    manager.update();

    // Check if motor updated
    std::cout << "Motor Current Velocity: " << motor.get_current_velocity() << " (Expected: 12.3)" << std::endl;
    std::cout << "Motor Temperature: " << motor.get_temperature() << " (Expected: 45.0)" << std::endl;

    return 0;
}
