#pragma once

#include "autonomous/Autonomous.h"
#include "pros/llemu.hpp"
#include "subsystems/Conveyer.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/Chassis.h"
#include "subsystems/Park.h"

class NatsSkillsAWP : public Autonomous {
    Conveyer* conveyer;
    MatchLoad* matchLoad;
    Park* park;

public:
    NatsSkillsAWP(Conveyer* conv, MatchLoad* ml, Park* pk)
        : conveyer(conv), matchLoad(ml), park(pk) { }
 
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
        chassis.moveToPoint(LOADER_BR.x, LOADER_BR.y, TIMEOUT_SHORT);
        pros::delay(TIMEOUT);
        chassis.moveToPoint(GOAL_R.x, GOAL_R.y, TIMEOUT,{
            .forwards = false,
            .maxSpeed=100
        });
        pros::delay(500);
        matchLoad->retract();
        conveyer->scoreLong();
        pros::delay(TIMEOUT);
        //END OF RIGHT GOAL
        conveyer->intake();
        chassis.swingToHeading(-45, lemlib::DriveSide::RIGHT, 900);
        chassis.moveToPoint(STACK_RR.x , STACK_RR.y, TIMEOUT_SHORT, {
            .maxSpeed = 50
        });
        pros::delay(500);
        // matchLoad->extend();
        pros::delay(400);
        matchLoad->retract();
        chassis.turnToHeading(-90, TIMEOUT_SUPER_SHORT);
        chassis.moveToPoint(STACK_B_MIDPOINT.x, STACK_B_MIDPOINT.y, TIMEOUT_SHORT);
        chassis.moveToPoint(STACK_B.x, STACK_B.y, 800,{
            .maxSpeed=50
        });
        chassis.turnToHeading(-135, TIMEOUT_SUPER_SHORT);
        chassis.moveToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, TIMEOUT_SHORT,{
            .forwards = false
        });
        
        chassis.turnToHeading(-137, TIMEOUT_SUPER_SHORT);
        pros::delay(100);
        conveyer->scoreMiddleAuto();

        //END OF MIDDLE GOAL
        pros::delay(TIMEOUT);
        conveyer->stow();
        pros::delay(100);


        chassis.moveToPoint(GOAL_L_WAIT.x, GOAL_L_WAIT.y, TIMEOUT_SHORT);
        chassis.turnToHeading(-180, TIMEOUT_SUPER_SHORT);
        matchLoad->extend();
        chassis.moveToPoint(LOADER_BL.x, LOADER_BL.y, TIMEOUT_SHORT);
        pros::delay(TIMEOUT_LONG);
        chassis.moveToPoint(GOAL_L.x, GOAL_L.y, TIMEOUT, {
            .forwards=false,
            .maxSpeed=100
        });
        matchLoad->retract();
        pros::delay(1000);
        conveyer->scoreLong();
        pros::delay(TIMEOUT);

        chassis.moveToPoint(PARK_WAY.x, PARK_WAY.y, TIMEOUT);
        chassis.turnToHeading(90, TIMEOUT_SHORT);
        chassis.moveToPoint(PARK_MID.x, PARK_MID.y, TIMEOUT);
        chassis.turnToHeading(180, TIMEOUT_SHORT);
        chassis.moveToPoint(PARK.x, PARK.y, TIMEOUT);
        pros::delay(TIMEOUT);
        park->toggle();

    }

private: 
  
    const lemlib::Pose STARTING_POS{15.25, 24, 90};

    const lemlib::Pose LOADER_BR_WAY = lemlib::Pose(49.3, 24, 90);
    const lemlib::Pose LOADER_BR = lemlib::Pose(49.3, 11, -180);

    const lemlib::Pose STACK_RR{19.5,52,-135};
    const lemlib::Pose STACK_B_MIDPOINT{-16, 48, -90};
    const lemlib::Pose STACK_B{-23.5, 48, -90};
    const lemlib::Pose GOAL_MIDDLE{-16,60,-135};

    const lemlib::Pose GOAL_L_WAIT{-52,24, -135};
    const lemlib::Pose GOAL_L = lemlib::Pose(-52, 48, -180);

    const lemlib::Pose GOAL_R = lemlib::Pose(48, 48, -180);


    const lemlib::Pose LOADER_BL_WAY = lemlib::Pose(47, 33, 90);
    const lemlib::Pose LOADER_MID = lemlib::Pose (0, 32, 90);
    const lemlib::Pose LOADER_BL_WAY1 = lemlib::Pose(-53, 31, -160);
    const lemlib::Pose LOADER_BL = lemlib::Pose(-51, 6, 180);

     const lemlib::Pose PARK_WAY = lemlib::Pose(-51, 33, 180);
     const lemlib::Pose PARK_MID = lemlib::Pose(-3, 33, 90);
     const lemlib::Pose PARK = lemlib::Pose(-3, 16, 90);

    

    int TIMEOUT_SUPER_SHORT = 500;
    int TIMEOUT_SHORT = 1000;
    int TIMEOUT = 2000;
    int TIMEOUT_LONG = 3000;
    float MIN_SPEED = 80;
    float LEAD = 0.8;
};
