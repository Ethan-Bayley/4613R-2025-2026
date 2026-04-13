#include "subsystems/Hood.h"
#include "commands/ButtonCommand.h"
#include "pros/adi.hpp"
#include <memory>

Hood::Hood() : piston(DEFAULT_PISTON_PORT, false, false),
                         isRetracted(false)
{
}

Hood::Hood(int pistonPort) : piston(pistonPort, false, false),
                                                         isRetracted(false)
{
}

void Hood::registerHandlers(EventLoop &eventLoop)
{
  eventLoop.registerCommand(
    std::unique_ptr<Command>(new ButtonCommand(
      eventLoop.controller,
      pros::E_CONTROLLER_DIGITAL_LEFT,
      [this]() { toggle(); }
    ))
  );
}

void Hood::setup()
{
  retract();
}

void Hood::extend()
{
  piston.extend();
  isRetracted = false;
}

void Hood::toggle()
{
  if (isRetracted) {
    extend();
  } else {
    retract();
  }

}

void Hood::retract()
{
  piston.retract();
  isRetracted = true;
}