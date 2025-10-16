#include "AutoSkills.h"

#include "lemlib/api.hpp"  // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"

//--------------------------------------
// MARK: - Constants
//--------------------------------------

	lemlib::Pose startingPos = lemlib::Pose(16, 24, 90);
	lemlib::Pose LOADER_BR = lemlib::Pose(48, 12, -180);
	lemlib::Pose GOAL_R = lemlib::Pose(45, 40, -180);
	//Heading to topright loader
	lemlib::Pose LOADER_TR_WAY = lemlib::Pose(45, 30, -180);
	lemlib::Pose LOADER_TR_WAY1 = lemlib::Pose(30, 55, 20);
	lemlib::Pose LOADER_TR_WAY2 = lemlib::Pose(33, 118, 41);
	lemlib::Pose LOADER_TR = lemlib::Pose(42, 138, 5);
	lemlib::Pose GOAL_R1 = lemlib::Pose(44, 105, 0);
	lemlib::Pose LOADER_TL_WAY = lemlib::Pose(44, 120, -90);
	lemlib::Pose LOADER_TL_WAY1 = lemlib::Pose(-48, 120, -90);
	int TIMEOUT_SHORT = 1000;
	int TIMEOUT = 2000;
	int TIMEOUT_LONG = 3000;
	float MIN_SPEED = 80;
	float LEAD = 0.8;

//--------------------------------------
// MARK: - Initialisers
//--------------------------------------

AutoSkills::AutoSkills(lemlib::Chassis &chassis)
    : chassis(chassis) {}

//--------------------------------------
// MARK: - Methods
//--------------------------------------
void AutoSkills::run() {
    chassis.setPose(startingPos);
	// FIRST LOADER
	chassis.moveToPose(
		LOADER_BR.x, LOADER_BR.y, LOADER_BR.theta,
		TIMEOUT, 
		{
			.minSpeed = MIN_SPEED, 
			.earlyExitRange = 8
		}
	);
	chassis.moveToPose(
		GOAL_R.x, GOAL_R.y, GOAL_R.theta, 
		TIMEOUT,
		{
			.forwards = false
		}
	);
	// MOVE TO SECOND LOADER
	chassis.moveToPose(
		LOADER_TR_WAY.x, LOADER_TR_WAY.y, LOADER_TR_WAY.theta,
		TIMEOUT_SHORT,
		{
			.earlyExitRange = 4
		}
	);
	chassis.swingToHeading(
    -20,
	lemlib::DriveSide::RIGHT,
    500,
    {
		.minSpeed = MIN_SPEED, 
		.earlyExitRange = 15, 
	}
	);
	chassis.moveToPoint(
		LOADER_TR_WAY1.x, LOADER_TR_WAY1.y,
		TIMEOUT_SHORT,
		{
			.earlyExitRange = 4
		}
	);
	// //turning and going to loader
	// chassis.turnToHeading(20, 500);
	chassis.moveToPoint(
		LOADER_TR_WAY2.x, LOADER_TR_WAY2.y,
		TIMEOUT,
		{
			.minSpeed = MIN_SPEED, 
			.earlyExitRange = 4
		}
	);
	//turning moving to loader
	chassis.moveToPose(
		LOADER_TR.x, LOADER_TR.y, LOADER_TR.theta,
		TIMEOUT
	);
	//scoring in Right goal from the other side
	chassis.moveToPose(
		GOAL_R1.x, GOAL_R1.y, GOAL_R1.theta,
		TIMEOUT,
		{
			.forwards = false
		}
	);
	chassis.moveToPoint(LOADER_TL_WAY.x, LOADER_TL_WAY.y, TIMEOUT_SHORT);
	chassis.turnToHeading(-90, TIMEOUT_SHORT);
	chassis.moveToPoint(LOADER_TL_WAY1.x, LOADER_TL_WAY1.y, TIMEOUT_LONG);
}
	

