#pragma once

#include "autonomous/Autonomous.h"
#include "pros/llemu.hpp"
#include "subsystems/Conveyer.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/Chassis.h"

class StatesLongGoal : public Autonomous {
    Conveyer* conveyer;
    MatchLoad* matchLoad;

public:
    StatesLongGoal(Conveyer* conv, MatchLoad* ml)
        : conveyer(conv), matchLoad(ml) { }

    void run() override {
        // Check subsystems are available
        if (!conveyer || !matchLoad) {
            pros::lcd::print(0, "ERROR: Missing subsystems!");
            return;
        }

        chassis.setPose(STARTING_POS);
        conveyer->intake();
        chassis.moveToPoint( STACK1.x, STACK1.y,  TIMEOUT, {
                // .minSpeed = MIN_SPEED, 
                .maxSpeed=80,
                .earlyExitRange = 4
            });
        chassis.moveToPose(STACK2.x, STACK2.y, STACK2.theta, TIMEOUT);
        chassis.moveToPoint(GOAL_WAYPOINTS.x, GOAL_WAYPOINTS.y,TIMEOUT, {
                .forwards = false,
                .minSpeed = MIN_SPEED, 
                .earlyExitRange = 4
            });
        chassis.moveToPose(GOAL.x, GOAL.y, GOAL.theta, TIMEOUT_LONG,{
                .forwards = false
            });
        conveyer->scoreLong();
        pros::c::delay(2000);
        matchLoad->extend();
        conveyer->intake();
        chassis.moveToPoint(MATCHLOAD.x, MATCHLOAD.y,   TIMEOUT);
        pros::c::delay(900);
        chassis.moveToPose(GOAL.x, GOAL.y, GOAL.theta, TIMEOUT_LONG,{
                .forwards = false
            });
        conveyer->scoreLong();
    }

private: 
    const lemlib::Pose STARTING_POS{20.25, 16.25, 0};
    const lemlib::Pose STACK1{24, 48, 20};
    const lemlib::Pose STACK2{44, 60, 75};
    const lemlib::Pose GOAL_WAYPOINTS{32, 36, 0};
    const lemlib::Pose GOAL{48, 40, -180};
    const lemlib::Pose MATCHLOAD{GOAL.x, 8, -180};

    const int TIMEOUT = 2000;
    const int TIMEOUT_LONG = 3000;
    const float MIN_SPEED = 80;

};
