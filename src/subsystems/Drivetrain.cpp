#include "subsystems/Drivetrain.h"
#include "commands/JoystickCommand.h"
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
    std::make_shared<JoystickCommand>(
      eventLoop.controller,
      pros::E_CONTROLLER_ANALOG_LEFT_X,
      [this](int value) { leftY = value; }
    )
  );

  eventLoop.registerCommand(
    std::make_shared<JoystickCommand>(
      eventLoop.controller,
      pros::E_CONTROLLER_ANALOG_LEFT_Y,
      [this](int value) { leftX = value; }
    )
  );
}

void Drivetrain::update()
{
  chassis.arcade(leftX, leftY);
}
