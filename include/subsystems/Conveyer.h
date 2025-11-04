#pragma once

#include "commands/EventLoop.h"
#include "Subsystem.h"
#include "pros/adi.hpp"
#include "pros/motors.hpp"

class Conveyer : public Subsystem
{
public:
  Conveyer();
  Conveyer(
      int bottomMotorPort, int topMotorPort,
      int pistonPort1, int pistonPort2);

  void setup() override;

  void registerHandlers(EventLoop &eventLoop) override;

  void intake();
  void outtake();
  void stow();
  void stop();
  void scoreLong();
  void scoreMiddle();

private:
  static constexpr int DEFAULT_BOTTOM_PORT = 7;
  static constexpr int DEFAULT_TOP_PORT = 20;
  static constexpr int DEFAULT_PISTON_1_PORT = 'A';
  static constexpr int DEFAULT_PISTON_2_PORT = 'D';

  pros::Motor topMotor;
  pros::Motor bottomMotor;
  pros::adi::Pneumatics piston1;
  pros::adi::Pneumatics piston2;
};