#include "main.h"
#include "autonomous/nats/AWP.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include <memory>
#include "Robot.h"
#include "commands/EventLoop.h"
#include "pros/misc.h"
#include "subsystems/Conveyer.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/Descore.h"
#include "subsystems/Park.h"

#include "autonomous/nats/LongGoal.h"
#include "autonomous/nats/LongGoalR.h"
#include "autonomous/nats/MLGoal.h"
#include "autonomous/nats/MLGoal2.h"
#include "autonomous/nats/Skills.h"
#include "autonomous/nats/SkillsAWP.h"

pros::Controller controller(pros::E_CONTROLLER_MASTER);
Robot robot(controller);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	// while (true) {
	// 			// print robot location to the brain screen
	// 		pros::lcd::set_text(0, "X: " + std::to_string(chassis.getPose().x)); // x
	// 		pros::lcd::set_text(1, "Y: " + std::to_string(chassis.getPose().y)); // y
	// 		pros::lcd::set_text(2, "Theta: " + std::to_string(chassis.getPose().theta)); // heading
	// 			// delay to save resources
	// 		pros::delay(20);
	// }


	robot.addSubsystem(std::make_unique<Conveyer>());
	robot.addSubsystem(std::make_unique<MatchLoad>());
	robot.addSubsystem(std::make_unique<Descore>());
	robot.addSubsystem(std::make_unique<Park>());

	robot.setup();
	robot.registerCommands();
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
	robot.runAuto<NatsMLGoal2>(
		robot.getSubsystem<Conveyer>(),
		robot.getSubsystem<MatchLoad>(),
		// robot.getSubsystem<Park>()
		robot.getSubsystem<Descore>()
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
	controller.rumble("---");
	while (true) {
		auto start_time = pros::millis();

		// Subsysytems
		robot.update();

		pros::c::task_delay_until(&start_time, 10);
	}



}