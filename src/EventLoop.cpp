#include "EventLoop.h"

EventLoop::EventLoop(pros::Controller &controller) : controller(controller) { }

void EventLoop::registerBinding(
  pros::controller_digital_e_t button, 
  std::function<void ()> onPress,
  std::function<void ()> onRelease
) {
  bindings.emplace_back(button, onPress, onRelease);
};

void EventLoop::update() {
  for (auto &binding : bindings) {
    bool currentState = controller.get_digital(binding.button);

    // If was pressed, and bindings last state was 'not pressed' handle press
    if (currentState && !binding.previousState && binding.onPress) {
      binding.onPress();
    }

    // If was not pressed, and bindings last state was 'pressed' handle release
    if (!currentState && binding.previousState && binding.onRelease) {
      binding.onRelease();
    }

  }
}

void EventLoop::clearBindings() {
  bindings.clear();
}
