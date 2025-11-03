#pragma once
#include "commands/Command.h"
#include "pros/misc.hpp"
#include <functional>

class ButtonCommand : public Command {
public:
  ButtonCommand(
    pros::Controller &controller,
    pros::controller_digital_e_t button,
    std::function<void()> onPress,
    std::function<void()> onRelease = nullptr
  );

private:
  pros::Controller &controller;
  pros::controller_digital_e_t button;
  std::function<void()> onPress;
  std::function<void()> onRelease;
  bool previousState;
};