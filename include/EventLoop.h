#pragma once

#include <functional>
#include <vector>

#include "lemlib/api.hpp"
#include "pros/misc.hpp"

struct ButtonBinding {
  pros::controller_digital_e_t button;
  std::function<void()> onPress;
  std::function<void()> onRelease;
  bool previousState;
};

class EventLoop {
public:
  EventLoop(pros::Controller &controller);

  void registerBinding(
      pros::controller_digital_e_t button,
      std::function<void()> onPress,
      std::function<void()> onRelease = nullptr);

  void update();
  void clearBindings();

private:
  pros::Controller &controller;
  std::vector<ButtonBinding> bindings;
};