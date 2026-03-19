#include "subsystems/MatchLoad.h"
#include "commands/ButtonCommand.h"
#include <memory>

MatchLoad::MatchLoad() : piston1(DEFAULT_PISTON_1_PORT, false),
                         isRetracted(false)
{
}

MatchLoad::MatchLoad(int pistonPort1, int pistonPort2) : piston1(pistonPort1, false),
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
  piston1.extend();
  piston2.retract();
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
  piston1.retract();
  piston2.extend();
  isRetracted = true;
}
