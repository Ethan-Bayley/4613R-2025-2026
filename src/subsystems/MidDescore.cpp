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
      pros::E_CONTROLLER_DIGITAL_UP,
      [this]() { toggle(); }
    ))
  );
}

void MidDescore::setup()
{
}

void MidDescore::toggle()
{
  piston.toggle();
}

void MidDescore::extend()
{
  piston.retract(); // inverted hardware
}

void MidDescore::retract()
{
  piston.extend(); // inverted hardware
}