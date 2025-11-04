#pragma once

#include "commands/EventLoop.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "subsystems/Subsystem.h"
#include <vector>
#include <memory>

class Robot {
  public:
    Robot(pros::Controller &controller);
    ~Robot() = default;

    void setup();

    void addSubsystem(std::unique_ptr<Subsystem> subsystem);
    void registerCommands();
    void update();

  private:
    pros::Controller &controller;
    EventLoop eventLoop;
    std::vector<std::unique_ptr<Subsystem>> robotSubsystems;
};
