#pragma once

#include "autonomous/Autonomous.h"
#include "lemlib/chassis/chassis.hpp"

#include "subsystems/Chassis.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/Passthrough.h"
#include "subsystems/Wing.h"
#include "subsystems/MidDescore.h"

class L_LW : public Autonomous
{
    Passthrough *passthrough;
    MatchLoad *matchLoad;
    Wing *wing;
    MidDescore *midDescore;

public:
    L_LW(Passthrough *pst, MatchLoad *ml, Wing *wg, MidDescore *md)
        : passthrough(pst), matchLoad(ml), wing(wg), midDescore(md) {}

    void run() override
    {
        chassis.setPose(0, 0, 0);
        pros::delay(100);
        chassis.setPose(STARTING_POS);

        chassis.moveToPoint(LOADER_BL_WAY.x, LOADER_BL_WAY.y, TIMEOUT_SHORT);
        chassis.turnToHeading(-180, TIMEOUT_SUPER_SHORT);

        chassis.moveToPoint(LOADER_BL.x, LOADER_BL.y, TIMEOUT_SHORT);
        pros::delay(2100);
        chassis.moveToPoint(GOAL_L.x, GOAL_L.y, TIMEOUT,
                            {.forwards = false, .maxSpeed = 100});
        chassis.moveToPoint(GOAL_QUICK.x, GOAL_QUICK.y, TIMEOUT_SHORT, {.maxSpeed = 80});
        pros::delay(1000);
        chassis.turnToHeading(-180, 900);
        chassis.moveToPoint(WING_LEFT.x, WING_LEFT.y, TIMEOUT_SHORT, {.forwards = false, .minSpeed = 80, .earlyExitRange = 8});
    }

private:
    const lemlib::Pose WING_LEFT = lemlib::Pose(-58, 45, -90);

    const lemlib::Pose GOAL_QUICK = lemlib::Pose(-58, 31, -90);

    const lemlib::Pose STARTING_POS{-15.25, 24, -90};

    const lemlib::Pose LOADER_BL_WAY = lemlib::Pose(-48, 24, -90);
    const lemlib::Pose LOADER_BL = lemlib::Pose(-48, 11, -180);

    const lemlib::Pose STACK_RR{19.5, 48, -135};
    const lemlib::Pose STACK_B_MIDPOINT{-16, 46, -90};
    const lemlib::Pose STACK_B{-23.5, 48, -90};
    const lemlib::Pose GOAL_MIDDLE{-10, 56, -135};

    const lemlib::Pose GOAL_L_WAIT{-50, 24, -135};
    const lemlib::Pose GOAL_L = lemlib::Pose(-47, 47, -180);
    const lemlib::Pose GOAL_R = lemlib::Pose(48, 48, -180);

    const lemlib::Pose LOADER_BR_WAY = lemlib::Pose(48, 17, 90);
    const lemlib::Pose LOADER_MID = lemlib::Pose(0, 32, 90);
    const lemlib::Pose LOADER_BR_WAY1 = lemlib::Pose(49, 30, 160);
    const lemlib::Pose LOADER_BR = lemlib::Pose(48, 6, 180);

    const lemlib::Pose PARK_WAY = lemlib::Pose(-51, 33, 180);
    const lemlib::Pose PARK_MID = lemlib::Pose(-3, 33, 90);
    const lemlib::Pose PARK = lemlib::Pose(-3, 16, 90);

    const lemlib::Pose HIT_LEFT = lemlib::Pose(-52, 38, -180);
    const lemlib::Pose HIT_RIGHT = lemlib::Pose(48, 39, -180);

    int TIMEOUT_SUPER_SHORT = 500;
    int TIMEOUT_SHORT = 1000;
    int TIMEOUT = 2000;
    int TIMEOUT_LONG = 3000;
    float MIN_SPEED = 80;
    float LEAD = 0.8;
};