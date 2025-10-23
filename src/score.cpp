#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/pose.hpp"
#include "autoLongGoal.h"
#include "config.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include "score.h"

//MARK: - CONSTANTS

//MARK: - INITIALISERS

Score::Score(pros::Motor &scoreMotor, pros::adi::Pneumatics &middleGoalPiston, pros::Controller &controller)
    :scoreMotor(scoreMotor), middleGoalPiston(middleGoalPiston), controller(controller) {}

//MARK: - METHODS

void Score::update(){
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        scoreHigh();
    }
    else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        scoreMiddle();
    }
    else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
        prepHighScore();
    }
    else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)){
        prepMiddleScore();
    }
}

void Score::scoreHigh() {
    scoreMotor.move_velocity(100);
}
void Score::scoreMiddle(){
    scoreMotor.move_velocity(-100);
}
void Score::prepHighScore(){
    middleGoalPiston.extend();
}
void Score::prepMiddleScore(){
    middleGoalPiston.retract();
}
