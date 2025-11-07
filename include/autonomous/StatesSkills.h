#pragma once

#include "autonomous/Autonomous.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "subsystems/Conveyer.h"
#include "subsystems/MatchLoad.h"
#include "subsystems/Chassis.h"
#include <cmath>

class StatesSkills : public Autonomous {
    Conveyer* conveyer;
    MatchLoad* matchLoad;

public:
    StatesSkills(Conveyer* conv, MatchLoad* ml)
        : conveyer(conv), matchLoad(ml) { }

    void run() override {

        chassis.setPose(STARTING_POS);
        conveyer->intake();
        chassis.moveToPoint(LOADER_BR_WAY.x, LOADER_BR_WAY.y, TIMEOUT_SHORT);
        chassis.turnToHeading(-180, TIMEOUT_SUPER_SHORT);
        matchLoad->extend();
        chassis.moveToPoint(LOADER_BR.x, LOADER_BR.y, TIMEOUT_SHORT);
        pros::delay(2500);
        chassis.moveToPoint(GOAL_R.x, GOAL_R.y, 5000,{
            .forwards = false
        });
        pros::delay(1000);
        conveyer->scoreLong();
        pros::delay(2000);
        matchLoad->retract();

        // Finish first score

        conveyer->stop();
        chassis.swingToHeading(-45, lemlib::DriveSide::RIGHT, TIMEOUT_SHORT);

      // MOVE TO SECOND LOADER
      chassis.moveToPoint(LOADER_TR_WAY.x, LOADER_TR_WAY.y, TIMEOUT_SHORT);
      chassis.turnToHeading(0,TIMEOUT_SUPER_SHORT);

      chassis.moveToPoint(
        LOADER_TR_WAY1.x, LOADER_TR_WAY1.y, TIMEOUT, {
          .maxSpeed=80
        });
      chassis.turnToHeading(90,TIMEOUT_SHORT);
      chassis.moveToPoint(
        LOADER_TR_WAY2.x, LOADER_TR_WAY2.y, TIMEOUT);
      chassis.turnToHeading(0, TIMEOUT_SHORT);
      matchLoad->extend();
      conveyer->intake();
      chassis.moveToPoint(LOADER_TR_WAY2.x-5, LOADER_TR_WAY2.y + 20, TIMEOUT);
      pros::delay(2000);
      chassis.moveToPoint(GOAL_TR.x, GOAL_TR.y, TIMEOUT, {
        .forwards = false,
        .maxSpeed=100
      });
      pros::delay(2000);
      conveyer->scoreLong();
      pros::delay(1500);
      matchLoad->retract();

      // Finish second score
      conveyer->stop();
      chassis.moveToPoint(GOAL_LOADER_TR_MIDPOINT.x - 3, GOAL_LOADER_TR_MIDPOINT.y, TIMEOUT);
      chassis.turnToHeading(-90, TIMEOUT_SUPER_SHORT);
      chassis.moveToPoint(GOAL_LOADER_TL_MIDPOINT.x, GOAL_LOADER_TL_MIDPOINT.y, TIMEOUT);
      chassis.turnToHeading(0, TIMEOUT_SUPER_SHORT);
      matchLoad->extend();
      conveyer->intake();
      chassis.moveToPoint(LOADER_TL.x, LOADER_TL.y, TIMEOUT,{
        .maxSpeed=100
      });
      pros::delay(1500);
      chassis.moveToPoint(GOAL_TL.x, GOAL_TL.y, TIMEOUT, {
        .forwards=false
      });
      conveyer->scoreLong();
      matchLoad->retract();
      // FINISH GOAL TL

      conveyer->stop();
      chassis.swingToHeading(135, lemlib::DriveSide::RIGHT, TIMEOUT_SHORT);

      // MOVE TO SECOND LOADER
      chassis.moveToPoint(LOADER_BL_WAY.x, LOADER_BL_WAY.y, TIMEOUT_SHORT);
      chassis.turnToHeading(180,TIMEOUT_SUPER_SHORT);

      chassis.moveToPoint(
        LOADER_BL_WAY1.x, LOADER_BL_WAY1.y, TIMEOUT, {
          .maxSpeed=80
        });
      chassis.turnToHeading(-90,TIMEOUT_SHORT);
      chassis.moveToPoint( LOADER_TR_WAY2.x, LOADER_TR_WAY2.y, TIMEOUT);
      // chassis.turnToHeading(0, TIMEOUT_SHORT);
      // matchLoad->extend();
      // conveyer->intake();
      // chassis.moveToPoint(LOADER_TR_WAY2.x - 5, LOADER_TR_WAY2.y + 20, TIMEOUT);
      // pros::delay(2000);
      // chassis.moveToPoint(GOAL_TR.x - 3, GOAL_TR.y, TIMEOUT, {
      //   .forwards = false
      // });
      // pros::delay(2000);
      // conveyer->scoreLong();
      // pros::delay(1500);
      // matchLoad->retract();

      
      // chassis.moveToPoint(LOADER_BL_WAY.x, LOADER_BL_WAY.y, TIMEOUT);
      // chassis.turnToHeading(90, TIMEOUT_SUPER_SHORT);
      // chassis.moveToPoint(LOADER_BL_WAY.x, LOADER_BL_WAY.y, TIMEOUT_SHORT);
      // chassis.turnToHeading(180, TIMEOUT_SUPER_SHORT);
      // chassis.moveToPoint(LOADER_BL_WAY1.x, LOADER_BL_WAY1.y, TIMEOUT);
      // chassis.turnToHeading(-90, TIMEOUT_SUPER_SHORT);
      // chassis.moveToPoint(LOADER_BL_WAY2.x, LOADER_BL_WAY2.y, TIMEOUT);
      // chassis.turnToHeading(180, TIMEOUT_SUPER_SHORT);
      // matchLoad->extend();
      // conveyer->intake();
      // chassis.moveToPoint(LOADER_BL.x, LOADER_BL.y, TIMEOUT);
      // pros::delay(1500);
      // chassis.moveToPoint(GOAL_BL.x, GOAL_BL.y, TIMEOUT, {
      //   .forwards=false
      // });
      // pros::delay(1500);
      // conveyer->scoreLong();
      // pros::delay(1500);
      // matchLoad->retract();
      


    //   matchLoad->extend();
    //   //turning moving to loader//LOADER BL
    //   chassis.moveToPose(
    //     LOADER_BL.x, LOADER_BL.y, LOADER_BL.theta,
    //     TIMEOUT
    //   );
    //   conveyer->scoreLong();
    //   //scoring in Left goal from the other side
    //   chassis.moveToPoint(GOAL_L.x, GOAL_L.y,TIMEOUT,
    //     {
    //       .forwards = false
    //     }
    //   );
    //   chassis.turnToHeading(90, TIMEOUT_SHORT);
    //   chassis.moveToPoint(PARK.x, PARK.y, TIMEOUT);

    }

private: 
    lemlib::Pose STARTING_POS{15.25, 24, 90};

    lemlib::Pose LOADER_BR_WAY = lemlib::Pose(48, 24, 90);
    lemlib::Pose LOADER_BR = lemlib::Pose(48, 12, -180);

    lemlib::Pose GOAL_R = lemlib::Pose(47, 44, -180);

    lemlib::Pose LOADER_TR_WAY = lemlib::Pose(28,52,-135); //diagonal forward toward middle goal
    lemlib::Pose LOADER_TR_WAY1 = lemlib::Pose(28, 118, 0); //across the field
    lemlib::Pose LOADER_TR_WAY2 = lemlib::Pose(40, 118, 0);

    lemlib::Pose GOAL_TR = lemlib::Pose(40, 105, 0);

    lemlib::Pose GOAL_LOADER_TR_MIDPOINT = lemlib::Pose(42, 115, 0);
    lemlib::Pose GOAL_LOADER_TL_MIDPOINT = lemlib::Pose(-55, 115, -90);
    lemlib::Pose LOADER_TL = lemlib::Pose(-55, 128, -90);
    lemlib::Pose GOAL_TL = lemlib::Pose(-55, 96, -90);


    lemlib::Pose LOADER_BL_WAY = lemlib::Pose(-30, 92, 180);
    lemlib::Pose LOADER_BL_WAY1 = lemlib::Pose(-30, 24, -160);
    lemlib::Pose LOADER_BL_WAY2 = lemlib::Pose(-48, 24, -160);
    // lemlib::Pose LOADER_BL_WAY2 = lemlib::Pose(-48, 22, 139);
    // lemlib::Pose LOADER_BL = lemlib::Pose(-55, 24, 180);

    lemlib::Pose GOAL_BL = lemlib::Pose(-53, 40, 180);

    lemlib::Pose PARK = lemlib::Pose(0, 12, 90);

    int TIMEOUT_SUPER_SHORT = 500;
    int TIMEOUT_SHORT = 1000;
    int TIMEOUT = 2000;
    int TIMEOUT_LONG = 3000;
    float MIN_SPEED = 80;
    float LEAD = 0.8;
};
