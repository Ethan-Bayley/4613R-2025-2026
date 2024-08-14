#include "main.h"

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
  pros::lcd::set_text(1, "Hello PROS User!");

  pros::lcd::register_btn1_cb(on_center_button);
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
void autonomous() {}

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
	pros::MotorGroup left_mg({-11, -12, -13});    // Creates a motor group 
	pros::MotorGroup right_mg({16,17,18});  // Creates a motor group 
	pros::MotorGroup Intake({2,-5});//creates intake
	pros::adi::Pneumatics Mogo_clip('h', false);         // Starts retracted, extends when the ADI port is high
  	pros::adi::Pneumatics Climber('g', false); // Starts retracted, extends when the ADI port is low 
  
	Intake.set_brake_mode_all(pros::motor_brake_mode_e_t::E_MOTOR_BRAKE_COAST);

	while (true) {
		bool mogo = false;
		bool climb = false;
		int leftStickY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int leftStickX = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

		int leftSpeedSetpoint = leftStickY + leftStickX * 0.6;
		int RightSpeedSetpoint = leftStickY - leftStickX * 0.6;

		left_mg.move(leftSpeedSetpoint);

		right_mg.move(RightSpeedSetpoint);

	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
	Intake.move(127);
	} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
		Intake.move(-127);
		
	} else {Intake.brake();}
	
	
	
	
	if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
		Mogo_clip.toggle();
	}	
	if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)){
		Climber.toggle();

	}
	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)){
		left_mg.move(-127);
		right_mg.move(127);
	}


	
	








	}
}