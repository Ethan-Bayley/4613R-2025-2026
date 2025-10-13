#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include "lemlib/api.h"

class Drivetrain{

public:
    Drivetrain(pros::MotorGroup &left_mg, pros::MotorGroup &right_mg);
    
    void turnToPoint();
    void moveToPoint();
    void setPose();

private:

    pros::MotorGroup &left_mg;
    pros::MotorGroup &right_mg;

    lemlib::Chassis &drivetrain
};

#endif