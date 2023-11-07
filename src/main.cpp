#include "main.h"
#include "hardware.hpp"

// std::shared_ptr<OdomChassisController> drivetrain;

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
	pros::delay(500);

	// drivetrain = ChassisControllerBuilder()
	// 	.withMotors(
	// 		{-1, -2}, // left motors -1, -2 (reversed)
	// 		{3, 4} // right motors 3, 4
	// 	)
	// 	// green gearset, 4 inch wheel diameter, 11.5 inch wheel track
	// 	.withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})
	// 	.withOdometry()
	// 	.buildOdometry();	
	// Autonomous Selector using LLEMU

	ez::as::auton_selector.add_autons({
		Auton("Example Drive\n\nDrive forward and come back.", drive_example),
		Auton("Example Turn\n\nTurn 3 times.", turn_example),
		Auton("Drive and Turn\n\nDrive forward, turn, come back. ", drive_and_turn),
		Auton("Drive and Turn\n\nSlow down during drive.", wait_until_change_speed),
		Auton("Swing Example\n\nSwing, drive, swing.", swing_example),
		Auton("Combine all 3 movements", combining_movements),
		Auton("Interference\n\nAfter driving forward, robot performs differently if interfered or not.", interfered_example),
	});

	// Initialize chassis and auton selector
	ez::as::initialize();
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
	ez::as::auton_selector.call_selected_auton(); // Calls selected auton from autonomous selector.
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
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor left_mtr(1);
	pros::Motor right_mtr(2);

	while (true) {
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);

		drivetrain->getModel()->arcade(left, right);

		pros::delay(ez::util::DELAY_TIME);
	}
}
