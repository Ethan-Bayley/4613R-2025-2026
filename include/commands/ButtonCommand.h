#pragma once
#include "commands/Command.h"
#include "pros/misc.hpp"
#include <functional>

struct Button {
  pros::controller_digital_e_t button;
  std::function<void()> onPress;
  std::function<void()> onRelease;
  bool previousState;
};

class ButtonCommand : public Command {
public:
  ButtonCommand(pros::Controller &controller);
  void registerButtonCommand(Button button);
  void execute() override;
  void clear() override;

private:
  pros::Controller &controller;
  std::vector<Button> buttonCommands;
};