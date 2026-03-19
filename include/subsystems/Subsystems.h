#pragma once

#include "commands/EventLoop.h"

class Subsystems {
public:
  virtual ~Subsystems() = default;

  // Lifecycle functions are required
  virtual void setup() = 0;

  // By default, we aren't required to register handlers
  virtual void registerHandlers(EventLoop &eventLoop) { };
};