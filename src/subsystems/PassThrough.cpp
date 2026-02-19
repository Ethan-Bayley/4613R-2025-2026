
#include "pros/motors.h"
#include "subsystems/Passthrough.h"


Passthrough::Passthrough() : 
    intakeMotor(INTAKE_MOTOR_PORT, pros::MotorGearset::blue),
    midGoalMotor(MIDGOAL_MOTOR_PORT, pros::MotorGearset::blue),
    longGoalMotor(LONGGOAL_MOTOR_PORT, pros::MotorGearset::blue)

{
}

void Passthrough::intake(){
    intakeMotor.move_voltage(12000);
}

void Passthrough::outtake(){
    intakeMotor.move_voltage(-12000);
}

void Passthrough::stop(){
    intakeMotor.move_voltage(0);
}

void Passthrough::scoreMiddle(){
    intakeMotor.move_voltage(12000);
    midGoalMotor.move_voltage(12000);
}

void Passthrough::scoreLong(){
    intakeMotor.move_voltage(12000);
    longGoalMotor.move_voltage(12000);
}
