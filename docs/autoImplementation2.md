# Autonomous Routine Implementation Guide (Revised)

## Overview

This guide explains how to implement autonomous routines while respecting C++ ownership rules, **avoiding circular dependencies** where robot is passed to auto, then auto is passed back to robot.

## Core Principle: Ownership Model

- **Robot owns subsystems** via `std::unique_ptr<Subsystem>` in `robotSubsystems` vector
- **Autonomous routines receive non-owning pointers** to subsystems they need
- **Autonomous routines are short-lived** - created in `autonomous()`, destroyed when complete
- **No circular dependencies** - clean separation between robot and auto

---

## Three Clean Approaches

### Option 1: Auto Runs Itself (Simplest)

The autonomous routine doesn't need to be "owned" by robot - just create and run it.

#### 1. Create Autonomous Base Class

Create `include/autonomous/Autonomous.h`:

```cpp
#pragma once

class Autonomous {
public:
    virtual ~Autonomous() = default;

    // Optional initialization
    virtual void init() { }

    // Main autonomous routine
    virtual void run() = 0;

    // Optional cleanup
    virtual void end() { }
};
```

#### 2. Create Concrete Autonomous Routine

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
        // Your autonomous routine
        if (conveyer) {
            // conveyer->someMethod();
        }

        if (matchLoad) {
            // matchLoad->someMethod();
        }

        // Example chassis movements
        // chassis.moveToPose(x, y, theta, timeout);
    }
};
```

#### 3. Add Robot Helper Method

Add to `include/Robot.h`:

```cpp
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
```

#### 4. Usage in main.cpp

```cpp
#include "autonomous/RedLeftAuto.h"

void autonomous() {
    // Create auto with subsystem pointers
    RedLeftAuto redLeft(
        robot.getSubsystem<Conveyer>(),
        robot.getSubsystem<MatchLoad>()
    );

    // Just run it directly
    redLeft.run();
}
```

**Pros:**
- Simplest approach - no circular dependencies
- No need for `robot.runAuto()` method
- Clear ownership: auto is stack-allocated, subsystems stay with robot
- Easy to understand

**Cons:**
- Auto lives on stack (not a big issue for short-lived routines)

---

### Option 2: Robot Manages Auto Lifetime

Robot takes ownership of the auto via `unique_ptr` and runs it.

#### 1. Base Class (same as Option 1)

```cpp
#pragma once

class Autonomous {
public:
    virtual ~Autonomous() = default;
    virtual void run() = 0;
};
```

#### 2. Add Robot Method

Add to `include/Robot.h`:

```cpp
#include "autonomous/Autonomous.h"

class Robot {
public:
    // ... existing methods ...

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

private:
    // ... existing members ...
};
```

Add to `src/Robot.cpp`:

```cpp
void Robot::runAuto(std::unique_ptr<Autonomous> autonomous) {
    autonomous->run();
    // Auto is automatically destroyed here when unique_ptr goes out of scope
}
```

#### 3. Concrete Auto (same as Option 1)

```cpp
class RedLeftAuto : public Autonomous {
    Conveyer* conveyer;
    MatchLoad* matchLoad;

public:
    RedLeftAuto(Conveyer* conv, MatchLoad* ml)
        : conveyer(conv), matchLoad(ml) { }

    void run() override {
        // Your autonomous routine
    }
};
```

#### 4. Usage in main.cpp

```cpp
void autonomous() {
    // Create auto with subsystem pointers
    auto redLeft = std::make_unique<RedLeftAuto>(
        robot.getSubsystem<Conveyer>(),
        robot.getSubsystem<MatchLoad>()
    );

    // Robot takes ownership and runs it
    robot.runAuto(std::move(redLeft));
}
```

**Pros:**
- Explicit ownership via `unique_ptr`
- Consistent with how subsystems are managed
- Robot controls auto lifecycle

**Cons:**
- Slightly more verbose
- Still need to get subsystems first

---

### Option 3: Robot Creates Auto (Dependency Injection)

Robot creates the auto internally, hiding subsystem retrieval.

#### 1. Base Class (same as Options 1 & 2)

```cpp
class Autonomous {
public:
    virtual ~Autonomous() = default;
    virtual void run() = 0;
};
```

#### 2. Add Robot Template Method

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

// Create and run auto in one call
template<typename AutoType, typename... Args>
void runAuto(Args&&... args) {
    AutoType autonomous(std::forward<Args>(args)...);
    autonomous.run();
}
```

#### 3. Concrete Auto (same as Options 1 & 2)

```cpp
class RedLeftAuto : public Autonomous {
    Conveyer* conveyer;
    MatchLoad* matchLoad;

public:
    RedLeftAuto(Conveyer* conv, MatchLoad* ml)
        : conveyer(conv), matchLoad(ml) { }

    void run() override {
        // Your autonomous routine
    }
};
```

#### 4. Usage in main.cpp

```cpp
void autonomous() {
    // Robot creates and runs auto in one call
    robot.runAuto<RedLeftAuto>(
        robot.getSubsystem<Conveyer>(),
        robot.getSubsystem<MatchLoad>()
    );
}
```

**Pros:**
- Single line to create and run
- Template-based, type-safe
- Robot manages lifecycle

**Cons:**
- More complex template syntax
- Still need to call `getSubsystem` explicitly

---

## Comparison

| Aspect | Option 1: Auto Runs Itself | Option 2: Robot Manages | Option 3: Template Injection |
|--------|---------------------------|------------------------|------------------------------|
| **Simplicity** | Simplest | Medium | More complex |
| **Verbosity** | Low | Medium | Low |
| **Ownership** | Stack-allocated | unique_ptr | Stack-allocated |
| **Circular Deps** | None | None | None |
| **Best For** | General use | Explicit lifetime control | Advanced users |

---

## Recommendation

**Use Option 1 (Auto Runs Itself)** - it's the simplest, has no circular dependencies, and is perfectly fine for autonomous routines that run once and exit.

```cpp
// main.cpp
void autonomous() {
    RedLeftAuto redLeft(
        robot.getSubsystem<Conveyer>(),
        robot.getSubsystem<MatchLoad>()
    );
    redLeft.run();
}
```

If you want more control over lifetime or consistency with subsystem pattern, use **Option 2**.

---

## Accessing Chassis

Since `lemlib::Chassis` is a member of Robot (not in the subsystems vector), you have two options:

**Option A: Add getter to Robot**

```cpp
// Robot.h
class Robot {
public:
    lemlib::Chassis chassis; // Already exists as public member
    // OR make it private and add:
    // lemlib::Chassis& getChassis() { return chassis; }
};

// Usage in auto
class RedLeftAuto : public Autonomous {
    lemlib::Chassis& chassis;

public:
    RedLeftAuto(lemlib::Chassis& chassis) : chassis(chassis) { }

    void run() override {
        chassis.moveToPose(0, 0, 0, 2000);
    }
};

// main.cpp
void autonomous() {
    RedLeftAuto redLeft(robot.chassis); // Assuming chassis is public
    redLeft.run();
}
```

**Option B: Pass to auto constructor**

```cpp
void autonomous() {
    RedLeftAuto redLeft(
        robot.chassis,
        robot.getSubsystem<Conveyer>(),
        robot.getSubsystem<MatchLoad>()
    );
    redLeft.run();
}
```

---

## Safety Considerations

1. **Null Checks**: Always check for `nullptr` when using `getSubsystem<T>()`:
   ```cpp
   void run() override {
       if (conveyer) {
           conveyer->doSomething();
       } else {
           // Handle missing subsystem
           pros::lcd::print(0, "ERROR: Conveyer not found!");
       }
   }
   ```

2. **Lifetime**: Autonomous objects are destroyed after `run()` completes, but subsystems persist in Robot.

3. **Constructor Validation**: Consider validating subsystems in constructor:
   ```cpp
   RedLeftAuto(Conveyer* conv, MatchLoad* ml)
       : conveyer(conv), matchLoad(ml) {
       if (!conveyer || !matchLoad) {
           pros::lcd::print(0, "ERROR: Missing subsystems!");
       }
   }
   ```

---

## File Structure

```
include/
  autonomous/
    Autonomous.h          # Base class
    RedLeftAuto.h         # Example concrete auto
    RedRightAuto.h        # Another auto routine
    BlueLeftAuto.h        # Another auto routine
    ...
src/
  Robot.cpp              # Add getSubsystem<T>() if not inline
  main.cpp               # Create and run autos
```

---

## Complete Example

### Autonomous.h
```cpp
#pragma once

class Autonomous {
public:
    virtual ~Autonomous() = default;
    virtual void run() = 0;
};
```

### RedLeftAuto.h
```cpp
#pragma once

#include "autonomous/Autonomous.h"
#include "subsystems/Conveyer.h"
#include "subsystems/MatchLoad.h"
#include "lemlib/api.hpp"

class RedLeftAuto : public Autonomous {
    lemlib::Chassis& chassis;
    Conveyer* conveyer;
    MatchLoad* matchLoad;

public:
    RedLeftAuto(lemlib::Chassis& chassis, Conveyer* conv, MatchLoad* ml)
        : chassis(chassis), conveyer(conv), matchLoad(ml) { }

    void run() override {
        // Example autonomous routine
        chassis.moveToPose(24, 24, 90, 2000);

        if (conveyer) {
            // conveyer->intake();
        }

        chassis.waitUntilDone();

        if (matchLoad) {
            // matchLoad->score();
        }
    }
};
```

### Robot.h additions
```cpp
// Add this method
template<typename T>
T* getSubsystem() {
    for (auto& subsystem : robotSubsystems) {
        if (auto* ptr = dynamic_cast<T*>(subsystem.get())) {
            return ptr;
        }
    }
    return nullptr;
}
```

### main.cpp
```cpp
#include "autonomous/RedLeftAuto.h"

void autonomous() {
    RedLeftAuto redLeft(
        robot.chassis,
        robot.getSubsystem<Conveyer>(),
        robot.getSubsystem<MatchLoad>()
    );

    redLeft.run();
}
```

---

## Next Steps

1. Create `include/autonomous/` directory
2. Implement `Autonomous.h` base class
3. Add `getSubsystem<T>()` to Robot.h
4. Create your first concrete autonomous routine (e.g., RedLeftAuto)
5. Update `autonomous()` function in main.cpp to create and run it

**No circular dependencies, clean separation of concerns!**
