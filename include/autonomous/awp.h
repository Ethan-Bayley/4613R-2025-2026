#pragma once

#include "autonomous/Autonomous.h"
#include "subsystems/Passthrough.h"
#include "subsystems/Pneumatic.h"
#include "subsystems/Chassis.h"
#include <cmath>

class WorldsAWP : public Autonomous
{
	Passthrough *passthrough;
	SingleActingPneumatic *matchLoad;

public:
	WorldsAWP(Passthrough *passthrough, SingleActingPneumatic *ml)
		: passthrough(passthrough), matchLoad(ml) {}

	void run() override
	{
		chassis.setPose(ROBOT_START);

		// Drive to right loader zone and align
		chassis.moveToPoint(RIGHT_LOADER_APPROACH.x, RIGHT_LOADER_APPROACH.y, TIMEOUT_SHORT);
		chassis.turnToHeading(-180, TIMEOUT_SUPER_SHORT);

		matchLoad->extend();
		passthrough->intake();

		// Move into right loader and score
		chassis.moveToPoint(RIGHT_LOADER.x, RIGHT_LOADER.y, TIMEOUT_SHORT);
		pros::delay(1000);
		chassis.moveToPoint(RIGHT_GOAL.x, RIGHT_GOAL.y, TIMEOUT,
							{.forwards = false, .maxSpeed = 100});

		passthrough->scoreLong();
		matchLoad->retract();

		pros::delay(2000);
		// END OF RIGHT GOAL

		// Pull back slightly and intake
		chassis.moveToPoint(RIGHT_GOAL.x, RIGHT_GOAL.y - 13, TIMEOUT_SHORT,
							{.maxSpeed = 80});
		passthrough->intake();

		// Navigate to right side triball stack
		chassis.turnToHeading(-60, 900);
		chassis.turnToPoint(RIGHT_TRIBALL_STACK.x, RIGHT_TRIBALL_STACK.y, TIMEOUT_SHORT);
		chassis.moveToPose(RIGHT_TRIBALL_STACK.x, RIGHT_TRIBALL_STACK.y, 270, TIMEOUT_SHORT,
						   {.maxSpeed = 127, .minSpeed = 50, .earlyExitRange = 4});

		matchLoad->extend();
		pros::Task retractMatchloadTask([&]()
										{
			pros::delay(200);
			matchLoad->retract();
			pros::delay(500);
			matchLoad->extend(); });

		// Push centre triball stack into middle goal
		chassis.moveToPoint(CENTRE_TRIBALL_STACK.x, CENTRE_TRIBALL_STACK.y, 1500, {.maxSpeed = 127});
		matchLoad->extend();
		chassis.turnToHeading(-135, TIMEOUT_SUPER_SHORT);
		chassis.moveToPoint(UPPER_MIDDLE_GOAL.x, UPPER_MIDDLE_GOAL.y, 2000,
							{.forwards = false, .maxSpeed = 50});

		passthrough->scoreMiddle();

		// Navigate to left loader zone
		chassis.moveToPoint(LEFT_LOADER_APPROACH.x, LEFT_LOADER_APPROACH.y - 10, 3000);
		passthrough->intake();
		chassis.turnToHeading(180, 500);

		// Move into left loader and score left goal
		chassis.moveToPoint(LEFT_LOADER.x, LEFT_LOADER.y, TIMEOUT_SHORT);
		chassis.moveToPoint(LEFT_GOAL.x, LEFT_GOAL.y, TIMEOUT_SHORT, {.forwards = false});
		passthrough->scoreLong();
	}

private:
	const lemlib::Pose ROBOT_START = lemlib::Pose(15.25, 24, 90);

	// Right side loader zone
	const lemlib::Pose RIGHT_LOADER_APPROACH = lemlib::Pose(48, 24, 90);
	const lemlib::Pose RIGHT_LOADER = lemlib::Pose(48, 11, -180);

	// Goals
	const lemlib::Pose RIGHT_GOAL = lemlib::Pose(48, 48, -180);
	const lemlib::Pose LEFT_GOAL = lemlib::Pose(-47, 47, -180);
	const lemlib::Pose UPPER_MIDDLE_GOAL = lemlib::Pose(-8, 56, -135);
	const lemlib::Pose LOWER_MIDDLE_GOAL = lemlib::Pose(8, 56, 135);

	// Triball stacks
	const lemlib::Pose RIGHT_TRIBALL_STACK = lemlib::Pose(19.5, 48, -135);
	const lemlib::Pose CENTRE_TRIBALL_STACK = lemlib::Pose(-23.5, 48, -90);

	// Left side loader zone
	const lemlib::Pose LEFT_LOADER_APPROACH = lemlib::Pose(-41, 30, -160);
	const lemlib::Pose LEFT_LOADER = lemlib::Pose(-41, 6, 180);

	// Unused waypoints (kept for future use)
	const lemlib::Pose LEFT_LOADER_SIDE_APPROACH = lemlib::Pose(-33, 17, 90);
	const lemlib::Pose CENTRE_APPROACH = lemlib::Pose(0, 32, 90);
	const lemlib::Pose PARK_APPROACH_SIDE = lemlib::Pose(-51, 33, 180);
	const lemlib::Pose PARK_APPROACH_CENTRE = lemlib::Pose(-3, 33, 90);
	const lemlib::Pose PARK = lemlib::Pose(-3, 16, 90);
	const lemlib::Pose LEFT_BAR_HIT = lemlib::Pose(-52, 38, -180);
	const lemlib::Pose RIGHT_BAR_HIT = lemlib::Pose(48, 39, -180);

	// Timeouts (ms)
	int TIMEOUT_SUPER_SHORT = 500;
	int TIMEOUT_SHORT = 1000;
	int TIMEOUT = 2000;
	int TIMEOUT_LONG = 3000;

	// Motion constants
	float MIN_SPEED = 80;
	float LEAD = 0.8;
};