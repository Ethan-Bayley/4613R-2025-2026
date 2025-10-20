#ifndef INTAKE_H
#define INTAKE_H

#include "pros/adi.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"

class Intake {
    public:
    Intake(pros::Motor &bottomIntake, pros::adi::Pneumatics &matchLoader, pros::Controller &controller);
    
    void update();

private:
    pros::Motor &bottomIntake;
    pros::adi::Pneumatics &matchLoader;
    pros::Controller &controller;

    void intake();
    void outtake();
    void matchLoaderUp();
    void matchLoaderDown();
};

#endif