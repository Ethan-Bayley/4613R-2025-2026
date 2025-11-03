#pragma once

#include "Command.h"
#include "pros/misc.hpp"
#include <memory>
#include <vector>

class EventLoop {
public:
  EventLoop(pros::Controller &controller);

  void update();
  void registerCommand(std::shared_ptr<Command> command);
  void clear();

  pros::Controller &controller;

private:
  std::vector<std::shared_ptr<Command>> commands;
};