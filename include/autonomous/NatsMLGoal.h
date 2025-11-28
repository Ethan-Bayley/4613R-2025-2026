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
        : conveyer(conv), matchLoad(ml) { }//67

    void run() override {
        // Check subsystems are available
        if (!conveyer || !matchLoad) {
            pros::lcd::print(0, "ERROR: Missing subsystems!");
            return;
        }

        chassis.setPose(STARTING_POS);
        conveyer->intake();
        chassis.moveToPoint( STACK1.x, STACK1.y,  1100, {
                .maxSpeed=60,
            });
        chassis.turnToHeading(MIDDLEGOAL.theta, TIMEOUT_SHORT);
        chassis.moveToPoint(MIDDLEGOAL.x, MIDDLEGOAL.y,  500, {
            .forwards=false
        });
        chassis.turnToHeading(225, TIMEOUT_SUPERSHORT + 100);
        conveyer->scoreMiddleAuto();
        pros::delay(TIMEOUT_SUPERSHORT);
        conveyer->intake();
        chassis.moveToPoint( STACK1.x, STACK1.y,  TIMEOUT_SUPERSHORT, {
            });
        chassis.turnToHeading(-45, TIMEOUT_SUPERSHORT);
        chassis.moveToPoint(STACK2.x, STACK2.y, 1200, {
                .maxSpeed=70
            });
        chassis.moveToPoint(STACK1.x, STACK1.y,  TIMEOUT_SHORT, {
            .forwards = false
        });

        chassis.turnToHeading(-130, TIMEOUT_SUPERSHORT);
        chassis.moveToPoint(GOAL_WAY.x, GOAL_WAY.y, TIMEOUT_SHORT);

        chassis.turnToHeading(180, TIMEOUT_SUPERSHORT);

        chassis.moveToPoint(GOAL.x, GOAL.y, TIMEOUT_SHORT, {
            .forwards = false
        });

        pros::delay(500);

        conveyer->scoreLong();

        pros::delay(700);

        chassis.turnToHeading(180, TIMEOUT_SUPERSHORT);
        pros::delay(TIMEOUT_SUPERSHORT);
        conveyer->intake();
        matchLoad->extend();
        chassis.moveToPoint(MATCHLOAD.x, MATCHLOAD.y, TIMEOUT_SHORT);
        chassis.turnToHeading(MATCHLOAD.theta, TIMEOUT_SUPERSHORT);
        chassis.moveToPoint(GOAL.x, GOAL.y, TIMEOUT_SHORT, {
            .forwards=false
        });
        conveyer->scoreLong();
    }

private: 
    const lemlib::Pose STARTING_POS{-20.25, 16.25, 0};
    const lemlib::Pose STACK1{-27, 50, -20};
    const lemlib::Pose STACK2{-46, 64, -60};

    const lemlib::Pose MIDDLEGOAL_WAY{-24, 48, 135};
    const lemlib::Pose MIDDLEGOAL{-13.5, 55.5,-135};

    const lemlib::Pose GOAL_WAY{-52, 25, -60};
    const lemlib::Pose GOAL{-53, 39, 180};
    const lemlib::Pose MATCHLOAD{GOAL.x, 8, 180};

    const int TIMEOUT_SUPERSHORT = 500;
    const int TIMEOUT_SHORT = 1000;
    const int TIMEOUT = 2000;
    const int TIMEOUT_LONG = 3000;
    const float MIN_SPEED = 80;


};
