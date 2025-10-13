#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "pros/rtos.h"
#include "longGoalAuto.h"

pros::Controller controller(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_mg({11, -14, -13}, pros::MotorGearset::blue);    // Creates a motor group with forwards ports 1 & 3 and reversed port 2
pros::MotorGroup right_mg({1, 2, -3}, pros::MotorGearset::blue);  // Creates a motor group with forwards port 5 and reversed ports 4 & 6
lemlib::Drivetrain drivetrain(
	&left_mg, 
	&right_mg, 
	12, 
	lemlib::Omniwheel::NEW_325, 
	400, 
	2
);

pros::Imu imu(10);
pros::Rotation odometry(-20);
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

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	chassis.calibrate();
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
	pros::Task screen_task([&] {
		while (true) {
			pros::lcd::print(0, "X: %f", chassis.getPose().x);
			pros::lcd::print(1, "Y: %f", chassis.getPose().y);
			pros::lcd::print(2, "Angle: %f", chassis.getPose().theta);
		}
	});
	
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	LongGoalAuto autoRoutine(chassis);
	autoRoutine.run();
	// SKILLS AUTO
	
	lemlib::Pose startingPos = lemlib::Pose(16, 24, 90);
	lemlib::Pose LOADER_BR = lemlib::Pose(48, 12, -180);
	lemlib::Pose GOAL_R = lemlib::Pose(45, 40, -180);
	lemlib::Pose LOADER_TR_WAY = lemlib::Pose(45, 30, -180);
	lemlib::Pose LOADER_TR_WAY1 = lemlib::Pose(30, 48, 20);
	lemlib::Pose LOADER_TR_WAY2 = lemlib::Pose(24, 125, 40);
	lemlib::Pose LOADER_TR = lemlib::Pose(48, 135, 0);
	int TIMEOUT = 2000;
	int TIMEOUT_SHORT = 1000;
	float MIN_SPEED = 80;

	chassis.setPose(startingPos);
	chassis.moveToPose(
		LOADER_BR.x, LOADER_BR.y, LOADER_BR.theta,
		TIMEOUT, 
		{
			.minSpeed = MIN_SPEED, 
			.earlyExitRange = 8
		}
	);
	chassis.moveToPose(
		GOAL_R.x, GOAL_R.y, GOAL_R.theta, 
		TIMEOUT,
		{
			.forwards = false
		}
	);
	chassis.moveToPose(
		LOADER_TR_WAY.x, LOADER_TR_WAY.y, LOADER_TR_WAY.theta,
		TIMEOUT_SHORT,
		{
			.earlyExitRange = 4
		}
	);
	chassis.moveToPose(
		LOADER_TR_WAY1.x, LOADER_TR_WAY1.y, LOADER_TR_WAY1.theta,
		TIMEOUT,
		{
			.earlyExitRange = 4
		}
	);
	chassis.moveToPose(
		LOADER_TR_WAY2.x, LOADER_TR_WAY2.y, LOADER_TR_WAY2.theta,
		TIMEOUT,
		{
			.earlyExitRange = 4
		}
	);
	chassis.moveToPose(
		LOADER_TR.x, LOADER_TR.y, LOADER_TR.theta,
		TIMEOUT,
		{
			.earlyExitRange = 4
		}
	);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	while (true) {
		// get left y and right x positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the robot
        chassis.arcade(leftY, rightX);
        // delay to save resources
        pros::delay(25);                               // Run for 20 ms then update
	}
}