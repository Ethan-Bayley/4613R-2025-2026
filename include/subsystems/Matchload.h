#pragma once

#include "Subsystems.h"
#include "commands/EventLoop.h"
#include "pros/adi.hpp"
#include "Robot.h"

class MatchLoad : public Subsystems {
public:
  MatchLoad();
  MatchLoad(int DEFAULT_PISTON_PORT);

  void setup() override;
  void registerHandlers(EventLoop &eventLoop) override;

  void extend();
  void retract();
  void toggle();
  // void setRobot(Robot *md);

private:
  static constexpr int DEFAULT_PISTON_PORT = 'G';
  // Robot *robot = nullptr;

  pros::adi::Pneumatics piston;
};