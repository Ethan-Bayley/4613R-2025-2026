#include "subsystems/Conveyer.h"
#include "subsystems/conveyer.h"
#include <cstdio>

Conveyer::Conveyer() : 
  topMotor(DEFAULT_TOP_PORT, pros::MotorGearset::blue), 
  bottomMotor(DEFAULT_BOTTOM_PORT, pros::MotorGearset::blue),
  piston1(DEFAULT_PISTON_1_PORT, true), 
  piston2(DEFAULT_PISTON_2_PORT, false)
{

}

Conveyer::Conveyer(int bottomMotorPort, int topMotorPort, int pistonPort1, int pistonPort2) : 
  topMotor(topMotorPort, pros::MotorGearset::blue), 
  bottomMotor(bottomMotorPort, pros::MotorGearset::blue), 
  piston1(pistonPort1, true), 
  piston2(pistonPort2, false)
{

}

void Conveyer::setup() {
  topMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  bottomMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  topMotor.set_reversed(true);
  bottomMotor.set_reversed(true);
}

void Conveyer::registerButtonHandlers(EventLoop &eventLoop) {
  // Intake
  eventLoop.registerBinding(
    pros::E_CONTROLLER_DIGITAL_R1, 
    [this]() {this->intake();}, 
    [this]() {this->stow();}
  );
  // Outtake
  eventLoop.registerBinding(
    pros::E_CONTROLLER_DIGITAL_R2, 
    [this]() {this->outtake();}, 
    [this]() {this->stow();}
  );
  // Score Long
  eventLoop.registerBinding(
    pros::E_CONTROLLER_DIGITAL_L1, 
    [this]() {this->scoreLong();}, 
    [this]() {this->stow();}
  );
  // Score Middle
  eventLoop.registerBinding(
    pros::E_CONTROLLER_DIGITAL_L2, 
    [this]() {this->scoreMiddle();}, 
    [this]() {this->stow();}
  );
}

void Conveyer::intake() {
  bottomMotor.move_voltage(12000);
}

void Conveyer::outtake() {
  bottomMotor.move_voltage(-12000);
}

void Conveyer::stow() {
  bottomMotor.move_voltage(10000);
  topMotor.move_voltage(0);
  piston1.extend();
  piston2.retract();
}

void Conveyer::stop() {
  bottomMotor.move_voltage(0);
}

void Conveyer::scoreLong() {
  bottomMotor.move_velocity(1200);
  topMotor.move_velocity(1200);
}

void Conveyer::scoreMiddle() {
  bottomMotor.move_velocity(12000);
  topMotor.move_velocity(-9000);
  piston1.retract();
  piston2.extend();
}

