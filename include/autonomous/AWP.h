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
    pros::Task passthroughUpdate([&]() {
      while (true) {
        passthrough->update();
        // delay to save resources
        pros::delay(20);
      }
    });
    chassis.setPose(STARTING_POS);

    passthrough->intakePreroller();

    chassis.moveToPoint(PUSH.x, PUSH.y, 750, {.minSpeed = 100});

    chassis.moveToPoint(LOADER_BR_WAY.x, LOADER_BR_WAY.y, 1500,
                        {.forwards = false});
    passthrough->intake();
    matchLoad->extend();

    chassis.turnToPoint(LOADER_BR.x, LOADER_BR.y, 600);
    pros::delay(250);

    chassis.moveToPoint(LOADER_BR.x, LOADER_BR.y, TIMEOUT_SHORT);

    // pros::Task hoodRaise([&]() {
    //   pros::delay(250);
    // });
    chassis.moveToPoint(GOAL_R.x, GOAL_R.y, 1500,
                        {.forwards = false, .maxSpeed = 75}, true);

    pros::delay(1250);
    passthrough->scoreLongNoColour();
    pros::delay(1000);
    matchLoad->retract();

    // END OF RIGHT GOAL

    chassis.swingToPoint(STACK_RR.x, STACK_RR.y, lemlib::DriveSide::RIGHT, 500);

    passthrough->intake();

    chassis.moveToPoint(STACK_RR.x, STACK_RR.y, 1000, {.minSpeed = 80, .earlyExitRange = 10});

    matchLoad->extend();

    // matchLoad->extend();

    // pros::Task retractMatchloadTask([&]() {
    //   pros::delay(500);
    //   matchLoad->retract();
    // });
    chassis.swingToPoint(
        STACK_MIDPOINT.x, STACK_MIDPOINT.y, lemlib::DriveSide::RIGHT, 500,
        {.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE});


    matchLoad->retract();
    pros::Task retractMatchloadTask([&]() {
      pros::delay(850);
      matchLoad->extend();
    });

    chassis.moveToPoint(STACK_MIDPOINT.x, STACK_MIDPOINT.y, 1000, {.minSpeed = 80, .earlyExitRange = 8});

    chassis.moveToPoint(STACK_B.x, STACK_B.y, 1000);
    chassis.moveToPoint(STACK_B.x+3, STACK_B.y, 250);

    chassis.turnToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, TIMEOUT_SUPER_SHORT,
                        {.forwards = false});
    chassis.moveToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, 750,
                        {.forwards = false, .maxSpeed = 75});
    passthrough->scoreMiddle();

    pros::delay(1000);

    chassis.moveToPoint(LOADER_BL_WAY.x, LOADER_BL_WAY.y, 1250, {.minSpeed = 100}, true);

    passthrough->outtake();
    pros::delay(500);

    passthrough->intake();


    chassis.turnToPoint(LOADER_BL.x, LOADER_BL.y, 750);
    chassis.moveToPoint(LOADER_BL.x, LOADER_BL.y, 1000);
    pros::delay(250);

    passthrough->intake();
    chassis.moveToPoint(GOAL_L.x, GOAL_L.y, 5000,
                        {.forwards = false, .minSpeed = 127});
    passthrough->scoreLong();
    pros::delay(1000);
  }

private:
  //   const lemlib::Pose STARTING_POS{15.25, 24, 90};
  //   const lemlib::Pose STARTING_POS{0, 27, -90};

  //   const lemlib::Pose PUSH{-5, 27, 90};

  //   const lemlib::Pose STACK_RR{25, 52, -135};
  //   const lemlib::Pose STACK_B{-23.5, 45, -90};
  //   const lemlib::Pose GOAL_MIDDLE{-5, 65, -135};

  //   const lemlib::Pose GOAL_R = lemlib::Pose(47, 52, -180);
  //   const lemlib::Pose GOAL_L = lemlib::Pose(-43, 47, -180);

  //   const lemlib::Pose LOADER_BR_WAY = lemlib::Pose(48, 26, 90);
  //   const lemlib::Pose LOADER_BR = lemlib::Pose(48, 10.5, -180);

  //   const lemlib::Pose LOADER_BL_WAY = lemlib::Pose(-41, 24, -160);
  //   const lemlib::Pose LOADER_BL = lemlib::Pose(-41, 6, 180);

  //   const lemlib::Pose HIT_LEFT = lemlib::Pose(-52, 38, -180);
  //   const lemlib::Pose HIT_RIGHT = lemlib::Pose(48, 39, -180);

  const lemlib::Pose STARTING_POS{0, 26, -90};

  const lemlib::Pose PUSH{-5, 26, 90};

  const lemlib::Pose STACK_RR{19.5, 49, -135};
  
  const lemlib::Pose STACK_MIDPOINT{0, 45, -135};

  const lemlib::Pose STACK_B{-23.5, 42, -90};
  const lemlib::Pose GOAL_MIDDLE{0, 61, -135};

  const lemlib::Pose GOAL_R = lemlib::Pose(50, 48, -180);
  const lemlib::Pose GOAL_L = lemlib::Pose(-38, 47, -180);

  const lemlib::Pose LOADER_BR_WAY = lemlib::Pose(48, 24, 90);
  const lemlib::Pose LOADER_BR = lemlib::Pose(48, 9.5, -180);

  const lemlib::Pose LOADER_BL_WAY = lemlib::Pose(-38, 24, -160);
  const lemlib::Pose LOADER_BL = lemlib::Pose(-38, 6, 180);

  const lemlib::Pose HIT_LEFT = lemlib::Pose(-52, 38, -180);
  const lemlib::Pose HIT_RIGHT = lemlib::Pose(48, 39, -180);

  int TIMEOUT_SUPER_SHORT = 500;
  int TIMEOUT_SHORT = 1000;
  int TIMEOUT = 2000;
  int TIMEOUT_LONG = 3000;
  float MIN_SPEED = 80;
  float LEAD = 0.8;
};
