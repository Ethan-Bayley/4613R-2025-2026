#ifndef CONVEYER_H
#define CONVEYER_H

#include "pros/adi.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"

class Conveyer {
    public:
    Conveyer(pros::Motor &bottomMotor, pros::Motor &topMotor, 
        pros::adi::Pneumatics &matchLoaderPiston, pros::adi::Pneumatics &middleGoalPiston);
    enum State {
        SCORE_LONG,
        SCORE_MIDDLE,
        OUTTAKE,
        INTAKE,
        MATCH_LOADER
    };
    void transitionTo(State nextState);
    

private:
    pros::Motor &topMotor;
    pros::Motor &bottomMotor;
    pros::adi::Pneumatics &matchLoaderPiston;
    pros::adi::Pneumatics &middleGoalPiston;

    State currentState;
    State previousState;

    
    void onEntry(State state);
    void scoreLong();
    void scoreMiddle();
    void outtake();
    void intake();
    void matchLoader();
};

#endif