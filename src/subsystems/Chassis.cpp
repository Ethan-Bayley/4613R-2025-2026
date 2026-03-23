#include "lemlib/api.hpp"
#include "lemlib/chassis/trackingWheel.hpp"

pros::MotorGroup left_mg({-11, -12, 14}, pros::MotorGearset::blue);
pros::MotorGroup right_mg({19, 18, -17}, pros::MotorGearset::blue);

pros::Imu imu(8);
// pros::Rotation odometry(15);
// pros::Rotation odometryH(19);

lemlib::Drivetrain drivetrain(
		&left_mg,
		&right_mg,
		12,
		lemlib::Omniwheel::NEW_325,
		400,
		0);

// lemlib::TrackingWheel vertical_tracking_wheel(
// 		&odometry,
// 		lemlib::Omniwheel::NEW_2,
// 		0.4);
// lemlib::TrackingWheel horizontal_tracking_wheel(
// 		&odometryH,
// 		lemlib::Omniwheel::NEW_275,
// 		1.85);

lemlib::OdomSensors sensors(
		// &vertical_tracking_wheel,
		nullptr,
		nullptr,
		// &horizontal_tracking_wheel,
		nullptr,
		nullptr,
		&imu);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(
		10, // proportional gain (kP)
		0,	 // integral gain (kI)
		1,	 // derivative gain (kD)
		0,	 // anti windup
		0,	 // small error range, in inches
		0,	 // small error range timeout, in milliseconds
		0,	 // large error range, in inches
		0,	 // large error range timeout, in milliseconds
		10	 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(
		1.2, // proportional gain (kP)//2.1
		0,	 // integral gain (kI)
		3.5, // derivative gain (kD)//8
		3,	 // anti windup
		0,	 // small error range, in degrees
		0,	 // small error range timeout, in milliseconds
		0,	 // large error range, in degrees
		0,	 // large error range timeout, in milliseconds
		10	 // maximum acceleration (slew)
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(
		3,		// joystick deadband out of 127
		10,		// minimum output where drivetrain will move out of 127
		1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(
		3,		// joystick deadband out of 127
		10,		// minimum output where drivetrain will move out of 127
		1.019 // expo curve gain
);

lemlib::Chassis chassis(
		drivetrain,
		lateral_controller,
		angular_controller,
		sensors,
		&throttle_curve,
		&steer_curve);
