#include "Robot.h"
#include "EventLoop.h"
#include "liblvgl/llemu.hpp"
#include "subsystems/Subsystem.h"

Robot::Robot(EventLoop &eventloop) : eventLoop(eventloop) {};

void Robot::setup() {
  for (auto &subsystem : robotSubsystems) {
    subsystem->setup();
  }
}

void Robot::addSubsystem(std::unique_ptr<Subsystem> subsystem) {
  robotSubsystems.emplace_back(std::move(subsystem));
}

void Robot::registerButtonHandlers() {
  for (auto &subsystem : robotSubsystems) {
    subsystem->registerButtonHandlers(eventLoop);
  }
}

void Robot::update() {
  pros::lcd::set_text(3, "update");
  for (auto &subsystem : robotSubsystems) {
    eventLoop.update();
  }
}