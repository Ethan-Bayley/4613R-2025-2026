#pragma once

#include "autonomous/Autonomous.h"
#include "lemlib/chassis/chassis.hpp"
#include "subsystems/Chassis.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/MidDescore.h"
#include "subsystems/Passthrough.h"
#include "subsystems/Wing.h"

class R_LW : public Autonomous {
  Passthrough *passthrough;
  MatchLoad *matchload;
  Wing *wing;

public:
  R_LW(Passthrough *pst, MatchLoad *ml, Wing *wg)
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
    
    chassis.turnToPoint(LOADER_BR.x, LOADER_BR.y, 750);

    chassis.moveToPoint(LOADER_BR.x, LOADER_BR.y, 1625);

    chassis.moveToPoint(GOAL_R.x, GOAL_R.y, 1750,
                        {.forwards = false, .maxSpeed = 80}, true);

    pros::delay(500);
    passthrough->scoreLong();
    pros::delay(1250);
    matchload->retract();

    chassis.moveToPoint(WING_PRE.x, WING_PRE.y, 1250);

    chassis.moveToPoint(WING_WAY.x, WING_WAY.y, 1250, {.forwards = false});

    chassis.setBrakeMode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD);
    chassis.moveToPoint(WING_MID.x, WING_MID.y, 1000,
                        {.forwards = false, .minSpeed = 127, .earlyExitRange = 4});
    chassis.moveToPoint(WING_END.x, WING_END.y, 1000000000, {.forwards = false, .maxSpeed = 50});

    //   pros::delay(20);
    // }

  }

private:
  const lemlib::Pose STARTING_POS_LONG_PUSH{12.5, 24.5, 0};

  const lemlib::Pose STACK_R{24, 47, 135};

  const lemlib::Pose LOADER_BR = lemlib::Pose(46, 9.5, 180);

  const lemlib::Pose GOAL_R_WAY = lemlib::Pose(45, 25, 0);
  const lemlib::Pose GOAL_R = lemlib::Pose(48, 40, -180);
  const lemlib::Pose GOAL_MIDDLE{13, 50, -135};
  const lemlib::Pose LOADER_BR_WAY = lemlib::Pose(48, 15, -90);

  const lemlib::Pose WING_PRE = lemlib::Pose(60, 25, 180);
  const lemlib::Pose WING_WAY = lemlib::Pose(58, 32, 180);
  const lemlib::Pose WING_MID = lemlib::Pose(55, 48, 180);

  const lemlib::Pose WING_END = lemlib::Pose(39, 55, 180);

  int TIMEOUT_SUPER_SHORT = 500;
  int TIMEOUT_SHORT = 1000;
  int TIMEOUT = 2000;
  int TIMEOUT_LONG = 3000;
  float MIN_SPEED = 80;
  float LEAD = 0.8;
};
