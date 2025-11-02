#include "Robot.h"
#include "commands/EventLoop.h"
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

void Robot::registerCommands() {
  for (auto &subsystem : robotSubsystems) {
    subsystem->registerHandlers(eventLoop);
  }
}

void Robot::update() {
  for (auto &subsystem : robotSubsystems) {
    eventLoop.update();
    subsystem->update();
  }
}