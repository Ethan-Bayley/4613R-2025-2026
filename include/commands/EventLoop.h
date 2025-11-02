#pragma once

#include "ButtonCommand.h"
#include "JoystickCommand.h"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include <functional>

class EventLoop {
public:
  EventLoop(pros::Controller &controller);

  void update();

  template<typename T>
  void registerButton(
    pros::controller_digital_e_t button,
    T* instance,
    void (T::*onPress)(),
    void (T::*onRelease)() = nullptr)
  {
    std::function<void()> press = onPress ? [instance, onPress]() { (instance->*onPress)(); } : std::function<void()>();
    std::function<void()> release = onRelease ? [instance, onRelease]() { (instance->*onRelease)(); } : std::function<void()>();
    Button newButton {button,press, release, false};
    buttonCommand.registerButtonCommand(newButton);
  }

  void registerJoystick(
    pros::controller_analog_e_t axis, 
    std::function<void(int)> onChange);
  void clear();

  int getJoystickValue(pros::controller_analog_e_t axis);

private:
  pros::Controller &controller;
  ButtonCommand buttonCommand;
  JoystickCommand joystickCommand;
};