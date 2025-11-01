#pragma once

#include "EventLoop.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "subsystems/Subsystem.h"
#include <memory>

class Robot {
  public:
    Robot(EventLoop &eventLoop);
    
    void setup();

    void addSubsystem(std::unique_ptr<Subsystem> subsystem);
    void initSubsystems();
    void registerButtonHandlers();
    void update();

  private:
    EventLoop &eventLoop;
    std::vector<std::unique_ptr<Subsystem>> robotSubsystems;
};
