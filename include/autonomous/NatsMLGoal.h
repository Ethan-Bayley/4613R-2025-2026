#pragma once

#include "autonomous/Autonomous.h"
#include "pros/llemu.hpp"
#include "subsystems/Conveyer.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/Chassis.h"

class NatsMLGoal : public Autonomous {
    Conveyer* conveyer;
    MatchLoad* matchLoad;

public:
    NatsMLGoal(Conveyer* conv, MatchLoad* ml)
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
                .maxSpeed=60,
                .earlyExitRange = 4
            });
        chassis.turnToHeading(-45, TIMEOUT_SUPERSHORT);
        chassis.moveToPoint(STACK2.x, STACK2.y, 1300, {
                .maxSpeed=70
            });
        chassis.moveToPoint(MIDDLEGOAL_WAY.x, MIDDLEGOAL_WAY.y, TIMEOUT_SHORT, {
            .forwards=false
        });
        chassis.turnToHeading(-130, TIMEOUT_SUPERSHORT);
        chassis.moveToPoint(MIDDLEGOAL.x, MIDDLEGOAL.y, TIMEOUT, {
            .forwards=false
        });
        conveyer->scoreMiddle();
        chassis.moveToPoint(GOAL_WAY.x, GOAL_WAY.y, TIMEOUT);
        conveyer->stop();
        chassis.turnToHeading(180, TIMEOUT_SUPERSHORT);
        matchLoad->extend();
        conveyer->intake();
        chassis.moveToPoint(MATCHLOAD.x, MATCHLOAD.y,   TIMEOUT);
        pros::c::delay(700);
        chassis.moveToPoint(GOAL.x, GOAL.y, TIMEOUT_LONG,{
                .forwards = false
            });
        matchLoad->retract();
        conveyer->scoreLong();
    }

private: 
    const lemlib::Pose STARTING_POS{-20.25, 16.25, 0};
    const lemlib::Pose STACK1{-26, 50, -20};
    const lemlib::Pose STACK2{-47, 63, -60};

    const lemlib::Pose MIDDLEGOAL_WAY{-24, 48, -60};
    const lemlib::Pose MIDDLEGOAL{-17,58,-135};

    const lemlib::Pose GOAL_WAY{-5423, 24, -60};
    const lemlib::Pose GOAL{-54, 39, 180};
    const lemlib::Pose MATCHLOAD{GOAL.x, 5, -180};

    const int TIMEOUT_SUPERSHORT = 500;
    const int TIMEOUT_SHORT = 1000;
    const int TIMEOUT = 2000;
    const int TIMEOUT_LONG = 3000;
    const float MIN_SPEED = 80;


};
