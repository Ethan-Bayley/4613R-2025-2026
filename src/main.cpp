#include "main.h"
#include "Robot.h"
#include "autonomous/L_LRUSH.h"
#include "brainScreen.h"
#include "commands/EventLoop.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "subsystems/Chassis.h"
#include "subsystems/Hood.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/MidDescore.h"
#include "subsystems/Passthrough.h"
#include "subsystems/Wing.h"

#include "autonomous/R_AWP.h"

#include "autonomous/R_LMW.h"
#include "autonomous/R_LW.h"

#include "autonomous/L_LMW.h"

#include "autonomous/R_LRUSH.h"

#include "autonomous/L_LW.h"

#include <cstdlib>
#include <memory>
#include <string>

pros::Controller controller(pros::E_CONTROLLER_MASTER);
Robot robot(controller);

double averageVector(std::vector<double> vec) {
  double sum = 0;
  for (double num : vec) {
    sum += num;
  }
  return sum / vec.size();
}

void initialize() {
  // pros::lcd::initialize(); // initialize brain screen

  chassis.calibrate();
  robot.addSubsystem(std::make_unique<Passthrough>());
  robot.addSubsystem(std::make_unique<MatchLoad>());
  robot.addSubsystem(std::make_unique<Wing>());
  robot.addSubsystem(std::make_unique<MidDescore>());

  // auto* ml = robot.getSubsystem<MatchLoad>();
  // auto* md = robot.getSubsystem<MidDescore>();
  // ml->setRobot(&robot);
  // md->setRobot(&robot);

  robot.setup();
  robot.registerCommands();
  Passthrough *pass = robot.getSubsystem<Passthrough>();

    pros::Task controllerScreenTask([pass]() {
    int tick = 0;
    while (true) {
      std::string leftTemp =
          "L:" +
          std::to_string((int)averageVector(left_mg.get_temperature_all()));
      std::string rightTemp =
          "R:" +
          std::to_string((int)averageVector(right_mg.get_temperature_all()));
      std::string prerollerTemp =
          "P:" + std::to_string((int)pass->prerollerMotor.get_temperature());
      std::string middleTemp =
          "M:" + std::to_string((int)pass->middleMotor.get_temperature());
      std::string topTemp =
          "T:" + std::to_string((int)pass->topMotor.get_temperature());
      std::string battStr =
          "Bat:" + std::to_string((int)pros::battery::get_capacity()) + "%";

      switch (tick % 3) {
        case 0: controller.set_text(0, 0, leftTemp + " " + rightTemp); break;
        case 1: controller.set_text(1, 0, prerollerTemp + " " + middleTemp +
        " " + topTemp); break; case 2: controller.set_text(2, 0, battStr);
        break;
      }

      tick++;
      pros::delay(150);
    }
  });

  drawImage();
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
  pros::Task passthroughUpdate([]() {
    while (true) {
      robot.getSubsystem<Passthrough>()->update();
      pros::delay(20);
    }
  });
  uint32_t start = pros::millis();
  robot.runAuto<
      R_AWP
      // L_LMW
      // L_LW
      // L_LRUSH
      // R_LMW
      // R_LW
      // R_LRUSH
      >(robot.getSubsystem<Passthrough>(), robot.getSubsystem<MatchLoad>(),
        robot.getSubsystem<Wing>());

  controller.clear();
  controller.set_text(0, 1, std::to_string((pros::millis() - start) / 1000));
  controller.set_text(1, 1, std::to_string((pros::millis() - start) / 1000));
  controller.set_text(2, 1, std::to_string((pros::millis() - start) / 1000));
  controller.set_text(3, 1, std::to_string((pros::millis() - start) / 1000));
  passthroughUpdate.remove();
  /*

  pros build
  pros upload --slot 1 --name R_AWP

  Slot:
  1 - R_AWP
  2 - L_LMW ✅
  3 - L_LW ✅
  4 - L_LRUSH ✅
  5 - R_LMW ✅
  6 - R_LW ✅
  7 - R_LRUSH ✅

  */
}

void opcontrol() {

  // chassis.setPose(0, 26, -90);
  // controller.clear();
  // pros::Task screen_task([&]() {
  //   while (true) {
  //     controller.set_text(1, 1, std::to_string((chassis.getPose().x) /
  //     1000)); controller.set_text(2, 1, std::to_string((chassis.getPose().y)
  //     / 1000)); controller.set_text(3, 1,
  //                         std::to_string((chassis.getPose().theta) / 1000));
  //     // delay to save resources
  //     pros::delay(20);
  //   }
  // });
  // bool isTip = false;

  while (true) {
    robot.update();
    pros::delay(20);
  }
  // uint32_t start = pros::millis();
  // robot.runAuto<L_LMW>(robot.getSubsystem<Passthrough>(),
  //                      robot.getSubsystem<MatchLoad>(),
  //                      robot.getSubsystem<Wing>());
  // controller.set_text(1, 1, std::to_string((pros::millis() - start) / 1000));

  // uint32_t start = pros::millis();
  // robot.runAuto<ChampsLeftLongMiddle>(robot.getSubsystem<Passthrough>(),
  //                                     robot.getSubsystem<MatchLoad>(),
  //                                     robot.getSubsystem<Wing>());
  // controller.set_text(1, 1, std::to_string((pros::millis() - start) / 1000));
}
