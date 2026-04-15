#pragma once 

#include "commands/EventLoop.h"
#include "Subsystems.h"
#include "pros/adi.hpp"

class Hood : public Subsystems {
  public:
    Hood();
    Hood(int DEFAULT_PISTON_PORT);

    void setup() override;
    void registerHandlers(EventLoop &eventLoop) override;

    void extend();
    void retract();
    void toggle();

  private:
  static constexpr int DEFAULT_PISTON_PORT = 'D';

  pros::adi::Pneumatics piston;
};