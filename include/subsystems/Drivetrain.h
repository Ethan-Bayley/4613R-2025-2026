#pragma once 

#include "commands/EventLoop.h"
#include "Subsystem.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

class Drivetrain : public Subsystem {
  public: 
    Drivetrain(lemlib::Chassis &chassis);

    void setup() override;
    void registerButtonHandlers(EventLoop &eventLoop) override;
    void registerJoystickHandlers(EventLoop &eventLoop) override;
    void update() override;

  private:
    lemlib::Chassis &chassis;
    int leftY;
    int leftX;
};