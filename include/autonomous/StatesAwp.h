#pragma once

#include "autonomous/Autonomous.h"
#include "pros/llemu.hpp"
#include "subsystems/Conveyer.h"
#include "subsystems/MatchLoad.h"
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
        chassis.moveToPoint(LOADER_BR.x, LOADER_BR.y, TIMEOUT_SHORT,{}
        );
        chassis.moveToPoint(GOAL_R.x, GOAL_R.y, 800,{
            .forwards = false
        });
        pros::delay(500);
        matchLoad->retract();
        conveyer->scoreLong();
        pros::delay(1500);
        //END OF RIGHT GOAL
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
        pros::delay(500);
        conveyer->stow();
        chassis.moveToPoint(GOAL_L_WAIT.x, GOAL_L_WAIT.y, TIMEOUT_SHORT);
        chassis.turnToHeading(-180, TIMEOUT_SUPER_SHORT);
        // chassis.moveToPoint(LOADER_BL.x, LOADER_BR.y, TIMEOUT_SHORT);
        chassis.moveToPoint(GOAL_L.x, GOAL_L.y, TIMEOUT, {
            .forwards=false
        });
        conveyer->scoreLong();

    }

private: 
    const lemlib::Pose STARTING_POS{15, 24, 90};
    const lemlib::Pose GOAL_L_WAIT{-53,24, -135};
    const lemlib::Pose STACK_RR{20,52,-135};
    const lemlib::Pose STACK_B_MIDPOINT{-16, 48, -90};
    const lemlib::Pose STACK_B{-24, 47, -90};
    const lemlib::Pose GOAL_MIDDLE{-14,60,-135};

    const lemlib::Pose LOADER_BR_WAY {48, 24, 90};
    const lemlib::Pose LOADER_BR = {48, 11, -180};

    const lemlib::Pose GOAL_R = lemlib::Pose{47, 44, -180};
    const lemlib::Pose GOAL_L = lemlib::Pose{-55, 40, 180};

    int TIMEOUT_SUPER_SHORT = 500;
    int TIMEOUT_SHORT = 1000;
    int TIMEOUT = 2000;
    int TIMEOUT_LONG = 3000;
    float MIN_SPEED = 80;
    float LEAD = 0.8;
};
