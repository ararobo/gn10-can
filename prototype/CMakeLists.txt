cmake_minimum_required(VERSION 3.10)
project(gn10_can_models_prototype)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

# Source files
set(SOURCES
    src/can_manager.cpp
    src/can_device.cpp
    src/motor_driver.cpp
    src/servo_driver.cpp
)

# Create executable
add_executable(prototype_test 
    tests/test_main.cpp
    ${SOURCES}
)
