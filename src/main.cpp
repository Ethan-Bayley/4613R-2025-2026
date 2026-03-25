#include "main.h"
#include "Robot.h"
#include "commands/EventLoop.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "subsystems/Chassis.h"
#include "subsystems/Passthrough.h"
#include <memory>


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

  imu.reset(true); // blocks until IMU finishes calibrating (~3 seconds)
  pros::lcd::print(0, "IMU done: %.2f", imu.get_heading());

  chassis.calibrate(); // starts LemLib odometry task
  pros::lcd::print(1, "Chassis ready");

  robot.addSubsystem(std::make_unique<Passthrough>());
  robot.setup();
  robot.registerCommands();

  pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            // delay to save resources
            pros::delay(20);
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
  controller.rumble(".");

  const lemlib::Pose STARTING_POS_AWP{15.25, 24, 90};
  const lemlib::Pose STARTING_POS_LONG_PUSH{17, 22, 15};

  const lemlib::Pose LOADER_BR_WAY = lemlib::Pose(48, 24, 90);
  const lemlib::Pose LOADER_BR = lemlib::Pose(48, 11, -180);

  const lemlib::Pose STACK_RR{19.5, 48, -135};
  const lemlib::Pose STACK_B{-23.5, 48, -90};
  const lemlib::Pose GOAL_MIDDLE{-10, 56, -135};

  const lemlib::Pose GOAL_L = lemlib::Pose(-47, 47, -180);

  const lemlib::Pose GOAL_R = lemlib::Pose(48, 48, -180);

  const lemlib::Pose LOADER_BL_WAY = lemlib::Pose(-33,17, 90);
  const lemlib::Pose LOADER_MID = lemlib::Pose(0, 32, 90);
  const lemlib::Pose LOADER_BL_WAY1 = lemlib::Pose(-41, 30, -160);
  const lemlib::Pose LOADER_BL = lemlib::Pose(-41, 6, 180);

  const lemlib::Pose PARK_WAY = lemlib::Pose(-51, 33, 180);
  const lemlib::Pose PARK_MID = lemlib::Pose(-3, 33, 90);
  const lemlib::Pose PARK = lemlib::Pose(-3, 16, 90);

  const lemlib::Pose HIT_LEFT = lemlib::Pose(-52, 38, -180);
  const lemlib::Pose HIT_RIGHT = lemlib::Pose(48, 39, -180);

  
  const lemlib::Pose UNDER_GOAL_R = lemlib::Pose(40, 55, 60);

  int TIMEOUT_SUPER_SHORT = 500;
  int TIMEOUT_SHORT = 1000;
  int TIMEOUT = 2000;
  int TIMEOUT_LONG = 3000;
  float MIN_SPEED = 80;
  float LEAD = 0.8;


  //slow long goal
  chassis.setPose(STARTING_POS_LONG_PUSH);
  
  chassis.moveToPoint(STACK_RR.x, STACK_RR.y, TIMEOUT);
  chassis.turnToHeading(60, 500);


  chassis.moveToPoint(UNDER_GOAL_R.x, UNDER_GOAL_R.y, 1500);
  chassis.moveToPoint(STACK_RR.x, STACK_RR.y, TIMEOUT_SHORT, {.forwards = false, .minSpeed = 80, .earlyExitRange = 8});
  chassis.moveToPoint(LOADER_BR_WAY.x, LOADER_BR_WAY.y, TIMEOUT_SHORT);

  chassis.turnToHeading(180, 900);
  chassis.moveToPoint(LOADER_BR.x, LOADER_BR.y, TIMEOUT_SHORT);

  chassis.moveToPose(GOAL_R.x, GOAL_R.y, 180, TIMEOUT, {.forwards = false});

  //solo awp:
  pros::delay(100); // small delay to let pose set

  

  // chassis.setPose(STARTING_POS);
  
  // chassis.moveToPoint(LOADER_BR_WAY.x, LOADER_BR_WAY.y, TIMEOUT_SHORT);
  // chassis.turnToHeading(-180, TIMEOUT_SUPER_SHORT);
  
  // chassis.moveToPoint(LOADER_BR.x, LOADER_BR.y, TIMEOUT_SHORT);
  // pros::delay(2100);
  // chassis.moveToPoint(GOAL_R.x, GOAL_R.y, TIMEOUT,
  //                     {.forwards = false, .maxSpeed = 100});
  // pros::delay(500);
  
  // // 
  
  // pros::delay(4000);
  // // END OF RIGHT GOAL
  // chassis.moveToPoint(GOAL_R.x, GOAL_R.y - 13, TIMEOUT_SHORT, {.maxSpeed = 80});

  // //chassis.moveToPoint(GOAL_R.x, GOAL_R.y, TIMEOUT,
  //                     //{.forwards = false, .maxSpeed = 60, .minSpeed = 60});
  // chassis.turnToHeading(-60, 900);
  // chassis.moveToPose(STACK_RR.x, STACK_RR.y, 270, TIMEOUT_SHORT, {.maxSpeed = 127, .minSpeed = 50, .earlyExitRange = 8});
  // chassis.moveToPoint(STACK_B.x, STACK_B.y, 1500, {.maxSpeed = 127});
  // chassis.turnToHeading(-135, TIMEOUT_SUPER_SHORT);
  // chassis.moveToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, 2000, {.forwards = false, .maxSpeed = 50});


  // chassis.moveToPoint(LOADER_BL_WAY1.x, LOADER_BL_WAY1.y - 10, 3000);
  // chassis.turnToHeading(180, 500);
  // chassis.moveToPoint(LOADER_BL.x, LOADER_BL.y, TIMEOUT_SHORT);
  // chassis.moveToPoint(GOAL_L.x, GOAL_L.y, TIMEOUT_SHORT, {.forwards = false});
  while (true) {
		auto start_time = pros::millis();

		// Subsysytems
		robot.update();

		pros::c::task_delay_until(&start_time, 10);
	}
}