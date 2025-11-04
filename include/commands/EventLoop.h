#pragma once

#include "Command.h"
#include "ButtonCommand.h"
#include "PeriodicCommand.h"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include <functional>
#include <vector>
#include <memory>

class EventLoop {
public:
  EventLoop(pros::Controller &controller);
  ~EventLoop() = default;

  void update();
  void registerCommand(std::unique_ptr<Command> command);
  void clear();

  pros::Controller &controller;

private:
  std::vector<std::unique_ptr<Command>> commands;
};