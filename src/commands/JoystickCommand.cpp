#include "commands/JoystickCommand.h"
#include "pros/misc.hpp"
#include <cstdio>

JoystickCommand::JoystickCommand(
  pros::Controller &controller,
  pros::controller_analog_e_t axis,
  std::function<void(int)> onChange
) : Command("JoystickCommand", nullptr),
    controller(controller),
    axis(axis),
    onChange(onChange)
{
  executeFunc = [this]() {
    int currentValue = this->controller.get_analog(this->axis);

    if (this->onChange) {
      this->onChange(currentValue);
    }
  };
}
