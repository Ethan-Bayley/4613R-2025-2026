#include "commands/JoystickCommand.h"
#include "pros/misc.hpp"
#include <cstdio>

JoystickCommand::JoystickCommand(pros::Controller &controller) : controller(controller) {
  
};

void JoystickCommand::registerJoystickCommand(Joystick j) {
  joystickCommands.emplace_back(j.axis, j.onChange);
}

void JoystickCommand::execute() {
  for (auto j : joystickCommands) {
    int currentValue = controller.get_analog(j.axis);
    
    // Apply deadzone
    if (abs(currentValue) < j.deadzone) {
      currentValue = 0;
    }
    
    if (j.onChange) {
      j.onChange(currentValue);
    }
  }
};

int JoystickCommand::getValue(pros::controller_analog_e_t axis) {
  return controller.get_analog(axis);
}

void JoystickCommand::clear() {
  joystickCommands.clear();
}
