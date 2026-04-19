#include "MatchLoad.h"
#include "commands/ButtonCommand.h"
#include "pros/rtos.hpp"
#include <memory>


MatchLoad::MatchLoad() : piston(DEFAULT_PISTON_PORT, false) {}

MatchLoad::MatchLoad(int pistonPort) : piston(pistonPort, false) {}

void MatchLoad::registerHandlers(EventLoop &eventLoop) {
  eventLoop.registerCommand(std::unique_ptr<Command>(
      new ButtonCommand(eventLoop.controller, pros::E_CONTROLLER_DIGITAL_B,
                        [this]() { toggle(); })));
}

void MatchLoad::setup() {}

void MatchLoad::extend() {
  // robot->getSubsystem<MidDescore>()->extend();
  // pros::Task extendMatchLoadTask([this]() {
  //   pros::delay(250);
  //   piston.extend();
  // });
    piston.extend();
}

void MatchLoad::retract() { piston.retract(); }

void MatchLoad::toggle() {
  piston.toggle();
}

// void MatchLoad::setRobot(Robot *md) { robot = md; }