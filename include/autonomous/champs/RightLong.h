#pragma once

#include "autonomous/Autonomous.h"
#include "pros/llemu.hpp"
#include "subsystems/Chassis.h"


class ChampsRightLong : public Autonomous {


public:


    void run() override {
        
        
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