#include "hardware.hpp"

std::shared_ptr<OdomChassisController> drivetrain = 
    ChassisControllerBuilder()
		.withMotors(
			{-1, -2}, // left motors -1, -2 (reversed)
			{3, 4} // right motors 3, 4
		)
		// green gearset, 4 inch wheel diameter, 11.5 inch wheel track
		.withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})
		.withOdometry()
		.buildOdometry();