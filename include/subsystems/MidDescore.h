#pragma once 

#include "Subsystems.h"
#include "commands/EventLoop.h"
#include "pros/adi.hpp"
#include "Robot.h"

class MidDescore : public Subsystems {
  public:
    MidDescore();
    MidDescore(int pistonPort);

    void setup() override;
    void registerHandlers(EventLoop &eventLoop) override;

    void extend();
    void retract();
    void toggle();
  // void setRobot(Robot *md);

  private:
  static constexpr int DEFAULT_PISTON_PORT = 'H';
  // Robot *robot = nullptr;
  

  pros::adi::Pneumatics piston;
};