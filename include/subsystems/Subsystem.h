#pragma once

#include "EventLoop.h"

class Subsystem {
public:
  virtual ~Subsystem() = default;

  virtual void setup() = 0;
  virtual void registerButtonHandlers(EventLoop &eventLoop) = 0;

};

