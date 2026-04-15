#pragma once

#include "autonomous/Autonomous.h"
#include "lemlib/chassis/chassis.hpp"

#include "subsystems/Chassis.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/Passthrough.h"
#include "subsystems/Wing.h"
#include "subsystems/MidDescore.h"

class R_CSAWP : public Autonomous
{
  Passthrough *passthrough;
  MatchLoad *matchLoad;
  Wing *wing;
  MidDescore *midDescore;

public:
  R_CSAWP(Passthrough *pst, MatchLoad *ml, Wing *wg, MidDescore *md)
      : passthrough(pst), matchLoad(ml), wing(wg), midDescore(md) {}

  void run() override
  {
    pros::Task passthroughUpdate([&]()
                                 {
      while (true) {
        passthrough->update();
        // delay to save resources
        pros::delay(20);
      } });
    chassis.setPose(STARTING_POS);

    passthrough->intakePreroller();

    chassis.moveToPoint(PUSH.x, PUSH.y, 750, {.minSpeed = 100});

    chassis.moveToPoint(LOADER_BR_WAY.x, LOADER_BR_WAY.y, 1500,
                        {.forwards = false, .minSpeed = 100});
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

    chassis.moveToPoint(STACK_RR.x, STACK_RR.y, 1000, {.minSpeed = 127, .earlyExitRange = 8});

    chassis.moveToPoint(STACK_MIDPOINT.x, STACK_MIDPOINT.y, 1000, {.minSpeed = 127, .earlyExitRange = 8});

    matchLoad->extend();

    pros::Task retractMatchloadTask([&]()
                                    {
      pros::delay(500);
    matchLoad->retract();
      pros::delay(850);
      matchLoad->extend(); });

    chassis.moveToPoint(STACK_B.x, STACK_B.y, 1000, {.minSpeed = 127, .earlyExitRange = 8}, true);

    chassis.moveToPoint(LOADER_BL_WAY.x, LOADER_BL_WAY.y, 1250,
                        {.minSpeed = 127}, true);
    matchLoad->retract();

    chassis.turnToPoint(GOAL_L.x, GOAL_L.y, 1000,
                        {.forwards = false});
    chassis.moveToPoint(GOAL_L.x, GOAL_L.y, 2000,
                        {.forwards = false, .minSpeed = 100}, true);

    passthrough->scoreLong();
    pros::delay(1000);

    passthrough->intake();

    chassis.moveToPoint(LOADER_BL.x, LOADER_BL.y, 1000);
    pros::delay(250);

    chassis.moveToPoint(LOADER_BL_WAY.x, LOADER_BL_WAY.y, 1250,
                        {.forwards = false, .minSpeed = 100}, true);

    chassis.turnToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, 750,
                        {.forwards = false});
    chassis.moveToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, 1000,
                        {.forwards = false, .maxSpeed = 75});

    pros::delay(1250);
    passthrough->scoreMiddle();

    chassis.moveToPoint(WING_WAY.x, WING_WAY.y, 1000);

    chassis.turnToHeading(180, 250);
    chassis.setBrakeMode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD);
    chassis.moveToPoint(WING_MID.x, WING_MID.y, 1000,
                        {.forwards = false, .minSpeed = 100});
    chassis.moveToPoint(WING_END.x, WING_END.y, 1000000000,
                        {.forwards = false});
  }

private:

  const lemlib::Pose STARTING_POS{0, 24, -90};

  const lemlib::Pose PUSH{-5, 24, 90};

  const lemlib::Pose STACK_RR{19.5, 47, -135};

  const lemlib::Pose STACK_MIDPOINT{0, 43, -135};

  const lemlib::Pose STACK_B{-23.5, 41, -90};
  const lemlib::Pose GOAL_MIDDLE{0 - 5, 60 - 5, -135};

  const lemlib::Pose GOAL_R = lemlib::Pose(50, 46, -180);
  const lemlib::Pose GOAL_L = lemlib::Pose(-40, 40, -180);

  const lemlib::Pose LOADER_BR_WAY = lemlib::Pose(48, 22, 90);
  const lemlib::Pose LOADER_BR = lemlib::Pose(48, 11, -180);

  const lemlib::Pose LOADER_BL_WAY = lemlib::Pose(-40, 22, -160);
  const lemlib::Pose LOADER_BL = lemlib::Pose(-40, 9.5, 180);

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