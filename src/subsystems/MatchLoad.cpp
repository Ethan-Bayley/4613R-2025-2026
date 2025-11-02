#include "subsystems/MatchLoad.h"

MatchLoad::MatchLoad() : 
  piston1(DEFAULT_PISTON_1_PORT, false), 
  piston2(DEFAULT_PISTON_2_PORT, true)
{

}

MatchLoad::MatchLoad(int pistonPort1, int pistonPort2) : 
  piston1(pistonPort1, false), 
  piston2(pistonPort2, true)
{

}

void MatchLoad::registerButtonHandlers(EventLoop &eventLoop) {
  eventLoop.registerButton(pros::E_CONTROLLER_DIGITAL_B, this, &MatchLoad::extend, &MatchLoad::retract);
}

void MatchLoad::registerJoystickHandlers(EventLoop &eventLoop) {

}

void MatchLoad::setup() {

}

void MatchLoad::update() {

}

void MatchLoad::extend() {
  piston1.extend();
  piston2.retract();
}

void MatchLoad::retract() {
  piston1.retract();
  piston2.extend();
}

