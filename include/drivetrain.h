#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H
#include "lemlib/chassis/chassis.hpp"
#include "pros/misc.hpp"

class Drivetrain{

public:
    Drivetrain(lemlib::Chassis &drivetrain, pros::Controller &controller);
    
    void arcade();
    void setPose(lemlib::Pose pose);
    void moveToPose(lemlib::Pose pose, bool forward, int timeout = 2000);
    void moveToPose(lemlib::Pose pose, bool forward = true, float minSpeed = 80, float earlyExitRange = 4, int timeout = 2000);
    void turnToPoint();
    

private:
    lemlib::Chassis &drivetrain;
    pros::Controller &controller;
};

#endif