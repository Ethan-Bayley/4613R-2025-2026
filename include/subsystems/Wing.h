#pragma once 

#include "subsystems/Subsystems.h"
#include "commands/EventLoop.h"
#include "pros/adi.hpp"

class Wing : public Subsystems {
  public:
    Wing();
    Wing(int pistonPort);

    void setup() override;
    void registerHandlers(EventLoop &eventLoop) override;

    void toggle();
    void extend();
    void retract();

  private:
  static constexpr int DEFAULT_PISTON_PORT = 'E';

  pros::adi::Pneumatics piston;
};