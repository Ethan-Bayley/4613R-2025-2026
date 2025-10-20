#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "autoLongGoal.h"
#include "pros/abstract_motor.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include "config.hpp"

// PORTS 5, 9, 20 DON'T WORK

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::Motor bottomIntake(1, pros::MotorGearset::blue);
pros::Motor topIntake(2, pros::MotorGearset::blue);

pros::MotorGroup left_mg({11, -14, -13}, pros::MotorGearset::blue);    // Creates a motor group with forwards ports 1 & 3 and reversed port 2
pros::MotorGroup right_mg({1, 2, -3}, pros::MotorGearset::blue);  // Creates a motor group with forwards port 5 and reversed ports 4 & 6

pros::adi::Pneumatics middleGoalPiston('A', true);
pros::adi::Pneumatics matchLoader('B', true);
pros::adi::Pneumatics parkPiston('C', true);

pros::Imu imu(10);
pros::Rotation odometry(-20);

lemlib::Drivetrain drivetrain(
	&left_mg, 
	&right_mg, 
	12, 
	lemlib::Omniwheel::NEW_325, 
	400, 
	2
);

lemlib::TrackingWheel vertical_tracking_wheel(
	&odometry, 
	lemlib::Omniwheel::NEW_2, 
	0
);
lemlib::OdomSensors sensors(
	&vertical_tracking_wheel,
	nullptr, 
	nullptr,
	nullptr, 
	&imu
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(
	6, // proportional gain (kP)
	0, // integral gain (kI)
	0, // derivative gain (kD)
	0, // anti windup
	0, // small error range, in inches
	0, // small error range timeout, in milliseconds
	0, // large error range, in inches
	0, // large error range timeout, in milliseconds
	10 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(
	1.75, // proportional gain (kP)
	0, // integral gain (kI)
	10, // derivative gain (kD)
	3, // anti windup
	0, // small error range, in degrees
	0, // small error range timeout, in milliseconds
	0, // large error range, in degrees
	0, // large error range timeout, in milliseconds
	10 // maximum acceleration (slew)
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(
	3, // joystick deadband out of 127
	10, // minimum output where drivetrain will move out of 127
	1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(
	3, // joystick deadband out of 127
	10, // minimum output where drivetrain will move out of 127
	1.019 // expo curve gain
);

lemlib::Chassis chassis(
	drivetrain, 
	lateral_controller, 
	angular_controller, 
	sensors, 
	&throttle_curve, 
	&steer_curve
);



