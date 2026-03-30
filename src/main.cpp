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
  pros::delay(100); // small delay to let pose set
  const lemlib::Pose GOAL_QUICK = lemlib::Pose(38, 30, 90);

  const lemlib::Pose STARTING_POS{15.25, 24, 90};
  // const lemlib::Pose STARTING_POS{15.25, 24, 90};

  const lemlib::Pose LOADER_BR_WAY = lemlib::Pose(48, 24, 90);
  const lemlib::Pose LOADER_BR = lemlib::Pose(48, 11, -180);

  const lemlib::Pose STACK_RR{19.5, 48, -135};
  const lemlib::Pose STACK_B_MIDPOINT{-16, 46, -90};
  const lemlib::Pose STACK_B{-23.5, 48, -90};
  const lemlib::Pose GOAL_MIDDLE{-10, 56, -135};


  const lemlib::Pose GOAL_L_WAIT{-50, 24, -135};
  const lemlib::Pose GOAL_L = lemlib::Pose(-52, 47, -180);

  const lemlib::Pose GOAL_R = lemlib::Pose(48, 48, -180);


  const lemlib::Pose LOADER_BL_WAY = lemlib::Pose(-33,17, 90);
  const lemlib::Pose LOADER_MID = lemlib::Pose(0, 32, 90);
  const lemlib::Pose LOADER_BL_WAY1 = lemlib::Pose(-49, 30, -160);
  const lemlib::Pose LOADER_BL = lemlib::Pose(-48, 6, 180);

  const lemlib::Pose PARK_WAY = lemlib::Pose(-51, 33, 180);
  const lemlib::Pose PARK_MID = lemlib::Pose(-3, 33, 90);
  const lemlib::Pose PARK = lemlib::Pose(-3, 16, 90);

  const lemlib::Pose HIT_LEFT = lemlib::Pose(-52, 38, -180);
  const lemlib::Pose HIT_RIGHT = lemlib::Pose(48, 39, -180);

  int TIMEOUT_SUPER_SHORT = 500;
  int TIMEOUT_SHORT = 1000;
  int TIMEOUT = 2000;
  int TIMEOUT_LONG = 3000;
  float MIN_SPEED = 80;
  float LEAD = 0.8;
  // int TIMEOUT_SUPER_SHORT = 500;
  // int TIMEOUT_SHORT = 1000;
  // int TIMEOUT = 2000;
  // int TIMEOUT_LONG = 3000;
  // float MIN_SPEED = 80;
  // float LEAD = 0.8;

  chassis.setPose(STARTING_POS);
  // chassis.setPose(STARTING_POS);
  
  chassis.moveToPoint(LOADER_BR_WAY.x, LOADER_BR_WAY.y, TIMEOUT_SHORT);
  chassis.turnToHeading(-180, TIMEOUT_SUPER_SHORT);
  // chassis.moveToPoint(LOADER_BR_WAY.x, LOADER_BR_WAY.y, TIMEOUT_SHORT);
  // chassis.turnToHeading(-180, TIMEOUT_SUPER_SHORT);
  
  chassis.moveToPoint(LOADER_BR.x, LOADER_BR.y, TIMEOUT_SHORT);
  pros::delay(2100);
  chassis.moveToPoint(GOAL_R.x, GOAL_R.y, TIMEOUT,
                      {.forwards = false, .maxSpeed = 100});
  chassis.moveToPoint(GOAL_QUICK.x, GOAL_QUICK.y , TIMEOUT_SHORT, {.maxSpeed = 80});
  chassis.turnToHeading(45, 900);
  pros::delay(500);
  chassis.moveToPoint(LOADER_BL_WAY1.x, LOADER_BL_WAY1.y , TIMEOUT_SHORT, {.maxSpeed = 80});

  
  while (true) {
		auto start_time = pros::millis();

		robot.update();

		pros::c::task_delay_until(&start_time, 10);
	}

}