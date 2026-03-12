#pragma once

#include "commands/EventLoop.h"

class Subsystems {
public:
    virtual ~Subsystems() = default;

    virtual void setup() = 0;
    virtual void registerHandlers(EventLoop &eventLoop) { }
    virtual void update() { }  // <-- add this
};