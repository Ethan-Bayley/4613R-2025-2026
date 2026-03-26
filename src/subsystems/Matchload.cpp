#include "subsystems/Matchload.h"
#include "commands/ButtonCommand.h"
#include <memory>

MatchLoad::MatchLoad() : piston(DEFAULT_PISTON_PORT, false),
                         isRetracted(false)
{
}

MatchLoad::MatchLoad(int pistonPort) : piston(pistonPort, false),
                                                         isRetracted(false)
{
}

void MatchLoad::registerHandlers(EventLoop &eventLoop)
{
  eventLoop.registerCommand(
    std::unique_ptr<Command>(new ButtonCommand(
      eventLoop.controller,
      pros::E_CONTROLLER_DIGITAL_DOWN,
      [this]() { toggle(); }
    ))
  );
}

void MatchLoad::setup()
{
  retract();
}

void MatchLoad::extend()
{
  piston.extend();
  isRetracted = false;
}

void MatchLoad::toggle()
{
  if (isRetracted) {
    extend();
  } else {
    retract();
  }

}

void MatchLoad::retract()
{
  piston.retract();
  isRetracted = true;
}