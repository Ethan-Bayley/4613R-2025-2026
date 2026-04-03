#pragma once

#include "autonomous/Autonomous.h"
#include "subsystems/Chassis.h"

class ChampsLeftLongPush : public Autonomous {

public:

    void run() override {
        chassis.setPose(0, 0, 0);
        chassis.setPose(STARTING_POS_LONG_PUSH);

        chassis.moveToPoint(STACK_LL.x, STACK_LL.y, TIMEOUT);
        chassis.turnToHeading(-60, 500);

        chassis.moveToPoint(UNDER_GOAL_L.x, UNDER_GOAL_L.y, 1500);
        chassis.moveToPoint(STACK_LL.x, STACK_LL.y, TIMEOUT_SHORT, {.forwards = false, .minSpeed = 80, .earlyExitRange = 8});
        chassis.moveToPoint(LOADER_BL_WAY.x, LOADER_BL_WAY.y, TIMEOUT_SHORT);

        chassis.turnToHeading(180, 900);
        chassis.moveToPoint(LOADER_BL.x, LOADER_BL.y, TIMEOUT_SHORT);

        chassis.moveToPose(GOAL_L.x, GOAL_L.y, 180, TIMEOUT, {.forwards = false});
    }

private:
    const lemlib::Pose STARTING_POS_LONG_PUSH{-17, 22, -15};
    const lemlib::Pose UNDER_GOAL_L = lemlib::Pose(-40, 55, -60);

    const lemlib::Pose STACK_LL{-19.5, 48, 135};

    const lemlib::Pose LOADER_BL_WAY = lemlib::Pose(-48, 24, -90);
    const lemlib::Pose LOADER_BL = lemlib::Pose(-48, 11, -180);

    const lemlib::Pose GOAL_L = lemlib::Pose(-48, 48, -180);

    int TIMEOUT_SUPER_SHORT = 500;
    int TIMEOUT_SHORT = 1000;
    int TIMEOUT = 2000;
    int TIMEOUT_LONG = 3000;
    float MIN_SPEED = 80;
    float LEAD = 0.8;
};