#include "main.h"
#include "Robot.h"
#include "brainScreen.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "subsystems/Chassis.h"
#include "subsystems/Passthrough.h"
#include "subsystems/Pneumatic.h"
#include "autonomous/AWP.h"
#include <memory>

pros::Controller controller(pros::E_CONTROLLER_MASTER);
Robot robot(controller);

void initialize()
{
	pros::lcd::initialize();

	imu.reset(true);
	pros::lcd::print(0, "IMU done: %.2f", imu.get_heading());

	chassis.calibrate();
	pros::lcd::print(1, "Chassis ready");

	robot.addSubsystem(std::make_unique<Passthrough>());

	robot.addSubsystem<SingleActingPneumatic>('D')
		.withName("wing")
		.withHandler(pros::E_CONTROLLER_DIGITAL_B);

	robot.addSubsystem<SingleActingPneumatic>('H')
		.withName("hood")
		.withHandler(pros::E_CONTROLLER_DIGITAL_DOWN);

	robot.addSubsystem<SingleActingPneumatic>('G')
		.withName("matchload")
		.withHandler(pros::E_CONTROLLER_DIGITAL_L1);

	robot.setup();
	robot.registerCommands();

	drawImage();

	pros::Task screen_task([&]()
						   {
		while (true) {
			pros::lcd::print(0, "X: %f", chassis.getPose().x);
			pros::lcd::print(1, "Y: %f", chassis.getPose().y);
			pros::lcd::print(2, "Theta: %f", chassis.getPose().theta);
			robot.update();
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
	robot.runAuto<WorldsAWP>(
		robot.getSubsystem<Passthrough>(),
		robot.getSubsystem<SingleActingPneumatic>("matchload")
	);
}