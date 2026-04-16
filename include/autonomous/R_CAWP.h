#pragma once

#include "autonomous/Autonomous.h"
#include "lemlib/chassis/chassis.hpp"

#include "subsystems/Chassis.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/MidDescore.h"
#include "subsystems/Passthrough.h"
#include "subsystems/Wing.h"

class R_CAWP : public Autonomous {
  Passthrough *passthrough;
  MatchLoad *matchLoad;
  Wing *wing;

public:
  R_CAWP(Passthrough *pst, MatchLoad *ml, Wing *wg)
      : passthrough(pst), matchLoad(ml), wing(wg) {}

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

    chassis.moveToPoint(LOADER_BR.x, LOADER_BR.y, TIMEOUT_SHORT);

    // pros::Task hoodRaise([&]() {
    //   pros::delay(250);
    // });
    chassis.moveToPoint(GOAL_R.x, GOAL_R.y, 1500,
                        {.forwards = false, .maxSpeed = 100}, true);

    pros::delay(500);
    passthrough->scoreLong();
    matchLoad->retract();
    pros::delay(750);

    // END OF RIGHT GOAL

    chassis.swingToPoint(STACK_RR.x, STACK_RR.y, lemlib::DriveSide::RIGHT, 500);

    passthrough->intake();

    chassis.moveToPose(STACK_RR.x, STACK_RR.y, -90, 1000,
                       {.minSpeed = 80, .earlyExitRange = 8});

    matchLoad->extend();

    chassis.moveToPoint(STACK_MIDPOINT.x, STACK_MIDPOINT.y, 1000,
                        {.minSpeed = 80, .earlyExitRange = 4});
    matchLoad->retract();

    chassis.moveToPoint(STACK_B.x, STACK_B.y, 1000,
                        {.minSpeed = 127, .earlyExitRange = 8}, true);
    pros::delay(500);
    matchLoad->extend();

    chassis.moveToPoint(LOADER_BL_WAY.x, LOADER_BL_WAY.y, 1250,
                        {.minSpeed = 127}, true);

    chassis.turnToPoint(GOAL_L.x, GOAL_L.y, 1000, {.forwards = false});
    chassis.moveToPoint(GOAL_L.x, GOAL_L.y, 2000,
                        {.forwards = false, .minSpeed = 100}, true);

    passthrough->scoreLong();
    pros::delay(1000);

    passthrough->intake();

    chassis.moveToPoint(LOADER_BL.x, LOADER_BL.y, 1000);
    pros::delay(250);

    chassis.moveToPoint(LOADER_BL_WAY.x, LOADER_BL_WAY.y, 1250,
                        {.forwards = false, .minSpeed = 100}, true);
    matchLoad->retract();

    chassis.turnToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, TIMEOUT_SUPER_SHORT,
                        {.forwards = false});
    chassis.moveToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, 2500, {.forwards = false},
                        true);
    pros::delay(1000);
    passthrough->scoreMiddleSlow();
    pros::delay(1500);

    chassis.moveToPoint(WING_WAY.x, WING_WAY.y, 1000);

    chassis.setBrakeMode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD);
    chassis.moveToPoint(
        WING_MID.x, WING_MID.y, 1000,
        {.forwards = false, .minSpeed = 127, .earlyExitRange = 4});
    chassis.moveToPoint(WING_END.x, WING_END.y, 1000000000,
                        {.forwards = false, .maxSpeed = 100});
  }

private:
  const lemlib::Pose STARTING_POS{0, 24, -90};

  const lemlib::Pose PUSH{-5, 24, 90};

  const lemlib::Pose STACK_RR{23, 44, -135};

  const lemlib::Pose STACK_MIDPOINT{0, 40, -135};

  const lemlib::Pose STACK_B{-23.5, 41, -90};
  const lemlib::Pose GOAL_MIDDLE{-10, 67, -135};

  const lemlib::Pose GOAL_R = lemlib::Pose(50, 46, -180);
  const lemlib::Pose GOAL_L = lemlib::Pose(-45, 40, -180);

  const lemlib::Pose LOADER_BR_WAY = lemlib::Pose(50, 22, 90);
  const lemlib::Pose LOADER_BR = lemlib::Pose(50, 10, -180);

  const lemlib::Pose LOADER_BL_WAY = lemlib::Pose(-45, 22, -160);
  const lemlib::Pose LOADER_BL = lemlib::Pose(-45, 9.5, 180);

  const lemlib::Pose WING_WAY = lemlib::Pose(-34, 44, 180);
  const lemlib::Pose WING_MID = lemlib::Pose(-35, 48, 180);

  const lemlib::Pose WING_END = lemlib::Pose(-36, 67, 180);

  int TIMEOUT_SUPER_SHORT = 500;
  int TIMEOUT_SHORT = 1000;
  int TIMEOUT = 2000;
  int TIMEOUT_LONG = 3000;
  float MIN_SPEED = 80;
  float LEAD = 0.8;
};