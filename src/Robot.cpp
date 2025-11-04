#include "Robot.h"
#include "subsystems/Chassis.h" // Chassis isn't registered in main, but instead initialised and managed by lemlib
#include "commands/EventLoop.h"

Robot::Robot(pros::Controller &controller)
    : controller(controller), eventLoop(controller) {
}

void Robot::setup() {
  chassis.calibrate();
  for (auto& subsystem : robotSubsystems) {
    subsystem->setup();
  }
}

void Robot::addSubsystem(std::unique_ptr<Subsystem> subsystem) {
  robotSubsystems.push_back(std::move(subsystem));
}

void Robot::registerCommands() {
  for (auto& subsystem : robotSubsystems) {
    subsystem->registerHandlers(eventLoop);
  }
}

void Robot::update() {

  // Update the subsystems
  eventLoop.update();

  // Update drivetrain
  int throttle = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
	int turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
	chassis.arcade(throttle, turn);
}