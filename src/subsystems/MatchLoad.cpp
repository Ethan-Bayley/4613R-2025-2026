#include "subsystems/MatchLoad.h"
#include "commands/ButtonCommand.h"

MatchLoad::MatchLoad() : piston1(DEFAULT_PISTON_1_PORT, false),
                         piston2(DEFAULT_PISTON_2_PORT, true)
{
}

MatchLoad::MatchLoad(int pistonPort1, int pistonPort2) : piston1(pistonPort1, false),
                                                         piston2(pistonPort2, true)
{
}

void MatchLoad::registerHandlers(EventLoop &eventLoop)
{
  eventLoop.registerCommand(
    std::make_shared<ButtonCommand>(
      eventLoop.controller,
      pros::E_CONTROLLER_DIGITAL_B,
      [this]() { extend(); },
      [this]() { retract(); }
    )
  );
}

void MatchLoad::setup()
{
}

void MatchLoad::update()
{
}

void MatchLoad::extend()
{
  piston1.extend();
  piston2.retract();
}

void MatchLoad::retract()
{
  piston1.retract();
  piston2.extend();
}
