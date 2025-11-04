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
        // Check subsystems are available
        if (!conveyer || !matchLoad) {
            pros::lcd::print(0, "ERROR: Missing subsystems!");
            return;
        }

        chassis.setPose(STARTING_POS);
        conveyer->intake();
        chassis.moveToPoint(LOADER_BR_WAY.x, LOADER_BR_WAY.y, TIMEOUT_SHORT);
        chassis.turnToHeading(-180, TIMEOUT_SUPER_SHORT);
        matchLoad->extend();
        chassis.moveToPoint(LOADER_BR.x, LOADER_BR.y, TIMEOUT,{}
        );
        pros::delay(1000);
        chassis.moveToPoint(GOAL_R.x, GOAL_R.y, TIMEOUT_SHORT,{
            .forwards = false
        });
        //END OF RIGHT GOAL
        matchLoad->retract();
        conveyer->scoreLong();
        pros::delay(1000);
        conveyer->intake();
        chassis.swingToHeading(-45, lemlib::DriveSide::RIGHT, TIMEOUT_SHORT);
        chassis.moveToPoint(STACK_RR.x , STACK_RR.y, TIMEOUT_SHORT, {
            .maxSpeed = 60
        });
        pros::delay(400);
        chassis.turnToHeading(-90, TIMEOUT_SUPER_SHORT);
        chassis.moveToPoint(STACK_B_MIDPOINT.x, STACK_B_MIDPOINT.y, TIMEOUT_SHORT);
        chassis.moveToPoint(STACK_B.x, STACK_B.y, TIMEOUT_SHORT,{
            .maxSpeed=60
        });
        chassis.turnToHeading(-135, TIMEOUT_SUPER_SHORT);
        chassis.moveToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, TIMEOUT_SHORT,{
            .forwards = false
        });
        conveyer->scoreMiddle();
        chassis.turnToHeading(-137, TIMEOUT_SUPER_SHORT);
        //END OF MIDDLE GOAL
        conveyer->intake();
        chassis.moveToPoint(GOAL_L_WAIT.x, GOAL_L_WAIT.y, TIMEOUT_SHORT);
        chassis.turnToHeading(-180, TIMEOUT_SUPER_SHORT);
        // chassis.moveToPoint(LOADER_BL.x, LOADER_BR.y, TIMEOUT_SHORT);
        chassis.moveToPoint(GOAL_L.x, GOAL_L.y, TIMEOUT, {
            .forwards=false
        });
        conveyer->scoreLong();

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
