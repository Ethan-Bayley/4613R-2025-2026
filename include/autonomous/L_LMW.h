#pragma once

#include "autonomous/Autonomous.h"
#include "lemlib/chassis/chassis.hpp"
#include "subsystems/Chassis.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/MidDescore.h"
#include "subsystems/Passthrough.h"
#include "subsystems/Wing.h"

class L_LMW : public Autonomous {
  Passthrough *passthrough;
  MatchLoad *matchload;
  Wing *wing;

public:
  L_LMW(Passthrough *pst, MatchLoad *ml, Wing *wg)
      : passthrough(pst), matchload(ml), wing(wg) {}

  void run() override {
    chassis.setPose(STARTING_POS_LONG_PUSH);
    passthrough->intake();
    
    pros::Task ballsInMatchload([this]() {
      pros::delay(350);
      matchload->extend();
      pros::delay(300);
      matchload->retract();
      return;
    });

    chassis.moveToPoint(STACK_LL.x, STACK_LL.y, TIMEOUT,
                        {.minSpeed = 127, .earlyExitRange = 8});

    chassis.moveToPoint(BEFORE_UNDER_GOAL_L.x, BEFORE_UNDER_GOAL_L.y, TIMEOUT,
                        {.minSpeed = 80, .earlyExitRange = 4});


    chassis.moveToPoint(UNDER_GOAL_L.x, UNDER_GOAL_L.y, 750, {}, true);
    pros::Task ballsInMatchload1([this]() {
      pros::delay(350);
      matchload->extend();
    });

    chassis.moveToPoint(
        STACK_LL.x, STACK_LL.y, TIMEOUT,
        {.forwards = false, .minSpeed = 127, .earlyExitRange = 8});

    chassis.moveToPoint(GOAL_L_WAY.x, GOAL_L_WAY.y, 1500, {.forwards = false});

    chassis.turnToPoint(GOAL_L.x, GOAL_L.y, 500, {.forwards = false});
    chassis.moveToPoint(GOAL_L.x, GOAL_L.y, 1750, {.forwards = false, .maxSpeed = 80}, true);

    pros::delay(500);
    passthrough->scoreLong();
    pros::delay(1250);

    passthrough->intakePreroller();

    chassis.moveToPoint(LOADER_BL.x, LOADER_BL.y, 1500, {.maxSpeed = 80}, true);
    pros::delay(500);

    chassis.moveToPoint(LOADER_BL_WAY.x, LOADER_BL_WAY.y, TIMEOUT_SUPER_SHORT,
                        {.forwards = false});
    matchload->retract();
    chassis.turnToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, TIMEOUT_SUPER_SHORT,
                        {.forwards = false});
    

    chassis.moveToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, 2500, {.forwards = false}, true);
      pros::delay(550);
      
      passthrough->outtake();
      pros::delay(50);
      passthrough->scoreMiddleSlow();
    chassis.turnToHeading(-135, 500);
      pros::delay(250);
      passthrough->scoreMiddleSlowSlow();

    chassis.moveToPoint(WING_WAY.x, WING_WAY.y, 1250);
    passthrough->intake();

    chassis.turnToHeading(180, 750);
    chassis.setBrakeMode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD);
    chassis.moveToPoint(WING_MID.x, WING_MID.y, 1000,
                        {.forwards = false, .minSpeed = 127, .earlyExitRange = 4});
    chassis.moveToPoint(WING_END.x, WING_END.y, 1000000000,
                        {.forwards = false, .maxSpeed = 50});

    //   pros::delay(20);
    // }
  }

private:
  const lemlib::Pose STARTING_POS_LONG_PUSH{-12.5, 24.5, 0};
  const lemlib::Pose UNDER_GOAL_L = lemlib::Pose(-43.5, 62, -60);
  const lemlib::Pose BEFORE_UNDER_GOAL_L =
      lemlib::Pose(-24, UNDER_GOAL_L.y - 11, 135);

  const lemlib::Pose STACK_LL{-24, 47, 135};

  const lemlib::Pose LOADER_BL = lemlib::Pose(-50, 10, 180);

  const lemlib::Pose GOAL_L_WAY = lemlib::Pose(-50, 24, 0);
  const lemlib::Pose GOAL_L = lemlib::Pose(-52, 51, -180);
  const lemlib::Pose GOAL_MIDDLE{-13, 64.5, -135};
  const lemlib::Pose LOADER_BL_WAY = lemlib::Pose(-44, 25, -90);

  const lemlib::Pose WING_WAY = lemlib::Pose(-40, 40, 180);
  const lemlib::Pose WING_MID = lemlib::Pose(-42, 48, 180);

  const lemlib::Pose WING_END = lemlib::Pose(-43, 67, 180);

  int TIMEOUT_SUPER_SHORT = 500;
  int TIMEOUT_SHORT = 1000;
  int TIMEOUT = 2000;
  int TIMEOUT_LONG = 3000;
  float MIN_SPEED = 80;
  float LEAD = 0.8;
};
