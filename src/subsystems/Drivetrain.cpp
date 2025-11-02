#include "subsystems/Drivetrain.h"
#include "commands/PeriodicCommand.h"
#include "pros/misc.h"
#include "subsystems/DriveConstants.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

Drivetrain::Drivetrain(lemlib::Chassis &chassis) : chassis(chassis) {};

void Drivetrain::setup()
{
}

void Drivetrain::registerHandlers(EventLoop &eventLoop)
{
  eventLoop.registerCommand(
    std::make_shared<PeriodicCommand>(
      [this, &eventLoop]() {
        leftY = eventLoop.controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
        leftX = eventLoop.controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
      }
    )
  );
}

void Drivetrain::update()
{
  chassis.arcade(leftX, leftY);
}
