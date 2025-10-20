#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/pose.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include "subsystems/intake.h"

//MARK: - CONSTANTS

//MARK: - INITIALISERS

Intake::Intake(pros::Motor &bottomIntake, pros::adi::Pneumatics &matchLoader, pros::Controller &controller)
    : bottomIntake(bottomIntake), matchLoader(matchLoader), controller(controller) {}

//MARK: - METHODS

void Intake::update() {   
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        intake();
    }
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        outtake();
    }
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
        matchLoaderUp();
    }
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
        matchLoaderDown();
    }
}

void Intake::intake() {
    bottomIntake.move_velocity(100);
}
void Intake::outtake() {
    bottomIntake.move_velocity(-100);
}
void Intake::matchLoaderUp() {
    matchLoader.extend();
}
void Intake::matchLoaderDown() {
    matchLoader.retract();
}