#include "subsystems/MidDescore.h"
#include "commands/ButtonCommand.h"
#include "pros/adi.hpp"
#include <memory>

MidDescore::MidDescore() : piston(DEFAULT_PISTON_PORT, false)
{
}

MidDescore::MidDescore(int pistonPort) : piston(pistonPort, false)
{
}

void MidDescore::registerHandlers(EventLoop &eventLoop)
{
  eventLoop.registerCommand(
    std::unique_ptr<Command>(new ButtonCommand(
      eventLoop.controller,
      pros::E_CONTROLLER_DIGITAL_Y,
      [this]() { toggle(); }
    ))
  );
}

void MidDescore::setup()
{
}

void MidDescore::extend() {
  // robot->getSubsystem<MatchLoad>()->retract();
  // pros::Task extendMatchLoadTask([this]() {
  //   pros::delay(250);
  //   piston.extend();
  // });
    piston.extend();
}

void MidDescore::retract() { piston.retract(); }

void MidDescore::toggle() {
  if (!piston.is_extended()) {
    extend();
  } else {
    retract();
  }
}

// void MidDescore::setRobot(Robot *md) { robot = md; }