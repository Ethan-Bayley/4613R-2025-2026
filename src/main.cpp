#include "main.h"
#include "EventLoop.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "subsystems/DriveConstants.h"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "Robot.h"
#include "subsystems/Conveyer.h"
#include "subsystems/MatchLoad.h"
#include <memory>

pros::Controller controller(pros::E_CONTROLLER_MASTER);
EventLoop eventLoop{controller};
std::unique_ptr<Robot> robotInstance;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();

	robotInstance = std::make_unique<Robot>(eventLoop);

	robotInstance->addSubsystem(
		std::make_unique<Conveyer>()
	);
	robotInstance->addSubsystem(
		std::make_unique<MatchLoad>()
	);
	
	robotInstance->setup();
	robotInstance->registerButtonHandlers();
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
	while (true) {
		int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int leftX = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
		chassis.arcade(leftY, leftX);
		robotInstance->update();
		pros::delay(25);
	}
	
}