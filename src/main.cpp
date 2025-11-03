#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
// #include "commands/EventLoop.h"
// #include "subsystems/DriveConstants.h"
// #include "Robot.h"
// #include "subsystems/Conveyer.h"
// #include "subsystems/Drivetrain.h"
// #include "subsystems/MatchLoad.h"
// #include <cstdio>
// #include <memory>

pros::MotorGroup left_mg({-1, 2, -3}, pros::MotorGearset::blue);
pros::MotorGroup right_mg({8, -9, 10}, pros::MotorGearset::blue);


lemlib::Drivetrain drivetrain(
		&left_mg,
		&right_mg,
		12,
		lemlib::Omniwheel::NEW_325,
		400,
		0);

pros::Imu imu(12);
pros::Rotation vertical_tracking_encoder(15);

lemlib::TrackingWheel vertical_tracking_wheel(
		&vertical_tracking_encoder,
		lemlib::Omniwheel::NEW_2,
		0.4);
lemlib::OdomSensors sensors(
		&vertical_tracking_wheel,
		nullptr,
		nullptr,
		nullptr,
		&imu);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(
		6.5, // proportional gain (kP)
		0,	 // integral gain (kI)
		0,	 // derivative gain (kD)
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

lemlib::Chassis drive(
	drivetrain,
	lateral_controller,
	angular_controller,
	sensors
);

// pros::Controller controller(pros::E_CONTROLLER_MASTER);
// EventLoop eventLoop{controller};
// std::unique_ptr<Robot> robotInstance;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	// robotInstance = std::make_unique<Robot>(eventLoop);

	// drive.calibrate();

// 	robotInstance->addSubsystem(
// 		std::make_unique<Conveyer>()
// 	); 
// 	robotInstance->addSubsystem(
// 		std::make_unique<MatchLoad>()
// 	);
// 	robotInstance->addSubsystem(
// 		std::make_unique<Drivetrain>(drive)
// 	);
	
// 	robotInstance->setup();
// 	robotInstance->registerCommands();
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
	// Match auto Red left
	// autoLongGoal autoRoutine(chassis);
	// autoRoutine.run();

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
	// while (true) {
	// 	robotInstance->update();
	// 	pros::delay(25);
	// }
	
}