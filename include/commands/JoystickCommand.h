#pragma once
#include "commands/Command.h"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include <functional>

class JoystickCommand : public Command {
public:
  JoystickCommand(
    pros::Controller &controller,
    pros::controller_analog_e_t axis,
    std::function<void(int)> onChange
  );

private:
  pros::Controller &controller;
  pros::controller_analog_e_t axis;
  std::function<void(int)> onChange;
};