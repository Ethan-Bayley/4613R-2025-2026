# Autonomous Routine Implementation Guide

## Overview

This guide explains how to implement autonomous routines while respecting C++ ownership rules in the robot codebase.

## Core Principle: Ownership Model

- **Robot owns subsystems** via `std::unique_ptr<Subsystem>` in `robotSubsystems` vector
- **Autonomous routines receive non-owning references/pointers** to subsystems
- **Autonomous routines are short-lived** - created in `autonomous()`, destroyed when complete
- **No ownership transfer** - subsystems remain owned by Robot throughout lifecycle

## Recommended Architecture

### Option A: Robot Reference (Recommended)

This approach provides clean access to all subsystems through the Robot instance.

#### 1. Create Autonomous Base Class

Create `include/autonomous/Autonomous.h`:

```cpp
#pragma once

class Robot; // Forward declaration

class Autonomous {
protected:
    Robot& robot;

public:
    explicit Autonomous(Robot& robot) : robot(robot) { }
    virtual ~Autonomous() = default;

    // Called once when auto starts (optional)
    virtual void init() { }

    // Main autonomous routine
    virtual void run() = 0;

    // Optional cleanup
    virtual void end() { }
};
```

#### 2. Add Robot Methods

Add to `include/Robot.h`:

```cpp
#include "autonomous/Autonomous.h" // Add to includes

class Robot {
public:
    // ... existing methods ...

    // Get subsystem by type (returns nullptr if not found)
    template<typename T>
    T* getSubsystem() {
        for (auto& subsystem : robotSubsystems) {
            if (auto* ptr = dynamic_cast<T*>(subsystem.get())) {
                return ptr;
            }
        }
        return nullptr;
    }

    // Run autonomous routine
    void runAuto(std::unique_ptr<Autonomous> autonomous);

private:
    // ... existing members ...
};
```

Add to `src/Robot.cpp`:

```cpp
void Robot::runAuto(std::unique_ptr<Autonomous> autonomous) {
    autonomous->init();
    autonomous->run();
    autonomous->end();
}
```

#### 3. Create Concrete Autonomous Routines

Create `include/autonomous/RedLeftAuto.h`:

```cpp
#pragma once

#include "autonomous/Autonomous.h"
#include "Robot.h"
#include "subsystems/Conveyer.h"
#include "subsystems/MatchLoad.h"

class RedLeftAuto : public Autonomous {
public:
    explicit RedLeftAuto(Robot& robot) : Autonomous(robot) { }

    void init() override {
        // Optional: setup before auto runs
    }

    void run() override {
        // Get subsystems (non-owning pointers)
        Conveyer* conveyer = robot.getSubsystem<Conveyer>();
        MatchLoad* matchLoad = robot.getSubsystem<MatchLoad>();

        // Your autonomous routine here
        if (conveyer) {
            // conveyer->someMethod();
        }

        if (matchLoad) {
            // matchLoad->someMethod();
        }

        // Example: chassis movements using lemlib
        // chassis.moveToPose(x, y, theta, timeout);
    }

    void end() override {
        // Optional: cleanup after auto
    }
};
```

#### 4. Usage in main.cpp

Update `src/main.cpp`:

```cpp
#include "autonomous/RedLeftAuto.h"

void autonomous() {
    // Create and run autonomous routine
    auto redLeft = std::make_unique<RedLeftAuto>(robot);
    robot.runAuto(std::move(redLeft));
}
```

---

### Option B: Explicit Subsystem Pointers

This approach makes dependencies more explicit by passing specific subsystems.

#### 1. Simple Base Class

Create `include/autonomous/Autonomous.h`:

```cpp
#pragma once

class Autonomous {
public:
    virtual ~Autonomous() = default;

    virtual void init() { }
    virtual void run() = 0;
    virtual void end() { }
};
```

#### 2. Concrete Auto with Dependencies

Create `include/autonomous/RedLeftAuto.h`:

```cpp
#pragma once

#include "autonomous/Autonomous.h"
#include "subsystems/Conveyer.h"
#include "subsystems/MatchLoad.h"

class RedLeftAuto : public Autonomous {
    Conveyer* conveyer;      // Non-owning pointer
    MatchLoad* matchLoad;    // Non-owning pointer

public:
    RedLeftAuto(Conveyer* conv, MatchLoad* ml)
        : conveyer(conv), matchLoad(ml) { }

    void run() override {
        // Use subsystems directly
        conveyer->someMethod();
        matchLoad->someMethod();
    }
};
```

#### 3. Usage with Robot Helper

Add to `include/Robot.h`:

```cpp
template<typename T>
T* getSubsystem() {
    for (auto& subsystem : robotSubsystems) {
        if (auto* ptr = dynamic_cast<T*>(subsystem.get())) {
            return ptr;
        }
    }
    return nullptr;
}

void runAuto(std::unique_ptr<Autonomous> autonomous);
```

Update `src/main.cpp`:

```cpp
void autonomous() {
    // Get subsystem pointers
    auto* conveyer = robot.getSubsystem<Conveyer>();
    auto* matchLoad = robot.getSubsystem<MatchLoad>();

    // Create auto with explicit dependencies
    auto redLeft = std::make_unique<RedLeftAuto>(conveyer, matchLoad);
    robot.runAuto(std::move(redLeft));
}
```

---

## Comparison: Option A vs Option B

| Aspect | Option A (Robot Reference) | Option B (Explicit Pointers) |
|--------|---------------------------|------------------------------|
| **Simplicity** | Simpler - just pass robot | More verbose - pass each subsystem |
| **Dependencies** | Hidden in auto implementation | Explicit in constructor |
| **Flexibility** | Easy to add subsystems | Must update constructor signature |
| **Type Safety** | Runtime cast (nullptr check needed) | Known at construction time |
| **Best For** | General use, cleaner code | When dependencies are critical |

## Recommendation

**Use Option A (Robot Reference)** - it's cleaner, more flexible, and matches your existing architecture pattern where subsystems register themselves with Robot.

## Safety Considerations

1. **Null Checks**: Always check for `nullptr` when using `getSubsystem<T>()`:
   ```cpp
   if (auto* conveyer = robot.getSubsystem<Conveyer>()) {
       conveyer->doSomething();
   }
   ```

2. **Lifetime**: Autonomous objects are destroyed after `runAuto()` completes, but subsystems persist in Robot.

3. **No Copying**: Autonomous routines should not be copied (delete copy constructor/assignment if needed).

4. **Chassis Access**: If you need lemlib chassis, make it accessible through Robot:
   ```cpp
   // In Robot.h
   lemlib::Chassis chassis; // Already exists (see Robot.cpp:10,34)
   lemlib::Chassis& getChassis() { return chassis; }
   ```

## File Structure

```
include/
  autonomous/
    Autonomous.h          # Base class
    RedLeftAuto.h         # Example concrete auto
    BlueLeftAuto.h        # Another auto routine
    ...
src/
  Robot.cpp              # Add runAuto() implementation
  main.cpp               # Call robot.runAuto()
```

## Example Complete Implementation

See the sections above for complete code examples. The pattern follows your existing Subsystem architecture:

- **Subsystem.h** defines interface, concrete subsystems implement it
- **Autonomous.h** defines interface, concrete autos implement it
- **Robot** manages lifetime via `std::unique_ptr`
- Implementations receive non-owning references/pointers

---

## Next Steps

1. Create `include/autonomous/` directory
2. Implement `Autonomous.h` base class (choose Option A or B)
3. Add `getSubsystem<T>()` and `runAuto()` to Robot
4. Create your first concrete autonomous routine
5. Update `autonomous()` function in main.cpp
