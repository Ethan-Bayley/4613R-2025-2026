#pragma once

#include "autonomous/Autonomous.h"
#include "subsystems/Chassis.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/Passthrough.h"
#include <cmath>

class WorldsAWP : public Autonomous {
  Passthrough *passthrough;
  MatchLoad *matchLoad;

public:
  WorldsAWP(Passthrough *passthrough, MatchLoad *ml)
      : passthrough(passthrough), matchLoad(ml) {}

  void run() override {
    chassis.setPose(STARTING_POS);

    chassis.moveToPoint(LOADER_BR_WAY.x, LOADER_BR_WAY.y, TIMEOUT_SHORT);
    chassis.turnToHeading(-180, TIMEOUT_SUPER_SHORT);

    matchLoad->extend();
    passthrough->intake();

    chassis.moveToPoint(LOADER_BR.x, LOADER_BR.y, TIMEOUT_SHORT);
    pros::delay(1000);
    chassis.moveToPoint(GOAL_R.x, GOAL_R.y, TIMEOUT,
                        {.forwards = false, .maxSpeed = 100});

    passthrough->scoreLong();
    matchLoad->retract();
    matchLoad->retract();

    pros::delay(2000);
    // END OF RIGHT GOAL

    chassis.moveToPoint(GOAL_R.x, GOAL_R.y - 13, TIMEOUT_SHORT,
                        {.maxSpeed = 80});
    passthrough->intake();

    // chassis.moveToPoint(GOAL_R.x, GOAL_R.y, TIMEOUT,
    //{.forwards = false, .maxSpeed = 60, .minSpeed = 60});
    chassis.turnToHeading(-60, 900);
    chassis.moveToPose(STACK_RR.x, STACK_RR.y, 270, TIMEOUT_SHORT,
                       {.maxSpeed = 127, .minSpeed = 50, .earlyExitRange = 8});
    matchLoad->extend();
    pros::Task retractMatchloadTask([&]() {
      pros::delay(200);
      matchLoad->retract();
      pros::delay(500);
      matchLoad->extend();
    });

    chassis.moveToPoint(STACK_B.x, STACK_B.y, 1500, {.maxSpeed = 127});
    matchLoad->extend();
    chassis.turnToHeading(-135, TIMEOUT_SUPER_SHORT);
    chassis.moveToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, 2000,
                        {.forwards = false, .maxSpeed = 50});

    passthrough->scoreMiddle();

    chassis.moveToPoint(LOADER_BL_WAY1.x, LOADER_BL_WAY1.y - 10, 3000);
    passthrough->intake();
    chassis.turnToHeading(180, 500);
    chassis.moveToPoint(LOADER_BL.x, LOADER_BL.y, TIMEOUT_SHORT);
    chassis.moveToPoint(GOAL_L.x, GOAL_L.y, TIMEOUT_SHORT, {.forwards = false});
    passthrough->scoreLong();
  }

private:
  const lemlib::Pose STARTING_POS{15.25, 24, 90};

  const lemlib::Pose LOADER_BR_WAY = lemlib::Pose(48, 24, 90);
  const lemlib::Pose LOADER_BR = lemlib::Pose(48, 11, -180);

  const lemlib::Pose STACK_RR{19.5, 48, -135};
  const lemlib::Pose STACK_B{-23.5, 48, -90};
  const lemlib::Pose GOAL_MIDDLE{-10, 56, -135};

  const lemlib::Pose GOAL_L = lemlib::Pose(-47, 47, -180);

  const lemlib::Pose GOAL_R = lemlib::Pose(48, 48, -180);

  const lemlib::Pose LOADER_BL_WAY = lemlib::Pose(-33, 17, 90);
  const lemlib::Pose LOADER_MID = lemlib::Pose(0, 32, 90);
  const lemlib::Pose LOADER_BL_WAY1 = lemlib::Pose(-41, 30, -160);
  const lemlib::Pose LOADER_BL = lemlib::Pose(-41, 6, 180);

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
};