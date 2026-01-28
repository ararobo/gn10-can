# Investigation Results: feat/* Branch Differences and Characteristics

## Overview

This document summarizes the differences and characteristics of the three feat/* branches in the gn10-can repository. Each branch implements a different approach to addressing design challenges in the CAN library (resolving circular dependencies, improving usability, etc.).

## Target Branches

1. **feat/proposal-1-interface** - Dependency Inversion through Interface Separation
2. **feat/proposal-4-forward-decl** - Simple Structure Using Forward Declaration
3. **feat/proposal-5-raii** - Auto-Registration and Modern API with RAII

## Comparison Table

| Item | proposal-1-interface | proposal-4-forward-decl | proposal-5-raii |
|------|---------------------|------------------------|-----------------|
| **Main Concept** | Dependency Inversion Principle (DIP) | Circular reference resolution via forward declaration | Automatic management with RAII |
| **Manager Class Name** | CANBusController | CANManager | CANBus |
| **Interfaces** | ICANSender, ICANReceiver | None (forward declaration only) | None (friend class) |
| **Registration Method** | Manual (register_receiver) | Manual (register_device) | Automatic (constructor) |
| **Circular Reference Solution** | Interface separation | Forward declaration + .cpp implementation | friend + RAII |
| **Code Volume** | High (interfaces added) | Low (minimal changes) | Medium (includes API improvements) |
| **Ease of Use** | Standard | Standard | Very High |
| **Testability** | Very High | Medium | Medium-High |

## Detailed Analysis of Each Branch

---

### 1. feat/proposal-1-interface

**Design Philosophy**: Apply the Dependency Inversion Principle (DIP)

#### Key Changes

- `CANManager` → renamed to `CANBusController`
- Introduction of new interfaces:
  - `ICANSender`: Frame sending functionality
  - `ICANReceiver`: Frame receiving functionality
- `CANDevice` implements `ICANReceiver` and depends on `ICANSender`
- `CANBusController` implements `ICANSender`

#### Architecture

```
DriverInterface
       ↑
       |
CANBusController (implements ICANSender)
       ↑                    ↑
       |                    |
       |              ICANSender
       |                    ↑
       |                    |
    registers          uses for send
       |                    |
ICANReceiver ← implements ← CANDevice
```

#### Usage Example

```cpp
gn10_can::drivers::Stm32CanDriver driver;
gn10_can::CANBusController controller(driver);

MyMotorDriver motor(controller, 0x200);
controller.register_receiver(&motor);

while (true) {
    controller.update();
}
```

#### Benefits

- ✅ Complete resolution of circular dependencies
- ✅ High testability (easy mocking)
- ✅ Clear separation of concerns
- ✅ Compliant with SOLID principles

#### Drawbacks

- ❌ Increased number of files and code volume
- ❌ Requires understanding of interface concepts
- ❌ Somewhat complex design

---

### 2. feat/proposal-4-forward-decl

**Design Philosophy**: Minimal changes using C++ basic features (forward declaration)

#### Key Changes

- Class names remain as `CANManager` and `CANDevice`
- Forward declarations in header files:
  ```cpp
  class CANManager; // in can_device.hpp
  class CANDevice;  // in can_manager.hpp
  ```
- Full definitions included in implementation (.cpp) files
- Enhanced ID-based routing functionality

#### Architecture

```
DriverInterface
       ↑
       |
   CANManager (holds std::map<uint32_t, CANDevice*>)
       ↑                    
       |                    
    pointer               
       |                    
   CANDevice (holds CANManager*)
```

#### Usage Example

```cpp
gn10_can::drivers::Stm32CanDriver driver;
gn10_can::CANManager manager(driver);

MotorDriver motor1(&manager, 0x100);
manager.register_device(&motor1);

while (true) {
    manager.update();
}
```

#### Benefits

- ✅ Simplest solution
- ✅ Minimal changes from existing code
- ✅ Reduced compilation time
- ✅ Low learning cost

#### Drawbacks

- ❌ Tight coupling (CANDevice depends on CANManager implementation)
- ❌ Mock creation for testing is somewhat difficult
- ❌ Interdependency still exists

---

### 3. feat/proposal-5-raii

**Design Philosophy**: Developer Experience (DX) prioritized, "It just works"

#### Key Changes

- `CANManager` → renamed to `CANBus` (physical bus concept)
- Automatic registration/deregistration with RAII pattern:
  - Automatically registers to bus in constructor
  - Automatically deregisters from bus in destructor
- Uses `friend class CANDevice` to hide internal APIs
- Improved ID management (Type + ID based key)

#### Architecture

```
DriverInterface
       ↑
       |
    CANBus (friend of CANDevice)
       ↑                    
       |                    
  attach/detach          
  (automatic)               
       |                    
   CANDevice (RAII managed)
   - Constructor: auto attach
   - Destructor: auto detach
```

#### Usage Example

```cpp
gn10_can::drivers::Stm32CanDriver driver;
gn10_can::CANBus bus(driver);

// Auto-registration just by instantiation!
MyMotor motor_left(bus, 0x100);
MyMotor motor_right(bus, 0x101);

while (true) {
    bus.update();
    motor_left.set_speed(100);
}
// Auto-deregistration when going out of scope
```

#### Benefits

- ✅ Best usability (minimal code)
- ✅ Prevents registration forget bugs
- ✅ Safe scope-based management
- ✅ Prevents dangling pointers
- ✅ Intuitive API

#### Drawbacks

- ❌ Constructor side effects are less visible
- ❌ Requires mock bus for unit testing
- ❌ Requires understanding of C++ RAII concept

---

## File Changes Comparison

### proposal-1-interface
```
Main added files:
+ include/gn10_can/core/can_bus_controller.hpp
+ include/gn10_can/core/ican_sender.hpp
+ include/gn10_can/core/ican_receiver.hpp
+ src/core/can_bus_controller.cpp

Removed files:
- include/gn10_can/core/can_manager.hpp
- src/core/can_manager.cpp

Changes: 17 files (+224, -233)
```

### proposal-4-forward-decl
```
Main added files:
+ src/core/can_device.cpp (new)

Main changes:
* include/gn10_can/core/can_device.hpp (greatly simplified)
* include/gn10_can/core/can_manager.hpp (routing improvements)

Changes: 13 files (+121, -186)
```

### proposal-5-raii
```
Main added files:
+ include/gn10_can/core/can_bus.hpp
+ src/core/can_bus.cpp

Removed files:
- include/gn10_can/core/can_manager.hpp
- src/core/can_manager.cpp

Main changes:
* include/gn10_can/core/can_device.hpp (RAII implementation)
* include/gn10_can/devices/motor_driver.hpp (API improvements)

Changes: 16 files (+362, -445)
```

---

## Design Principles and Applied Patterns

| Branch | Main Design Patterns | SOLID Principles |
|---------|---------------------|------------------|
| proposal-1 | Dependency Inversion, Observer | DIP, ISP, SRP |
| proposal-4 | Forward Declaration (language feature) | SRP |
| proposal-5 | RAII, Facade | SRP, (partially DIP) |

---

## Recommendations and Selection Criteria

### Choose proposal-1-interface when:
- Test-Driven Development (TDD) is a priority
- Long-term maintenance with large development teams
- SOLID principle compliance is important
- Multiple implementations may need to be swapped

### Choose proposal-4-forward-decl when:
- Want to minimize impact on existing codebase
- Prioritize simplicity and performance
- Small to medium-sized projects
- Want to use only basic C++ features

### Choose proposal-5-raii when:
- Team has many beginners or new members
- Usability (DX) is the top priority
- Want to minimize bug occurrence
- Want to adopt modern C++ best practices

---

## Technical Details

### ID Management and Routing

#### proposal-1
- Retrieves receive ID via `ICANReceiver::get_target_id()`
- Routing with `std::map<uint32_t, ICANReceiver*>`

#### proposal-4
- Explicitly specifies RX ID during registration
- Routing with `std::map<uint32_t, CANDevice*>`
- Efficient matching with base ID and mask

#### proposal-5
- Uses Type(4bit) + ID(4bit) as key
- Registers with base ID excluding command part (3bit)
- Routing with `std::map<uint32_t, CANDevice*>`

### Memory Management

In all branches:
- Devices managed by pointers (ownership external)
- Minimal dynamic memory allocation (embedded-friendly)
- Uses `std::map` (can be changed to fixed arrays if needed)

---

## Commit History Analysis

### proposal-1: 4 commits (ahead of main)
```
1c6d0f3 feat: enhance CANDevice and CANBusController...
6b722dd feat: replace test_can_manager with test_can_bus_controller...
08787ee feat: refactor CAN communication structure...
ba6a64d feat: implement CANManager class...
```

### proposal-4: Many commits
- Includes design document updates
- Incremental implementation and test additions
- Also includes build system improvements

### proposal-5: Many commits
- Gradual introduction of modern APIs
- Complete revision of test cases
- Renewal of usage examples and documentation

---

## Test Code Differences

### proposal-1
```cpp
TEST(CANBusControllerTest, RegisterAndRoute) {
    MockDriver driver;
    CANBusController controller(driver);
    MockReceiver receiver(0x100);
    controller.register_receiver(&receiver);
    // ...
}
```

### proposal-4
```cpp
TEST(CANManagerTest, RegisterDevice) {
    MockDriver driver;
    CANManager manager(driver);
    MockDevice device(&manager, 0x100);
    manager.register_device(&device, 0x100);
    // ...
}
```

### proposal-5
```cpp
TEST(CANBusTest, AutoRegisterOnConstruction) {
    MockDriver driver;
    CANBus bus(driver);
    // Auto-registration on instantiation
    MockDevice device(bus, 0x100);
    // Auto-deregistration in destructor
}
```

---

## Conclusion

The three feat/* branches each have different design philosophies and priorities:

1. **proposal-1-interface**: "Textbook" implementation emphasizing software engineering principles
2. **proposal-4-forward-decl**: "Pragmatic" implementation emphasizing practicality and simplicity
3. **proposal-5-raii**: "Modern" implementation emphasizing user experience

The final choice should be made based on project scale, team experience level, maintainability requirements, etc.

## Related Documents

- [Proposal 1 Details](design/01_interface_injection.md)
- [Proposal 4 Details](design/04_simple_forward_decl.md)
- [Proposal 5 Details](design/05_modern_auto_register.md)
- [Design Proposal Comparison Summary](design/comparison_summary.md) (main branch)

---

**Created**: 2026-01-28  
**Target Repository**: ararobo/gn10-can  
**Investigator**: GitHub Copilot
