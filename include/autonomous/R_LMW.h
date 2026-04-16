#pragma once

#include "autonomous/Autonomous.h"
#include "lemlib/chassis/chassis.hpp"
#include "subsystems/Chassis.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/MidDescore.h"
#include "subsystems/Passthrough.h"
#include "subsystems/Wing.h"

class R_LMW : public Autonomous {
  Passthrough *passthrough;
  MatchLoad *matchload;
  Wing *wing;

public:
  R_LMW(Passthrough *pst, MatchLoad *ml, Wing *wg)
      : passthrough(pst), matchload(ml), wing(wg) {}

  void run() override {
    pros::Task passthroughUpdate([&]() {
      while (true) {
        passthrough->update();
        pros::delay(20);
      }
    });
    chassis.setPose(STARTING_POS_LONG_PUSH);
    passthrough->intake();

    chassis.moveToPoint(BEFORE_UNDER_GOAL_R.x, BEFORE_UNDER_GOAL_R.y, TIMEOUT,
                        {.minSpeed = 80, .earlyExitRange = 4}, true);
    pros::Task ballsInMatchload([&]() {
      pros::delay(300);
      matchload->extend();
      pros::delay(350);
      matchload->retract();
    });

    chassis.moveToPoint(UNDER_GOAL_R.x, UNDER_GOAL_R.y, 750, {}, true);
    pros::Task ballsInMatchload1([&]() {
      pros::delay(500);
      matchload->extend();
    });
    chassis.turnToHeading(270, 100, {}, true);
    chassis.moveToPoint(
        STACK_R.x, STACK_R.y, TIMEOUT,
        {.forwards = false, .minSpeed = 127, .earlyExitRange = 8});

    chassis.moveToPoint(GOAL_R_WAY.x, GOAL_R_WAY.y, 1750, {.forwards = false});

    chassis.turnToPoint(GOAL_R.x, GOAL_R.y, 500, {.forwards = false});
    chassis.moveToPoint(GOAL_R.x, GOAL_R.y, 1750, {.forwards = false, .maxSpeed = 80}, true);

    pros::delay(500);
    passthrough->scoreLong();
    pros::delay(1250);

    chassis.moveToPoint(LOADER_BR.x, LOADER_BR.y, 1500, {.maxSpeed = 80}, true);
    pros::delay(500);
    passthrough->intake();

    chassis.moveToPoint(LOADER_BR_WAY.x, LOADER_BR_WAY.y, TIMEOUT_SUPER_SHORT,
                        {.forwards = false});
    matchload->retract();
    chassis.turnToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, TIMEOUT_SUPER_SHORT);
    chassis.moveToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, 2500, {},
                        true);
    pros::delay(1000);
    passthrough->scoreLow();
    pros::delay(1500);

    chassis.moveToPoint(WING_WAY.x, WING_WAY.y, 1000, {.forwards = false});

    chassis.turnToHeading(0, 250);
    chassis.setBrakeMode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD);
    chassis.moveToPoint(WING_MID.x, WING_MID.y, 1000,
                        {.minSpeed = 127, .earlyExitRange = 4});
    chassis.moveToPoint(WING_END.x, WING_END.y, 1000000000,
                        {.maxSpeed = 50});

    //   pros::delay(20);
    // }
  }

private:
  const lemlib::Pose STARTING_POS_LONG_PUSH{11.5, 24.5, 0};
  const lemlib::Pose UNDER_GOAL_R = lemlib::Pose(42, 64, -60);
  const lemlib::Pose BEFORE_UNDER_GOAL_R =
      lemlib::Pose(24, UNDER_GOAL_R.y - 11, 135);

  const lemlib::Pose STACK_R{24, 47, 135};

  const lemlib::Pose LOADER_BR = lemlib::Pose(45, 14, 180);

  const lemlib::Pose GOAL_R_WAY = lemlib::Pose(45, 24, 0);
  const lemlib::Pose GOAL_R = lemlib::Pose(51, 52, -180);
  const lemlib::Pose GOAL_MIDDLE{14, 57, -135};
  const lemlib::Pose LOADER_BR_WAY = lemlib::Pose(44, 30, -90);
  
  const lemlib::Pose WING_WAY = lemlib::Pose(34, 35, 180);
  const lemlib::Pose WING_MID = lemlib::Pose(36, 48, 180);

  const lemlib::Pose WING_END = lemlib::Pose(39, 67, 180);

  int TIMEOUT_SUPER_SHORT = 500;
  int TIMEOUT_SHORT = 1000;
  int TIMEOUT = 2000;
  int TIMEOUT_LONG = 3000;
  float MIN_SPEED = 80;
  float LEAD = 0.8;
};
