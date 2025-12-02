#pragma once

#include "autonomous/Autonomous.h"
#include "pros/llemu.hpp"
#include "subsystems/Conveyer.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/Chassis.h"

class NatsLongGoalR : public Autonomous {
    Conveyer* conveyer;
    MatchLoad* matchLoad;

public:
    NatsLongGoalR(Conveyer* conv, MatchLoad* ml)
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
                .maxSpeed=50,
                .earlyExitRange = 4
            });
        chassis.turnToHeading(48, TIMEOUT_SUPERSHORT);
        chassis.moveToPoint(STACK2.x, STACK2.y, 1300, {
                .maxSpeed=70
            });
        chassis.moveToPoint(GOAL_WAY.x, GOAL_WAY.y, TIMEOUT_SHORT,{
            .forwards=false,
        });
        chassis.turnToHeading(0, 300);
        conveyer->stop();
        chassis.moveToPoint(GOAL_WAYPOINTS.x, GOAL_WAYPOINTS.y, TIMEOUT, {
                .forwards = false,
                .minSpeed = MIN_SPEED, 
            });
        chassis.turnToHeading(-90, TIMEOUT_SHORT);
        chassis.moveToPoint(GOAL_WAY3.x, GOAL_WAY3.y, TIMEOUT_SHORT, {
            .forwards=false
        });
        chassis.turnToHeading(180, TIMEOUT_SUPERSHORT);
        chassis.moveToPoint(GOAL.x, GOAL.y, TIMEOUT_LONG,{
                .forwards = false
            });
        conveyer->scoreLong();
        pros::c::delay(1500);
        matchLoad->extend();
        conveyer->intake();
        chassis.moveToPoint(MATCHLOAD.x, MATCHLOAD.y,   1700);
        chassis.moveToPoint(GOAL1.x, GOAL1.y, TIMEOUT_LONG,{
                .forwards = false
            });
        matchLoad->retract();
        pros::c::delay(TIMEOUT-700);
        conveyer->scoreLong();
    }

private: 
    const lemlib::Pose STARTING_POS{20.25, 16.25, 0};
    const lemlib::Pose STACK1{27, 50, -20};
    const lemlib::Pose STACK2{48, 61, -60};
    const lemlib::Pose GOAL_WAY{40, 50, -60};
    const lemlib::Pose GOAL_WAYPOINTS{40, 36, 0};
    const lemlib::Pose GOAL_WAY3{51, 34};
    const lemlib::Pose GOAL{52, 39, 180};
    const lemlib::Pose GOAL1{55, 39, 180};
    const lemlib::Pose MATCHLOAD{GOAL.x, 7, -180};

    const int TIMEOUT_SUPERSHORT = 500;
    const int TIMEOUT_SHORT = 1000;
    const int TIMEOUT = 2000;
    const int TIMEOUT_LONG = 3000;
    const float MIN_SPEED = 80;


};
