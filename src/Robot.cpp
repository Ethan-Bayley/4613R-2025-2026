#include "Robot.h"
#include "subsystems/Chassis.h"
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

void Robot::addSubsystem(std::unique_ptr<Subsystems> subsystem) {
    robotSubsystems.push_back(std::move(subsystem));
}

void Robot::registerCommands() {
    for (auto& subsystem : robotSubsystems) {
        subsystem->registerHandlers(eventLoop);
    }
}

void Robot::update() {

    // Process button presses -> sets subsystem states
    eventLoop.update();

    // Run update() on all subsystems -> applies states to motors
    for (auto& subsystem : robotSubsystems) {
        subsystem->update();
    }

    // Update drivetrain
    int throttle = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) * 0.8;
    chassis.arcade(throttle, turn);
}