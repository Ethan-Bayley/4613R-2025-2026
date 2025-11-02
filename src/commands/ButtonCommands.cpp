#include "commands/ButtonCommand.h"
#include <cstdio>

ButtonCommand::ButtonCommand(
  pros::Controller &controller) : controller(controller) {

};

void ButtonCommand::registerButtonCommand(Button button) {
  buttonCommands.emplace_back(button.button, button.onPress, button.onRelease);
}

void ButtonCommand::execute() {
  for (auto &b : buttonCommands) {
    bool currentState = controller.get_digital(b.button);

    // If was pressed, and bindings last state was 'not pressed' handle press
    if (currentState && !b.previousState && b.onPress) {
      b.onPress();
    }

    // If was not pressed, and bindings last state was 'pressed' handle release
    if (!currentState && b.previousState && b.onRelease) {

      b.onRelease();
    }

    b.previousState = currentState;
  }
  
};

void ButtonCommand::clear() {
  buttonCommands.clear();
}
