#include "drivetrain.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "pros/misc.hpp"

//MARK: - CONSTANTS

//MARK: - INITIALISERS

Drivetrain::Drivetrain(lemlib::Chassis &drivetrain, pros::Controller &controller) 
    : drivetrain(drivetrain), controller(controller) {}

//MARK: - METHODS

void Drivetrain::arcade() {
	int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int leftX = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
	drivetrain.arcade(leftY, leftX);
}

void Drivetrain::setPose(lemlib::Pose pose) {
	drivetrain.setPose(pose);
}

void Drivetrain::moveToPose(lemlib::Pose pose, int timeout = 2000) {
	drivetrain.moveToPose(pose.x, pose.y, pose.theta, timeout);
}

