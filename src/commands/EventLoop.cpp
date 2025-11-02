#include "commands/EventLoop.h"
#include <cstdio>

EventLoop::EventLoop(pros::Controller &controller) 
: controller(controller), buttonCommand(controller), joystickCommand(controller) {}

void EventLoop::update() {
  buttonCommand.execute();
  joystickCommand.execute();
}


void EventLoop::registerJoystick(pros::controller_analog_e_t axis, std::function<void(int value)> onChange) {
  Joystick newJoystick {axis, onChange, 5};
  joystickCommand.registerJoystickCommand(newJoystick);
}

void EventLoop::clear() {
  buttonCommand.clear();
  joystickCommand.clear();
}

int EventLoop::getJoystickValue(pros::controller_analog_e_t axis) {
  return joystickCommand.getValue(axis);
}
