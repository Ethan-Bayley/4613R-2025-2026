#pragma once

#include "commands/EventLoop.h"

class Subsystem {
public:
  virtual ~Subsystem() = default;

  virtual void setup() = 0;
  virtual void registerButtonHandlers(EventLoop &eventLoop) = 0;
  virtual void registerJoystickHandlers(EventLoop &eventLoop) = 0;
  virtual void update() = 0;

};

