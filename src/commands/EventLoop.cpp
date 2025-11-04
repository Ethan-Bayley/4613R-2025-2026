#include "commands/EventLoop.h"

EventLoop::EventLoop(pros::Controller &controller)
  : controller(controller) {}

void EventLoop::update() {
  for (auto& command : commands) {
    command->execute();
  }
}

void EventLoop::registerCommand(std::unique_ptr<Command> command) {
  commands.push_back(std::move(command));
}

void EventLoop::clear() {
  commands.clear();
}
