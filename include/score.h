#ifndef SCORE_H
#define SCORE_H

#include "pros/adi.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"

class Score{
    public:
    Score(pros::Motor &scoreMotor, pros::adi::Pneumatics &middleGoalPiston, pros::Controller &controller);

    void update();

    private:
        pros::Controller &controller;
        pros::adi::Pneumatics &middleGoalPiston;
        pros::Motor &scoreMotor;

        void scoreHigh();
        void scoreMiddle();
        void prepHighScore();
        void prepMiddleScore();

};

#endif