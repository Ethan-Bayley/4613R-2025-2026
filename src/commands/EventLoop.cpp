#include "commands/EventLoop.h"
#include <memory>

EventLoop::EventLoop(pros::Controller &controller)
  : controller(controller) {}

void EventLoop::update() {
  for (auto &command : commands) {
    command->execute();
  }
}

void EventLoop::registerCommand(std::shared_ptr<Command> command) {
  commands.push_back(command);
}

void EventLoop::clear() {
  commands.clear();
}
 