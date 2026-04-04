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
#include <string>

#include "autonomous/AWP.h"

pros::Controller controller(pros::E_CONTROLLER_MASTER);
Robot robot(controller);

void initialize() {
  imu.reset(true);

  chassis.calibrate(); // starts LemLib odometry task

  robot.addSubsystem(std::make_unique<Passthrough>());
  robot.addSubsystem(std::make_unique<MatchLoad>());
  robot.addSubsystem(std::make_unique<Wing>());
  robot.setup();
  robot.registerCommands();

  drawImage();

  pros::Task screen_task([&]() {
    while (true) {
      // print robot location to the brain screen

      controller.set_text(0, 0, "X: " + std::to_string(chassis.getPose().x));
      controller.set_text(1, 0, "Y: " + std::to_string(chassis.getPose().y));
      controller.set_text(2, 0,
                          "Theta: " + std::to_string(chassis.getPose().theta));
      robot.update();
      // delay to save resources
      pros::delay(20);
    }
  });
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
  controller.rumble(".");

  chassis.setPose(0, 0, 0);
  // robot.runAuto<WorldsAWP>(robot.getSubsystem<Passthrough>(),
  // 						 robot.getSubsystem<MatchLoad>());

  while (true) {
    auto start_time = pros::millis();

    // Subsysytems
    robot.update();
    if (robot.getSubsystem<Passthrough>()->getColourSorting() == 1) {
      controller.set_text(
          1, 1,
          std::to_string(
              robot.getSubsystem<Passthrough>()->getColourSorting()));
    }
	
	if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
		controller.clear();
	}

    pros::c::task_delay_until(&start_time, 10);
  }
}