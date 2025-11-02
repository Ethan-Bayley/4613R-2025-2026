#pragma once
#include "commands/Command.h"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include <functional>

struct Joystick {
  pros::controller_analog_e_t axis;
  std::function<void(int value)> onChange;
  int deadzone;
};

class JoystickCommand: public Command {
  public:
  JoystickCommand(pros::Controller &controller);
  void registerJoystickCommand(Joystick j);
  void execute() override;
  void clear() override;
  
  int getValue(pros::controller_analog_e_t axis); // Does not account for deadzones

  private: 
    pros::Controller &controller;
    std::vector<Joystick> joystickCommands;
};