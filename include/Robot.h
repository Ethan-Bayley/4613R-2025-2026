#pragma once

#include "commands/EventLoop.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "subsystems/Subsystems.h"
#include <vector>
#include <memory>
#include <utility>

class Robot {
  public:
    Robot(pros::Controller &controller);
    ~Robot() = default;

    void setup();

    void addSubsystem(std::unique_ptr<Subsystems> subsystem);
    void registerCommands();
    void update();

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

    // Create and run auto in one call (Option 3: Dependency Injection)
    template<typename AutoType, typename... Args>
    void runAuto(Args&&... args) {
        AutoType autonomous(std::forward<Args>(args)...);
        autonomous.run();
    }

  private:
    pros::Controller &controller;
    EventLoop eventLoop;
    std::vector<std::unique_ptr<Subsystems>> robotSubsystems;
};