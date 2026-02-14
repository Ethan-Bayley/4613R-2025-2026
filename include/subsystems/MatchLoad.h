#pragma once 

#include "commands/EventLoop.h"
#include "Subsystem.h"
#include "pros/adi.hpp"

class MatchLoad : public Subsystem {
  public:
    MatchLoad();
    MatchLoad(int pistonPort1, int pistonPort2);

    void setup() override;
    void registerHandlers(EventLoop &eventLoop) override;

    void extend();
    void retract();
    void toggle();

  private:
  static constexpr int DEFAULT_PISTON_1_PORT = 'F';
  static constexpr int DEFAULT_PISTON_2_PORT = 'H';

  bool isRetracted;

  pros::adi::Pneumatics piston1;
  pros::adi::Pneumatics piston2;
};