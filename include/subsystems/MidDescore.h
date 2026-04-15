#pragma once 

#include "subsystems/Subsystems.h"
#include "commands/EventLoop.h"
#include "pros/adi.hpp"

class MidDescore : public Subsystems {
  public:
    MidDescore();
    MidDescore(int pistonPort);

    void setup() override;
    void registerHandlers(EventLoop &eventLoop) override;

    void extend();
    void retract();
    void toggle();

  private:
  static constexpr int DEFAULT_PISTON_PORT = 'H';

  pros::adi::Pneumatics piston;
};