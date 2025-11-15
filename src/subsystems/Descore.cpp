#include "subsystems/Descore.h"
#include "commands/ButtonCommand.h"
#include <memory>

Descore::Descore() : piston(DEFAULT_PISTON_PORT, false)
{
}

Descore::Descore(int pistonPort) : piston(pistonPort, false)                          
{
}

void Descore::setup() {
  piston.retract();
}

void Descore::registerHandlers(EventLoop &eventLoop)
{
  eventLoop.registerCommand(
    std::unique_ptr<Command>(new ButtonCommand(
      eventLoop.controller,
      pros::E_CONTROLLER_DIGITAL_B,
      [this]() { toggle(); }
    ))
  );
}

void Descore::toggle()
{
  if (piston.is_extended()) {
    piston.retract();
  } else {
    piston.extend();
  }

}

