#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H
#include "lemlib/chassis/chassis.hpp"
#include "pros/misc.hpp"

class Drivetrain{

public:
    Drivetrain(lemlib::Chassis &drivetrain, pros::Controller &controller);
    
    void arcade();
    void setPose(lemlib::Pose pose);
    void moveToPose(lemlib::Pose pose, int timeout);
    void turnToPoint();
    

private:
    lemlib::Chassis &drivetrain;
    pros::Controller &controller;
};

#endif