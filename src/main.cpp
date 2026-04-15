#include "main.h"
#include "Robot.h"
#include "brainScreen.h"
#include "commands/EventLoop.h"
#include "subsystems/Chassis.h"
#include "subsystems/Hood.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/MidDescore.h"
#include "subsystems/Passthrough.h"
#include "subsystems/Wing.h"

#include "autonomous/Autonomous.h"

#include "autonomous/L_LMW.h"
#include "autonomous/L_LW.h"

#include "autonomous/R_LMW.h"
#include "autonomous/R_LW.h"

#include "autonomous/R_AWP.h"
#include "autonomous/R_CSAWP.h"

pros::Controller controller(pros::E_CONTROLLER_MASTER);
Robot robot(controller);

double averageVector(std::vector<double> vec)
{
  double sum = 0;
  for (double num : vec)
  {
    sum += num;
  }
  return sum / vec.size();
}

void initialize()
{
  chassis.calibrate();

  robot.addSubsystem(std::make_unique<Passthrough>());
  robot.addSubsystem(std::make_unique<MatchLoad>());
  robot.addSubsystem(std::make_unique<Wing>());
  robot.addSubsystem(std::make_unique<MidDescore>());
  robot.setup();
  robot.registerCommands();

  drawImage();

  pros::Task controllerScreenTask([&]()
                                  {
    Passthrough* pass = robot.getSubsystem<Passthrough>();
    while (true) {
      std::string leftTemp = "L:" + std::to_string((int)averageVector(left_mg.get_temperature_all()));
      std::string rightTemp = "R:" + std::to_string((int)averageVector(right_mg.get_temperature_all()));
      std::string prerollerTemp = "P:" + std::to_string((int)pass->prerollerMotor.get_temperature());
      std::string middleTemp = "M:" + std::to_string((int)pass->middleMotor.get_temperature());
      std::string topTemp = "T:" + std::to_string((int)pass->topMotor.get_temperature());

      controller.set_text(0, 0, leftTemp + " " + rightTemp + " " + prerollerTemp + " " + middleTemp + " " + topTemp);
      controller.set_text(1, 0, "B%: " + std::to_string((int)pros::battery::get_capacity()));
      controller.set_text(2, 0, "Auto: L LMW");

      pros::delay(1000);
    } });
}

void disabled() {}

void competition_initialize() {}

void autonomous()
{
  Passthrough *passthrough = robot.getSubsystem<Passthrough>();
  MatchLoad *matchLoad = robot.getSubsystem<MatchLoad>();
  Wing *wing = robot.getSubsystem<Wing>();
  MidDescore *midDescore = robot.getSubsystem<MidDescore>();

  enum class Auto
  {
    L_LMW,
    L_LW,
    R_LMW,
    R_LW,
    R_AWP,
    R_CSAWP
  };

  Auto selectedAuto = Auto::L_LMW;

  switch (selectedAuto)
  {
  case Auto::L_LMW:
    robot.runAuto<L_LMW>(passthrough, matchLoad, wing, midDescore);
    break;
  case Auto::L_LW:
    robot.runAuto<L_LW>(passthrough, matchLoad, wing, midDescore);
    break;
  case Auto::R_LMW:
    robot.runAuto<R_LMW>(passthrough, matchLoad, wing, midDescore);
    break;
  case Auto::R_LW:
    robot.runAuto<R_LW>(passthrough, matchLoad, wing, midDescore);
    break;
  case Auto::R_AWP:
    robot.runAuto<R_AWP>(passthrough, matchLoad, wing, midDescore);
    break;
  case Auto::R_CSAWP:
    robot.runAuto<R_CSAWP>(passthrough, matchLoad, wing, midDescore);
    break;
  }
}

void opcontrol()
{
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  robot.getSubsystem<Passthrough>()->stop();
  while (true)
  {
    robot.update();
    pros::delay(20);
  }
}