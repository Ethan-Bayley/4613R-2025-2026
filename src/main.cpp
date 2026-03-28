#include "main.h"
#include "Robot.h"
#include "brainScreen.h"
#include "commands/EventLoop.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "subsystems/Chassis.h"
#include "subsystems/Hood.h"
#include "subsystems/Matchload.h"
#include "subsystems/Passthrough.h"
#include "subsystems/Wing.h"
#include <memory>

#include "autonomous/AWP.h"

pros::Controller controller(pros::E_CONTROLLER_MASTER);
Robot robot(controller);

void initialize()
{
	pros::lcd::initialize();

	imu.reset(true); // blocks until IMU finishes calibrating (~3 seconds)
	pros::lcd::print(0, "IMU done: %.2f", imu.get_heading());

	chassis.calibrate(); // starts LemLib odometry task
	pros::lcd::print(1, "Chassis ready");

	robot.addSubsystem(std::make_unique<Passthrough>());
	robot.addSubsystem(std::make_unique<MatchLoad>());
	robot.addSubsystem(std::make_unique<Wing>());
	robot.setup();
	robot.registerCommands();

	drawImage();

	pros::Task screen_task([&]()
						   {
    while (true) {
      // print robot location to the brain screen
      pros::lcd::print(0, "X: %f", chassis.getPose().x);         // x
      pros::lcd::print(1, "Y: %f", chassis.getPose().y);         // y
      pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
      robot.update();
      // delay to save resources
      pros::delay(20);
    } });
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol()
{
	controller.rumble(".");

	chassis.setPose(0, 0, 0);
	robot.runAuto<WorldsAWP>(robot.getSubsystem<Passthrough>(),
							 robot.getSubsystem<MatchLoad>());

	// while (true) {
	// 	auto start_time = pros::millis();

	// 	// Subsysytems
	// 	robot.update();

	// 	pros::c::task_delay_until(&start_time, 10);
	// }
}