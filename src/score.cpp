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
    :scoreMotor(scoreMotor), controller(controller) {}

//MARK: - METHODS

void update(){
}


// void score(){
//     scoreMotor.move_velocity(100);
// }

