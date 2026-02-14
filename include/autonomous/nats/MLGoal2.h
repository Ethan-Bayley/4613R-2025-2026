#pragma once

#include "autonomous/Autonomous.h"
#include "pros/llemu.hpp"
#include "subsystems/Conveyer.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/Chassis.h"
#include "subsystems/Descore.h"

class NatsMLGoal2 : public Autonomous {
    Conveyer* conveyer;
    MatchLoad* matchLoad;
    Descore* descore;

public:
    NatsMLGoal2(Conveyer* conv, MatchLoad* ml, Descore* ds)
        : conveyer(conv), matchLoad(ml), descore(ds) { }

    void run() override {
        // Check subsystems are available
        if (!conveyer || !matchLoad) {
            pros::lcd::print(0, "ERROR: Missing subsystems!");
            return;
        }

        chassis.setPose(STARTING_POS);
        conveyer->intake();
        chassis.moveToPoint( STACK1.x, STACK1.y,  1300, {
                .maxSpeed=60,
            });
        pros::delay(800);
        matchLoad->extend();
        pros::delay(400);
        matchLoad->retract();

        chassis.moveToPoint(MIDDLEGOAL_WAY2.x, MIDDLEGOAL_WAY2.y, TIMEOUT_SUPERSHORT, {
            .forwards = false
        });

        // chassis.turnToHeading(-128, 400);
        chassis.turnToHeading(-135, 400);
        chassis.moveToPoint(MIDDLEGOAL.x, MIDDLEGOAL.y,  500, {
            .forwards=false
        });

        chassis.turnToHeading(-138, TIMEOUT_SHORT);
        pros::delay(100);
        conveyer->scoreMiddleAuto();
        pros::delay(400);
        conveyer->stow();
        pros::delay(100);

        chassis.turnToHeading(-130, TIMEOUT_SUPERSHORT);
        chassis.moveToPoint(GOAL_WAY.x, GOAL_WAY.y, TIMEOUT_SHORT);

        chassis.turnToHeading(180, TIMEOUT_SUPERSHORT);
        conveyer->intake();
        matchLoad->extend();
        chassis.moveToPoint(MATCHLOAD.x, MATCHLOAD.y, 1100);
        pros::delay(150);
        chassis.moveToPoint(GOAL.x, GOAL.y, 1500, {
            .forwards=false
        });
        pros::delay(400);
        conveyer->scoreLong();
        pros::delay(TIMEOUT);
        conveyer->stop();
        matchLoad->retract();
        
        chassis.moveToPoint(GOAL_WAY.x, GOAL_WAY.y, TIMEOUT_SHORT);


        pros::delay(TIMEOUT_SHORT);
        descore->toggle();
        chassis.turnToHeading(130, TIMEOUT_SUPERSHORT);
        chassis.moveToPoint(CZ1.x, CZ1.y, TIMEOUT_SHORT, {
            .forwards=false
        });
        descore->toggle();
        chassis.turnToHeading(180, TIMEOUT_SUPERSHORT);

        chassis.moveToPoint(CONTROLZONE.x, CONTROLZONE.y, TIMEOUT, {
            .forwards=false,
            .maxSpeed=60
        });
        pros::delay(2000);
        descore->toggle();
    }

private: 
    const lemlib::Pose STARTING_POS{-20.25, 16.25, 0};
    const lemlib::Pose STACK1{-28, 50, -20};

    const lemlib::Pose MIDDLEGOAL_WAY{-24, 48, 135};

    const lemlib::Pose MIDDLEGOAL_WAY2 {-23.3, 46.6, -135};
    const lemlib::Pose MIDDLEGOAL{-10, 57.5,-135};

    const lemlib::Pose GOAL_WAY{-51, 25, -60};
    const lemlib::Pose GOAL{-53, 39, 180};
    const lemlib::Pose MATCHLOAD{-51, 6, 180};

    const lemlib::Pose CZ{-51, 25, -60};
    const lemlib::Pose CZ1{-65, 38, 150};
    const lemlib::Pose CONTROLZONE{-65, 60, 180};

    const int TIMEOUT_SUPERSHORT = 500;
    const int TIMEOUT_SHORT = 1000;
    const int TIMEOUT = 2000;
    const int TIMEOUT_LONG = 3000;
    const float MIN_SPEED = 80;


};
