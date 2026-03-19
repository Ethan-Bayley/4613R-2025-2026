#include "subsystems/MatchLoad.h"
#include "commands/ButtonCommand.h"
#include <memory>

Wing::Wing() : piston1(DEFAULT_PISTON_1_PORT, false),
                         isRetracted(false)
{
}

Wing::Wing(int pistonPort1, int pistonPort2) : piston1(pistonPort1, false),
{
}

void Wing::registerHandlers(EventLoop &eventLoop)
{
  eventLoop.registerCommand(
    std::unique_ptr<Command>(new ButtonCommand(
      eventLoop.controller,
      pros::E_CONTROLLER_DIGITAL_DOWN,
      [this]() { toggle(); }
    ))
  );
}

void Wing::setup()
{
  retract();
}

void Wing::extend()
{
  piston1.extend();
  piston2.retract();
  isRetracted = false;
}

void Wing::toggle()
{
  if (isRetracted) {
    extend();
  } else {
    retract();
  }

}

void Wing::retract()
{
  piston1.retract();
  isRetracted = true;
}
