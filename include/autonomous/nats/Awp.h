#pragma once

#include "autonomous/Autonomous.h"
#include "pros/llemu.hpp"
#include "subsystems/Conveyer.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/Chassis.h"
#include "subsystems/Park.h"

class NatsAWP : public Autonomous {
    Conveyer* conveyer;
    MatchLoad* matchLoad;
    Park* park;

public:
    NatsAWP(Conveyer* conv, MatchLoad* ml)
        : conveyer(conv), matchLoad(ml){ }
 
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
        pros::delay(600);
        matchLoad->retract();
        conveyer->scoreLong();
        pros::delay(1500);
        //END OF RIGHT GOAL
        conveyer->intake();
        chassis.swingToHeading(-45, lemlib::DriveSide::RIGHT, 900);
        chassis.moveToPoint(STACK_RR.x , STACK_RR.y, TIMEOUT_SHORT, {
            .maxSpeed = 60
        });
        pros::delay(500);
        matchLoad->extend();
        pros::delay(400);
        matchLoad->retract();
        chassis.turnToHeading(-90, TIMEOUT_SUPER_SHORT);
        chassis.moveToPoint(STACK_B_MIDPOINT.x, STACK_B_MIDPOINT.y, TIMEOUT_SHORT);
        chassis.moveToPoint(STACK_B.x, STACK_B.y, 800,{
            .maxSpeed=60
        });
        chassis.turnToHeading(-135, TIMEOUT_SUPER_SHORT);
        chassis.moveToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, TIMEOUT_SHORT,{
            .forwards = false
        });
        
        chassis.turnToHeading(-135, 600);
        pros::delay(100);
        conveyer->scoreMiddleAuto();

        //END OF MIDDLE GOAL
        pros::delay(300);
        conveyer->stow();
        pros::delay(100);


        chassis.moveToPoint(GOAL_L_WAIT.x, GOAL_L_WAIT.y, TIMEOUT_SHORT);
        chassis.turnToHeading(-180, TIMEOUT_SUPER_SHORT);
        matchLoad->extend();
        chassis.moveToPoint(LOADER_BL.x, LOADER_BL.y, TIMEOUT_SHORT);
        pros::delay(100);
        chassis.moveToPoint(GOAL_L.x, GOAL_L.y, TIMEOUT_SHORT, {
            .forwards=false
        });
        pros::delay(100);
        matchLoad->retract();
        pros::delay(1000);
        conveyer->scoreLong();

    }

private: 
  
    const lemlib::Pose STARTING_POS{15.25, 24, 90};

    const lemlib::Pose LOADER_BR_WAY = lemlib::Pose(49.3, 24, 90);
    const lemlib::Pose LOADER_BR = lemlib::Pose(49.3, 11, -180);

    const lemlib::Pose STACK_RR{19.5,52,-135};
    // const lemlib::Pose STACK_B_MIDPOINT{-16, 45, -90};
    // const lemlib::Pose STACK_B{-26, 48, -90};
    // const lemlib::Pose GOAL_MIDDLE{-17,60,-135};

    const lemlib::Pose STACK_B_MIDPOINT{-16, 48, -90};
    const lemlib::Pose STACK_B{-23.5, 48, -90};
    const lemlib::Pose GOAL_MIDDLE{-16,60,-135};


    const lemlib::Pose GOAL_L_WAIT{-50,24, -135};
    const lemlib::Pose GOAL_L = lemlib::Pose(-51, 47, -180);

    const lemlib::Pose GOAL_R = lemlib::Pose(48, 48, -180);


    const lemlib::Pose LOADER_BL_WAY = lemlib::Pose(47, 33, 90);
    const lemlib::Pose LOADER_MID = lemlib::Pose (0, 32, 90);
    const lemlib::Pose LOADER_BL_WAY1 = lemlib::Pose(-49, 31, -160);
    const lemlib::Pose LOADER_BL = lemlib::Pose(-48, 4, 180);
    

    int TIMEOUT_SUPER_SHORT = 500;
    int TIMEOUT_SHORT = 1000;
    int TIMEOUT = 2000;
    int TIMEOUT_LONG = 3000;
    float MIN_SPEED = 80;
    float LEAD = 0.8;
};
