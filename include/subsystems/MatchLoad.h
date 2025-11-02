#pragma once 

#include "commands/EventLoop.h"
#include "Subsystem.h"
#include "pros/adi.hpp"

class MatchLoad : public Subsystem {
  public: 
    MatchLoad();
    MatchLoad(int pistonPort1, int pistonPort2);

    void setup() override;
    void registerButtonHandlers(EventLoop &eventLoop) override;
    void registerJoystickHandlers(EventLoop &eventLoop) override;
    void update() override;

    void extend();
    void retract();

  private:
  static constexpr int DEFAULT_PISTON_1_PORT = 'F';
  static constexpr int DEFAULT_PISTON_2_PORT = 'B';

  pros::adi::Pneumatics piston1;
  pros::adi::Pneumatics piston2;
};