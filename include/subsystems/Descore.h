#pragma once 

#include "commands/EventLoop.h"
#include "Subsystem.h"
#include "pros/adi.hpp"

class Descore : public Subsystem {
  public:
    Descore();
    Descore(int pistonPort);

    void setup() override;
    void registerHandlers(EventLoop &eventLoop) override;

    void toggle();

  private:
  static constexpr int DEFAULT_PISTON_PORT = 'E';

  pros::adi::Pneumatics piston;
};