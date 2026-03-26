#include "subsystems/Wing.h"
#include "commands/ButtonCommand.h"
#include <memory>

Wing::Wing() : piston(DEFAULT_PISTON_PORT, false)
{
}

Wing::Wing(int pistonPort) : piston(pistonPort, false)                          
{
}

void Wing::setup() {
  piston.retract();
}

void Wing::registerHandlers(EventLoop &eventLoop)
{
  eventLoop.registerCommand(
    std::unique_ptr<Command>(new ButtonCommand(
      eventLoop.controller,
      pros::E_CONTROLLER_DIGITAL_B,
      [this]() { toggle(); }
    ))
  );
}

void Wing::toggle()
{
  if (piston.is_extended()) {
    piston.retract();
  } else {
    piston.extend();
  }

}