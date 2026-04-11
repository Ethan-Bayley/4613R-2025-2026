#pragma once

#include "autonomous/Autonomous.h"
#include "subsystems/Chassis.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/Passthrough.h"



class ChampsLeftLongMiddle : public Autonomous {
    Passthrough *passthrough;
    MatchLoad *matchload;

public: 
    ChampsLeftLongMiddle(Passthrough *pst, MatchLoad *ml)
        : passthrough(pst), matchload(ml) {}

    void run() override {
        pros::Task passthroughUpdate([&]() {
            while(true) {
                passthrough->update();
                pros::delay(20);
            }
        });
        chassis.setPose(0, 0, 0);
        chassis.setPose(STARTING_POS_LONG_PUSH);

        passthrough->hoodRaised();
        passthrough->intake();

        chassis.moveToPoint(STACK_LL.x, STACK_LL.y, TIMEOUT);
        matchload->extend();
        matchload->retract();

        chassis.moveToPoint(UNDER_GOAL_L.x, UNDER_GOAL_L.y, 1500);
        matchload->extend();
	    pros::delay(500);
        matchload->retract();

        chassis.moveToPoint(GOAL_L_WAY.x, GOAL_L_WAY.y, 1500,
                        {.forwards = false});

        chassis.turnToPoint(GOAL_L.x, GOAL_L.y, TIMEOUT_SUPER_SHORT,
                        {.forwards = false});

        chassis.moveToPoint(GOAL_L.x, GOAL_L.y, TIMEOUT_SHORT);

        passthrough->scoreLong();





        
        matchload->extend();
        passthrough->intake();

        chassis.moveToPoint(LOADER_BL.x, LOADER_BL.y, TIMEOUT_SHORT);
        pros::delay(1000);
        matchload->retract();
        
        chassis.moveToPoint(LOADER_BL_WAY.x, LOADER_BL_WAY.y, TIMEOUT_SUPER_SHORT);
        chassis.turnToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, TIMEOUT_SUPER_SHORT,
                        {.forwards = false});
        chassis.moveToPoint(GOAL_MIDDLE.x, GOAL_MIDDLE.y, TIMEOUT_SUPER_SHORT, 
                        {.forwards = false, .maxSpeed = 75}, true);
        passthrough->scoreLong();

    }

private:
    const lemlib::Pose STARTING_POS_LONG_PUSH{-17, 22, -15};
    const lemlib::Pose UNDER_GOAL_L = lemlib::Pose(-32, 72, -60);

    const lemlib::Pose STACK_LL{-19.5, 48, 135};

    const lemlib::Pose LOADER_BL_WAY = lemlib::Pose(-25, 24, -90);
    
    const lemlib::Pose LOADER_BL = lemlib::Pose(-34, 6, 180);

    const lemlib::Pose GOAL_L_WAY = lemlib::Pose(-25, 40, 0);
    const lemlib::Pose GOAL_L = lemlib::Pose(-36, 47, -180);
	const lemlib::Pose GOAL_MIDDLE{-5, 60, -135};

    const lemlib::Pose WING_WAY = lemlib::Pose(-30, 44, 180);
    const lemlib::Pose WING_END = lemlib::Pose(-30, 55, 180);

    int TIMEOUT_SUPER_SHORT = 500;
    int TIMEOUT_SHORT = 1000;
    int TIMEOUT = 2000;
    int TIMEOUT_LONG = 3000;
    float MIN_SPEED = 80;
    float LEAD = 0.8;
};
