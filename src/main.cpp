#include "main.h"
#include "Robot.h"
#include "brainScreen.h"
#include "commands/EventLoop.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "subsystems/Chassis.h"
#include "subsystems/Hood.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/Matchload.h"
#include "subsystems/Passthrough.h"
#include "subsystems/Wing.h"

#include "autonomous/AWP.h"

#include <memory>
#include <string>

pros::Controller controller(pros::E_CONTROLLER_MASTER);
Robot robot(controller);

void initialize() {
  pros::lcd::initialize();

  imu.reset(); // blocks until IMU finishes calibrating (~3 seconds)
  pros::lcd::print(0, "IMU done: %.2f", imu.get_heading());

  chassis.calibrate(); // starts LemLib odometry task
  pros::lcd::print(1, "Chassis ready");

  robot.addSubsystem(std::make_unique<Passthrough>());
  robot.addSubsystem(std::make_unique<MatchLoad>());
  robot.addSubsystem(std::make_unique<Wing>());
  robot.setup();
  robot.registerCommands();

  //   drawImage();

  pros::Task screen_task([&]() {
    while (true) {
      // print robot location to the brain screen
      pros::lcd::print(0, "X: %f", chassis.getPose().x);         // x
      pros::lcd::print(1, "Y: %f", chassis.getPose().y);         // y
      pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
      // delay to save resources
      pros::delay(20);
    }
  });
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
  uint32_t start = pros::millis();
  robot.runAuto<AWP>(robot.getSubsystem<Passthrough>(),
                     robot.getSubsystem<MatchLoad>());
              controller.set_text(1, 1, std::to_string((pros::millis() - start)/1000));
}

void opcontrol() {
  pros::Task screen_task([&]() {
    while (true) {
      robot.update();
      pros::delay(20);
    }
  });
}
