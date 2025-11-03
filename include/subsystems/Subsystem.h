#pragma once

#include "commands/EventLoop.h"

class Subsystem {
public:
  virtual ~Subsystem() = default;

  // Lifecycle functions are required
  virtual void setup() = 0;
  virtual void update() = 0;

  // By default, we aren't required to register handlers
  virtual void registerHandlers(EventLoop &eventLoop) { };
};

