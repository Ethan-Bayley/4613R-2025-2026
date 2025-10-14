#include "autoLongGoal.h"

#include "lemlib/api.hpp"  // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"

//--------------------------------------
// MARK: - Constants
//--------------------------------------

const int TIMEOUT = 2000;
const int TIMEOUT_LONG = 3000;
const float MIN_SPEED = 80;

const lemlib::Pose STARTING_POS(-20.25, 16.25, 0);
const lemlib::Pose STACK1(-24, 48, -20);
const lemlib::Pose STACK2(-44, 60, -75);
const lemlib::Pose GOAL_WAYPOINTS(-32, 36, 0);
const lemlib::Pose GOAL(-58, 40, -180);
const lemlib::Pose MATCHLOAD(GOAL.x, 8, -180);

//--------------------------------------
// MARK: - Initialisers
//--------------------------------------

autoLongGoal::autoLongGoal(lemlib::Chassis &chassis) 
    : chassis(chassis) {}

//--------------------------------------
// MARK: - Methods
//--------------------------------------

void autoLongGoal::run() {
    chassis.setPose(STARTING_POS);
	chassis.moveToPoint(
		STACK1.x, STACK1.y,  
		TIMEOUT, 
		{
			.minSpeed = MIN_SPEED, 
			.earlyExitRange = 4
		}
	);
	chassis.moveToPose(
		STACK2.x, STACK2.y, STACK2.theta, 
		TIMEOUT
	);
	chassis.moveToPoint(
		GOAL_WAYPOINTS.x, GOAL_WAYPOINTS.y,
		TIMEOUT, 
		{
			.forwards = false,
			.minSpeed = MIN_SPEED, 
			.earlyExitRange = 4
		}
	);
	chassis.moveToPose(
		GOAL.x, GOAL.y, GOAL.theta, 
		TIMEOUT_LONG,
		{
			.forwards = false
		}
	);
	pros::c::delay(2000);
	chassis.moveToPoint(
		MATCHLOAD.x, MATCHLOAD.y,
		TIMEOUT
	);
	pros::c::delay(1000);
	chassis.moveToPose(
		GOAL.x, GOAL.y, GOAL.theta, 
		TIMEOUT_LONG,
		{
			.forwards = false
		}
	);
}