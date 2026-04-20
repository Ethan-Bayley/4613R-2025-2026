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
    chassis.setPose(STARTING_POS_LONG_PUSH);
    wing->extend();
    passthrough->intake();

    chassis.moveToPoint(STACK_R.x, STACK_R.y, 550, {}, true);
    pros::delay(500);
    matchload->extend();
    
    chassis.turnToHeading(0, 500);

    chassis.moveToPoint(GOAL_R_WAY.x, GOAL_R_WAY.y, 1250, {.forwards = false});

    chassis.turnToPoint(GOAL_R.x, GOAL_R.y, 750, {.forwards = false});
    chassis.moveToPoint(GOAL_R.x, GOAL_R.y, 1750,
                        {.forwards = false, .maxSpeed = 80}, true);

    pros::delay(500);
    passthrough->scoreLongNoColour();
    pros::delay(1250);
    passthrough->intakePreroller();

    chassis.moveToPoint(LOADER_BR.x, LOADER_BR.y, 1625);

    chassis.moveToPoint(LOADER_BR_WAY.x, LOADER_BR_WAY.y, 750,
                        {.forwards = false});
    matchload->retract();
    chassis.turnToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, 750);
    chassis.moveToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, 1250);
    chassis.turnToHeading(-45, 750, {}, true);
    passthrough->scoreLow();
    pros::delay(1500);

    chassis.moveToPoint(WING_WAY.x, WING_WAY.y, 1250, {.forwards = false});

    chassis.turnToHeading(5, 500);
    chassis.setBrakeMode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD);
    chassis.moveToPoint(WING_MID.x, WING_MID.y, 1000,
                        {.minSpeed = 127, .earlyExitRange = 4});
    chassis.moveToPoint(WING_END.x, WING_END.y, 1000000000, {.maxSpeed = 50});

    //   pros::delay(20);
    // }

  }

private:
  const lemlib::Pose STARTING_POS_LONG_PUSH{12.5, 24.5, 0};

  const lemlib::Pose STACK_R{24, 47, 135};

  const lemlib::Pose LOADER_BR = lemlib::Pose(46, 4, 180);

  const lemlib::Pose GOAL_R_WAY = lemlib::Pose(45, 25, 0);
  const lemlib::Pose GOAL_R = lemlib::Pose(48, 40, -180);
  const lemlib::Pose GOAL_MIDDLE{13, 52, -135};
  const lemlib::Pose LOADER_BR_WAY = lemlib::Pose(48, 15, -90);

  const lemlib::Pose WING_WAY = lemlib::Pose(35, 32, 180);
  const lemlib::Pose WING_MID = lemlib::Pose(36, 48, 180);

  const lemlib::Pose WING_END = lemlib::Pose(39, 55, 180);

  int TIMEOUT_SUPER_SHORT = 500;
  int TIMEOUT_SHORT = 1000;
  int TIMEOUT = 2000;
  int TIMEOUT_LONG = 3000;
  float MIN_SPEED = 80;
  float LEAD = 0.8;
};
