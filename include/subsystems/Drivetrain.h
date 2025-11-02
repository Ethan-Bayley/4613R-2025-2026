#pragma once

#include "commands/EventLoop.h"
#include "Subsystem.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

class Drivetrain : public Subsystem
{
public:
  Drivetrain(lemlib::Chassis &chassis);

  void setup() override;
  void update() override;

  void registerHandlers(EventLoop &eventLoop) override;

private:
  lemlib::Chassis &chassis;
  int leftY;
  int leftX;
};