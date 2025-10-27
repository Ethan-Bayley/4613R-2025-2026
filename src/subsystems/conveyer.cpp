#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/pose.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include "subsystems/conveyer.h"

//MARK: - CONSTANTS

//MARK: - INITIALISERS

Conveyer::Conveyer(pros::Motor &bottomMotor, pros::Motor &topMotor, 
    pros::adi::Pneumatics &matchLoaderPiston, pros::adi::Pneumatics &middleGoalPiston)
    : bottomMotor(bottomMotor), topMotor(topMotor), 
    matchLoaderPiston(matchLoaderPiston), middleGoalPiston(middleGoalPiston)
    {}

//MARK: - METHODS

void Conveyer::onEntry(State state) {
    switch(state) {
        case INTAKE:
        intake();
        break;
        case SCORE_LONG:
    bottomMotor.move_voltage(-12000);
    topMotor.move_voltage(12000);
            break;
        case SCORE_MIDDLE:
        scoreMiddle();
        break;
        case OUTTAKE:
        outtake();
        break;
        case MATCH_LOADER:
        matchLoader();
        break;
    }
}

void Conveyer::transitionTo(State nextState) {
    previousState = currentState;
    if(currentState != nextState) {
        if (previousState == MATCH_LOADER) {
            matchLoaderPiston.retract();
        }
        currentState = nextState;
        onEntry(nextState);
    }
}

void Conveyer::scoreLong() {
    bottomMotor.move_voltage(12000);
    topMotor.move_voltage(12000);
    middleGoalPiston.extend();
}

void Conveyer::scoreMiddle() {
    bottomMotor.move_voltage(12000);
    topMotor.move_voltage(12000);
    middleGoalPiston.retract();
}

void Conveyer::outtake() {
    bottomMotor.move_voltage(12000);
}

void Conveyer::intake(){
    bottomMotor.move_voltage(12000);
}

void Conveyer::matchLoader() {
    matchLoaderPiston.extend();
    bottomMotor.move_voltage(12000);
}