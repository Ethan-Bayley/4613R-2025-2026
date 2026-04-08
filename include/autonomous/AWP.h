#pragma once

#include "autonomous/Autonomous.h"
#include "autonomous/smerandoauto.h"
#include "lemlib/chassis/chassis.hpp"
#include "pros/llemu.hpp"
#include "pros/rtos.hpp"
#include "subsystems/Chassis.h"
#include "subsystems/Matchload.h"

class AWP : public Autonomous {
  Passthrough *passthrough;
  MatchLoad *matchLoad;

public:
  AWP(Passthrough *passthrough, MatchLoad *ml)
      : passthrough(passthrough), matchLoad(ml) {}
  void run() override {
    chassis.setPose(STARTING_POS);

    chassis.moveToPoint(PUSH.x, PUSH.y, 500,
                        {.forwards = false, .minSpeed = 80});

    passthrough->intake();

    chassis.moveToPoint(LOADER_BR_WAY.x, LOADER_BR_WAY.y, TIMEOUT);
    matchLoad->extend();

    chassis.turnToHeading(-180, TIMEOUT_SUPER_SHORT);
    pros::delay(500);

    chassis.moveToPoint(LOADER_BR.x, LOADER_BR.y, TIMEOUT_SHORT);

    // pros::Task hoodRaise([&]() {
    //   pros::delay(250);
    // });
    chassis.moveToPoint(GOAL_R.x, GOAL_R.y, 750,
                        {.forwards = false, .minSpeed = 127}, true);
    pros::delay(850);
    passthrough->scoreLongNoColour();
    matchLoad->retract();
    chassis.moveToPoint(GOAL_R.x, GOAL_R.y, 1250, {.forwards = false});

    passthrough->hoodRaised();

    // END OF RIGHT GOAL

    chassis.swingToPoint(STACK_RR.x, STACK_RR.y, lemlib::DriveSide::RIGHT, 500);

    passthrough->intake();

    chassis.moveToPoint(STACK_RR.x, STACK_RR.y, 750);

    matchLoad->extend();

    // matchLoad->extend();

    // pros::Task retractMatchloadTask([&]() {
    //   pros::delay(500);
    //   matchLoad->retract();
    // });
    chassis.turnToPoint(STACK_B.x, STACK_B.y, 500);

    matchLoad->retract();
    pros::Task retractMatchloadTask([&]() {
      pros::delay(750);
      matchLoad->extend();
    });
    chassis.moveToPoint(STACK_B.x, STACK_B.y, 1000);
    chassis.moveToPoint(STACK_B.x+5, STACK_B.y, 250, {.forwards = false});

    chassis.turnToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, TIMEOUT_SUPER_SHORT,
                        {.forwards = false});
    chassis.moveToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, 750,
                       {.forwards = false, .maxSpeed = 75});
    passthrough->scoreMiddleSlow();
    chassis.turnToHeading(-145, 200);
    chassis.turnToHeading(-125, 200);

    pros::delay(1000);
    
    passthrough->outtake();
    pros::delay(250);

    passthrough->intake();

    chassis.moveToPoint(LOADER_BL_WAY.x, LOADER_BL_WAY.y, 2000);
    
    chassis.turnToHeading(180, 500);
    chassis.moveToPoint(LOADER_BL.x, LOADER_BL.y, 1500);
    pros::delay(250);

    passthrough->hoodRaised();
    chassis.moveToPoint(GOAL_L.x, GOAL_L.y, TIMEOUT_SHORT,
                        {.forwards = false, .minSpeed = 127});
    passthrough->scoreLong();
    chassis.moveToPoint(GOAL_L.x, GOAL_L.y - 2, 2000, {.forwards = false});
  }

private:
  //   const lemlib::Pose STARTING_POS{15.25, 24, 90};
  const lemlib::Pose STARTING_POS{0, 26, 90};

  const lemlib::Pose PUSH{-3, 26, 90};

  const lemlib::Pose STACK_RR{19.5, 49, -135};
  const lemlib::Pose STACK_B{-23.5, 46, -90};
  const lemlib::Pose GOAL_MIDDLE{0, 60, -135};

  const lemlib::Pose GOAL_L = lemlib::Pose(-43, 47, -180);
  const lemlib::Pose GOAL_R = lemlib::Pose(48, 48, -180);

  const lemlib::Pose LOADER_BR_WAY = lemlib::Pose(48, 24, 90);
  const lemlib::Pose LOADER_BR = lemlib::Pose(48, 9.5, -180);

  const lemlib::Pose LOADER_BL_WAY = lemlib::Pose(-41, 24, -160);
  const lemlib::Pose LOADER_BL = lemlib::Pose(-41, 6, 180);

  const lemlib::Pose HIT_LEFT = lemlib::Pose(-52, 38, -180);
  const lemlib::Pose HIT_RIGHT = lemlib::Pose(48, 39, -180);

  int TIMEOUT_SUPER_SHORT = 500;
  int TIMEOUT_SHORT = 1000;
  int TIMEOUT = 2000;
  int TIMEOUT_LONG = 3000;
  float MIN_SPEED = 80;
  float LEAD = 0.8;
};
