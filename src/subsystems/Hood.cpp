#include "subsystems/Hood.h"
#include "commands/ButtonCommand.h"
#include "pros/adi.hpp"
#include <memory>

Hood::Hood() : piston(DEFAULT_PISTON_PORT, false)
{
}

Hood::Hood(int pistonPort) : piston(pistonPort, false)
{
}
// see passthrough.cpp for changing polarity thing

void Hood::registerHandlers(EventLoop &eventLoop)
{
  // eventLoop.registerCommand(
  //   std::unique_ptr<Command>(new ButtonCommand(
  //     eventLoop.controller,
  //     pros::E_CONTROLLER_DIGITAL_LEFT,
  //     [this]() { toggle(); }
  //   ))
  // );
}

void Hood::setup()
{
  retract();
}

void Hood::toggle()
{
  piston.toggle();
}

void Hood::extend()
{
  piston.retract(); // inverted hardware
}

void Hood::retract()
{
  piston.extend(); // inverted hardware
}