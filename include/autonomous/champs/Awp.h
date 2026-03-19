#pragma once

#include "autonomous/Autonomous.h"
#include "pros/llemu.hpp"
//#include "subsystems/Conveyer.h"
//#include "subsystems/MatchLoad.h"
#include "subsystems/Chassis.h"

class StatesAwp : public Autonomous {


public:


    void run() override {


        chassis.setPose(STARTING_POS);
        chassis.moveToPoint(LOADER_BR_WAY.x, LOADER_BR_WAY.y, TIMEOUT_SHORT);
        chassis.moveToPoint(Loader_BR_WAY.x, LOADERBR_WAY)
    }

private: 
    const lemlib::Pose STARTING_POS{15, 24, 90};

    const lemlib::Pose LOADER_BR_WAY {48, 24, 90};
    const lemlib::Pose LOADER_BR = {48, 11, -180};
    const lemlib::Pose GOAL_R = lemlib::Pose{47, 44, -180};
 
    const lemlib::Pose STACK_RR{20,52,-135};
    const lemlib::Pose STACK_B_MIDPOINT{-16, 48, -90};
    const lemlib::Pose STACK_B{-24, 47, -90};
    const lemlib::Pose GOAL_MIDDLE{-14,60,-135};


    const lemlib::Pose GOAL_L_WAIT{-53,24, -135};
    const lemlib::Pose GOAL_L = lemlib::Pose{-54, 40, 180};

    

    int TIMEOUT_SUPER_SHORT = 500;
    int TIMEOUT_SHORT = 1000;
    int TIMEOUT = 2000;
    int TIMEOUT_LONG = 3000;
    float MIN_SPEED = 80;
    float LEAD = 0.8;
};
