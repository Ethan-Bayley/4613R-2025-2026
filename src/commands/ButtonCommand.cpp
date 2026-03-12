#include "commands/ButtonCommand.h"
#include <cstdio>
#include "main.h"

ButtonCommand::ButtonCommand(
  pros::Controller &controller,
  pros::controller_digital_e_t button,
  std::function<void()> onPress,
  std::function<void()> onRelease
) : Command("ButtonCommand", nullptr),
    controller(controller),
    button(button),
    onPress(onPress),
    onRelease(onRelease),
    previousState(false)
{
  executeFunc = [this]() {
    bool currentState = this->controller.get_digital(this->button);

    // If was pressed, and bindings last state was 'not pressed' handle press
    if (currentState && !this->previousState && this->onPress) {
      this->onPress();
    }

    // If was not pressed, and bindings last state was 'pressed' handle release
    if (!currentState && this->previousState && this->onRelease) {
      this->onRelease();
    }

    this->previousState = currentState;
  };
}

