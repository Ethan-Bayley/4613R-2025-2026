#pragma once

#include "autonomous/Autonomous.h"
#include "subsystems/Conveyer.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/Descore.h"
#include "subsystems/Chassis.h"
#include <cmath>

class StatesSkills : public Autonomous {
    Conveyer* conveyer;
    MatchLoad* matchLoad;

public:
    StatesSkills(Conveyer* conv, MatchLoad* ml)
        : conveyer(conv), matchLoad(ml) { }

    void run() override {

        chassis.setPose(STARTING_POS);
        conveyer->intake();
        chassis.moveToPoint(LOADER_BR_WAY.x, LOADER_BR_WAY.y, TIMEOUT_SHORT);
        chassis.turnToHeading(-180, TIMEOUT_SUPER_SHORT);
        matchLoad->extend();
        chassis.moveToPoint(LOADER_BR.x, LOADER_BR.y, TIMEOUT_SHORT,{}
        );
        chassis.moveToPoint(GOAL_R.x, GOAL_R.y, TIMEOUT_SHORT,{
            .forwards = false
        });
        pros::delay(500);
        matchLoad->retract();
        conveyer->scoreLong();
        pros::delay(1500);

        conveyer->intake();
        chassis.swingToHeading(-45, lemlib::DriveSide::RIGHT, TIMEOUT_SHORT);
        chassis.moveToPoint(STACK_RR.x , STACK_RR.y, TIMEOUT_SHORT);

      // MOVE TO SECOND LOADER
      chassis.moveToPoint(LOADER_TR_WAY.x, LOADER_TR_WAY.y, TIMEOUT_SHORT);
      conveyer->intake();
      chassis.turnToHeading(-20,TIMEOUT_SHORT);

      chassis.moveToPoint(
        LOADER_TR_WAY1.x, LOADER_TR_WAY1.y,
        TIMEOUT_SHORT
      );
      // turning and going to loader
      chassis.moveToPoint(
        LOADER_TR_WAY2.x, LOADER_TR_WAY2.y,
        TIMEOUT
      );
      //turning moving to loader//LOADER TR
      matchLoad->extend();
      chassis.moveToPose(
        LOADER_TR.x, LOADER_TR.y, LOADER_TR.theta,
        TIMEOUT
      );
      //scoring in Right goal from the other side
      chassis.moveToPose(
        GOAL_R1.x, GOAL_R1.y, GOAL_R1.theta,
        TIMEOUT,
        {
          .forwards = false
        }
      );
      conveyer->scoreLong();
      matchLoad->retract();
      chassis.moveToPoint(LOADER_TL_WAY.x, LOADER_TL_WAY.y, TIMEOUT_SHORT);
      chassis.turnToHeading(-93, TIMEOUT_SHORT);
      conveyer->stop();
      chassis.moveToPoint(LOADER_TL_WAY1.x, LOADER_TL_WAY1.y, TIMEOUT_LONG);
      // To loader
      chassis.turnToHeading(0, TIMEOUT);

      matchLoad->extend();
      conveyer->intake();
      //LOADER TL
      chassis.moveToPose(LOADER_TL.x, LOADER_TL.y, LOADER_TL.theta, TIMEOUT,
        {
        .minSpeed=MIN_SPEED,
      });
      //LEFT GOAL
      chassis.moveToPose(GOAL_L1.x, GOAL_L1.y, GOAL_L1.theta,TIMEOUT_SHORT,
        {
          .minSpeed = MIN_SPEED,
        });
      conveyer->scoreLong();
      matchLoad->retract();
      //leaving Goal TL
      // MOVE TO FOURTH LOADER
      chassis.moveToPose(LOADER_BL_WAY.x, LOADER_BL_WAY.y, LOADER_BL_WAY.theta,
        TIMEOUT_SHORT,
        {
          .earlyExitRange = 4
        });
      chassis.swingToHeading(160,lemlib::DriveSide::RIGHT,500,{
        .minSpeed = MIN_SPEED, 
        .earlyExitRange = 15, 
      }
      );
      conveyer->intake();
      chassis.moveToPoint(
        LOADER_BL_WAY1.x, LOADER_BL_WAY1.y,
        TIMEOUT_SHORT,
        {
          .earlyExitRange = 4
        }
      );
      // turning and going to loader
      chassis.moveToPoint(
        LOADER_BL_WAY2.x, LOADER_BL_WAY2.y,
        TIMEOUT,
        {
          .minSpeed = MIN_SPEED, 
          .earlyExitRange = 4
        }
      );
      matchLoad->extend();
      //turning moving to loader//LOADER BL
      chassis.moveToPose(
        LOADER_BL.x, LOADER_BL.y, LOADER_BL.theta,
        TIMEOUT
      );
      conveyer->scoreLong();
      //scoring in Left goal from the other side
      chassis.moveToPoint(GOAL_L.x, GOAL_L.y,TIMEOUT,
        {
          .forwards = false
        }
      );
      chassis.turnToHeading(90, TIMEOUT_SHORT);
      chassis.moveToPoint(PARK.x, PARK.y, TIMEOUT);

    }

private: 
    lemlib::Pose STARTING_POS{15, 24, 90};
    lemlib::Pose GOAL_L_WAIT{-53,24, -135};
    lemlib::Pose STACK_RR{20,52,-135};
    lemlib::Pose STACK_B_MIDPOINT{-16, 48, -90};
    lemlib::Pose STACK_B{-24, 47, -90};
    lemlib::Pose GOAL_MIDDLE{-14,57,-135};

    lemlib::Pose LOADER_BR_WAY = lemlib::Pose(48, 24, 90);
    lemlib::Pose LOADER_BR = lemlib::Pose(48, 13, -180);

    lemlib::Pose GOAL_R = lemlib::Pose(47, 44, -180);

    lemlib::Pose LOADER_TR_WAY = lemlib::Pose(20,52,-135);//diagonal forward toward middle goal
    lemlib::Pose LOADER_TR_WAY1 = lemlib::Pose(30, 55, 20);//right turn
    lemlib::Pose LOADER_TR_WAY2 = lemlib::Pose(33, 118, 41);//across the field 
    lemlib::Pose LOADER_TR = lemlib::Pose(42, 138, 5);

    lemlib::Pose GOAL_R1 = lemlib::Pose(44, 105, 0);

    lemlib::Pose LOADER_TL_WAY = lemlib::Pose(44, 120, -90);
    lemlib::Pose LOADER_TL_WAY1 = lemlib::Pose(-48, 120, -90);
    lemlib::Pose LOADER_TL = lemlib::Pose(-55, 138, 0);

    lemlib::Pose GOAL_L1 = lemlib::Pose(-55, 105, -90);

    lemlib::Pose LOADER_BL_WAY = lemlib::Pose(-55, 115, 0);
    lemlib::Pose LOADER_BL_WAY1 = lemlib::Pose(-45, 90, -160);
    lemlib::Pose LOADER_BL_WAY2 = lemlib::Pose(-48, 22, 139);
    lemlib::Pose LOADER_BL = lemlib::Pose(-55, 24, 180);

    lemlib::Pose GOAL_L = lemlib::Pose(-53, 40, 180);

    lemlib::Pose PARK = lemlib::Pose(0, 12, 90);

    int TIMEOUT_SUPER_SHORT = 500;
    int TIMEOUT_SHORT = 1000;
    int TIMEOUT = 2000;
    int TIMEOUT_LONG = 3000;
    float MIN_SPEED = 80;
    float LEAD = 0.8;
};
