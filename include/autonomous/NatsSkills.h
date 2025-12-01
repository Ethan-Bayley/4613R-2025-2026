#pragma once

#include "autonomous/Autonomous.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "subsystems/Conveyer.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/Chassis.h"
#include <cmath>

class NatsSkills : public Autonomous {
    Conveyer* conveyer;
    MatchLoad* matchLoad;

public:
    NatsSkills(Conveyer* conv, MatchLoad* ml)
        : conveyer(conv), matchLoad(ml) { }

    void run() override {
        chassis.setPose(STARTING_POS);
        chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
        conveyer->intake();
        chassis.moveToPoint(LOADER_BR_WAY.x, LOADER_BR_WAY.y, TIMEOUT, {
          .maxSpeed=40
        });

        chassis.turnToHeading(-180, TIMEOUT_SUPER_SHORT);
        matchLoad->extend();
        chassis.moveToPoint(LOADER_BR.x, LOADER_BR.y, TIMEOUT_SHORT);
        pros::delay(2500);
        chassis.moveToPoint(GOAL_R.x, GOAL_R.y, TIMEOUT,{
            .forwards = false
        });
        pros::delay(1000);
        conveyer->scoreLong();
        pros::delay(3000);
        matchLoad->retract();
        conveyer->stop();

        chassis.moveToPoint(LOADER_BL_WAY.x, LOADER_BL_WAY.y, TIMEOUT_SHORT);
        chassis.turnToHeading(-90, TIMEOUT_SHORT);
        chassis.moveToPoint(LOADER_MID.x, LOADER_MID.y, TIMEOUT);

        chassis.moveToPoint(LOADER_BL_WAY1.x, LOADER_BL_WAY1.y, TIMEOUT, {
            .maxSpeed=80
          });
        chassis.turnToHeading(180, TIMEOUT_SHORT);
        matchLoad->extend();
        conveyer->intake();
        chassis.moveToPoint(LOADER_BL.x, LOADER_BL.y, TIMEOUT);

        //chassis.moveToPoint(GOAL_L.x, GOAL_R.y, TIMEOUT);
        // conveyer->scoreLong();
        // pros::delay(2000);
        
        // chassis.moveToPoint(PARK_WAY.x, PARK_WAY.y, TIMEOUT, {
        //   .forwards=false
        // });
        // chassis.turnToHeading(110, TIMEOUT_SUPER_SHORT);
        // matchLoad->retract();

        // chassis.moveToPoint(PARK.x, PARK.y, TIMEOUT, {
        //   .maxSpeed=127
        // });
      }

private: 
    lemlib::Pose STARTING_POS{15.25, 24, 90};

    lemlib::Pose LOADER_BR_WAY = lemlib::Pose(49.3, 24, 90);
    lemlib::Pose LOADER_BR = lemlib::Pose(49.3, 11, -180);

    lemlib::Pose GOAL_R = lemlib::Pose(47, 48, -180);
    lemlib::Pose GOAL_L = lemlib::Pose(-53, 48, -180);

    lemlib::Pose LOADER_BL_WAY = lemlib::Pose(47, 35, 90);
    Lemlib::Pose LOADER_MID = lemblib::Pose (0, 34, 90);
    lemlib::Pose LOADER_BL_WAY1 = lemlib::Pose(-53, 33, -160);
    lemlib::Pose LOADER_BL = lemlib::Pose(-53, 12, 180);

    lemlib::Pose PARK_WAY = lemlib::Pose(-53, 33, 180);
    lemlib::Pose PARK = lemlib::Pose(10, 10, 90);

    int TIMEOUT_SUPER_SHORT = 500;
    int TIMEOUT_SHORT = 1000;
    int TIMEOUT = 2000;
    int TIMEOUT_LONG = 3000;
    float MIN_SPEED = 80;
    float LEAD = 0.8;
};
