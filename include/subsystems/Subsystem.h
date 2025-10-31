#pragma once

#include "EventLoop.h"

class Subsystem {

  virtual ~Subsystem() = default;

  virtual void setup() = 0;

  virtual void registerButtonHandlers(EventLoop &eventLoop) = 0;

};

