#pragma once

#include "autonomous/Autonomous.h"
#include "subsystems/Conveyer.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/Descore.h"
#include "subsystems/Chassis.h"

class StatesAwp : public Autonomous {
    Conveyer* conveyer;
    MatchLoad* matchLoad;

public:
    StatesAwp(Conveyer* conv, MatchLoad* ml)
        : conveyer(conv), matchLoad(ml) { }

    void run() override {
        chassis.setPose(STARTING_POS);

        chassis.moveToPose(
          GOAL_R.x, GOAL_R.y, GOAL_R.theta, 
          TIMEOUT,
          {
            .forwards = false
          }
        );
        // MOVE TO SECOND LOADER
        chassis.moveToPose(
          LOADER_TR_WAY.x, LOADER_TR_WAY.y, LOADER_TR_WAY.theta,
          TIMEOUT_SHORT,
          {
            .earlyExitRange = 4
          }
        );
        chassis.swingToHeading(
          -20,
        lemlib::DriveSide::RIGHT,
          500,
          {
          .minSpeed = MIN_SPEED, 
          .earlyExitRange = 15, 
        }
        );
        chassis.moveToPoint(
          LOADER_TR_WAY1.x, LOADER_TR_WAY1.y,
          TIMEOUT_SHORT,
          {
            .earlyExitRange = 4
          }
        );
        // //turning and going to loader
        // chassis.turnToHeading(20, 500);
        chassis.moveToPoint(
          LOADER_TR_WAY2.x, LOADER_TR_WAY2.y,
          TIMEOUT,
          {
            .minSpeed = MIN_SPEED, 
            .earlyExitRange = 4
          }
        );
        //turning moving to loader
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
        chassis.moveToPoint(LOADER_TL_WAY.x, LOADER_TL_WAY.y, TIMEOUT_SHORT);
        chassis.turnToHeading(-93, TIMEOUT_SHORT);
        chassis.moveToPoint(LOADER_TL_WAY1.x, LOADER_TL_WAY1.y, TIMEOUT_LONG);
        // To loader
        chassis.turnToHeading(0, TIMEOUT);
        chassis.moveToPose(LOADER_TL.x, LOADER_TL.y, LOADER_TL.theta, TIMEOUT,{
          .minSpeed=MIN_SPEED,
          .earlyExitRange = 4
        });

    }

private: 
    lemlib::Pose STARTING_POS{23.5, 7, 90};
    lemlib::Pose GOAL_L_WAIT{-53,24, -135};
    lemlib::Pose STACK_RR{20,52,-135};
    lemlib::Pose STACK_B_MIDPOINT{-16, 48, -90};
    lemlib::Pose STACK_B{-24, 47, -90};
    lemlib::Pose GOAL_MIDDLE{-14,57,-135};

    lemlib::Pose LOADER_BR_WAY = lemlib::Pose(48, 24, 90);
    lemlib::Pose LOADER_BR = lemlib::Pose(48, 13, -180);

    lemlib::Pose GOAL_R = lemlib::Pose(48, 44, -180);

    lemlib::Pose LOADER_TR_WAY = lemlib::Pose(45, 30, -180);//drive forward
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

    int TIMEOUT_SUPER_SHORT = 500;
    int TIMEOUT_SHORT = 1000;
    int TIMEOUT = 2000;
    int TIMEOUT_LONG = 3000;
    float MIN_SPEED = 80;
    float LEAD = 0.8;
};
