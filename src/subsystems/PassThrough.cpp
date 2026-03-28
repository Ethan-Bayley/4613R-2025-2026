#include "subsystems/Passthrough.h"
#include "Subsystems/Hood.h"
#include "pros/motors.h"

// Constructor

Passthrough::Passthrough()
	: prerollerMotor(PREROLLER_MOTOR_PORT, pros::MotorGearset::blue),
	  middleMotor(MIDDLE_MOTOR_PORT, pros::MotorGearset::blue),
	  topMotor(TOP_MOTOR_PORT, pros::MotorGearset::blue),
	  hood(HOOD_PORT),
	  opticalSensor(OPTICAL_SENSOR_PORT),
	  distanceSensor(DISTANCE_SENSOR_PORT) {}

// Setup

void Passthrough::setup()
{
	opticalSensor.set_led_pwm(100); // ensure LED is on for readings
}

// Button bindings

void Passthrough::registerHandlers(EventLoop &eventLoop)
{
	eventLoop.registerCommand(std::unique_ptr<Command>(new ButtonCommand(
		eventLoop.controller, pros::E_CONTROLLER_DIGITAL_R1,
		[this]()
		{ setState(State::INTAKE); },
		[this]()
		{ setState(State::IDLE); })));

	eventLoop.registerCommand(std::unique_ptr<Command>(new ButtonCommand(
		eventLoop.controller, pros::E_CONTROLLER_DIGITAL_R2,
		[this]()
		{ setState(State::OUTTAKE); },
		[this]()
		{ setState(State::IDLE); })));

	eventLoop.registerCommand(std::unique_ptr<Command>(new ButtonCommand(
		eventLoop.controller, pros::E_CONTROLLER_DIGITAL_L1,
		[this]()
		{ setState(State::SCORE_LONG); },
		[this]()
		{ setState(State::IDLE); })));

	eventLoop.registerCommand(std::unique_ptr<Command>(new ButtonCommand(
		eventLoop.controller, pros::E_CONTROLLER_DIGITAL_L2,
		[this]()
		{ setState(State::SCORE_MIDDLE); },
		[this]()
		{ setState(State::IDLE); })));

	eventLoop.registerCommand(std::unique_ptr<Command>(new ButtonCommand(
		eventLoop.controller, pros::E_CONTROLLER_DIGITAL_LEFT,
		[this]()
		{ setState(State::SCORE_LOW); },
		[this]()
		{ setState(State::IDLE); })));
}

// setState -- only honour user states; internal states set directly

void Passthrough::setState(State newState)
{
	currentState = newState;
	returnState = newState; // remember for post-recovery
	// Reset colour-sort tracking when the driver changes mode
	colourReverseActive = false;
	colourScoreActive = false;
}

// Main update -- called every loop tick

void Passthrough::update()
{
	// Order matters: distance first (may stop middle), then jam/stall
	// (may override voltages), then colour sort (may override state),
	// then apply final voltages.
	handleDistanceSensor();
	handleJamStall();
	handleColourSort();
	applyMotorState();
}

// applyMotorState -- translates currentState into voltages

void Passthrough::applyMotorState()
{
	switch (currentState)
	{

	case State::IDLE:
		prerollerMotor.move_voltage(0);
		middleStoppedByBlock ? middleMotor.move_voltage(0)
							 : middleMotor.move_voltage(0);
		topMotor.move_voltage(0);
		hood.retract();
		break;

	case State::INTAKE:
		prerollerMotor.move_voltage(12000);
		middleMotor.move_voltage(middleStoppedByBlock ? 0 : 12000);
		topMotor.move_voltage(3000);
		hood.retract();
		break;

	case State::OUTTAKE:
		prerollerMotor.move_voltage(-12000);
		middleMotor.move_voltage(-12000);
		topMotor.move_voltage(-12000);
		break;

	case State::SCORE_MIDDLE:
		prerollerMotor.move_voltage(12000);
		middleMotor.move_voltage(middleStoppedByBlock ? 0 : 12000);
		topMotor.move_voltage(-12000);
		hood.retract();
		break;

	case State::SCORE_LOW:
		prerollerMotor.move_voltage(3000);
		middleMotor.move_voltage(12000);
		topMotor.move_voltage(-12000);
		break;

	case State::SCORE_LONG:
		prerollerMotor.move_voltage(12000);
		middleMotor.move_voltage(middleStoppedByBlock ? 0 : 12000);
		topMotor.move_voltage(12000);
		hood.extend();
		break;

	case State::JAM_REVERSE_SINGLE:
	case State::STALL_REVERSE_SINGLE:
		// Handled inside handleJamStall; voltages already set there
		break;

	case State::JAM_REVERSE_FULL:
	case State::STALL_REVERSE_FULL:
		// Handled inside handleJamStall
		break;

	case State::COLOUR_REVERSE:
		// Handled inside handleColourSort
		break;
	}
}

// Distance sensor -- stops middleMotor if a block has been sitting at the
// preroller/middle junction for BLOCK_STOP_DELAY_MS

void Passthrough::handleDistanceSensor()
{
	// Only active when we are intaking or scoring (not reversing/idle)
	bool active = (currentState == State::INTAKE);

	if (!active)
	{
		blockDetectedSince = 0;
		blockWasPresent = false;
		middleStoppedByBlock = false;
		return;
	}

	int dist = distanceSensor.get(); // mm
	bool blockPresent = (dist > 0 && dist < BLOCK_PRESENT_MM);

	if (blockPresent)
	{
		if (!blockWasPresent)
		{
			blockDetectedSince = pros::millis();
			blockWasPresent = true;
		}
		if ((pros::millis() - blockDetectedSince) >= BLOCK_STOP_DELAY_MS)
		{
			middleStoppedByBlock = true;
		}
	}
	else
	{
		blockWasPresent = false;
		blockDetectedSince = 0;
		middleStoppedByBlock = false;
	}
}

// Colour sort

bool Passthrough::isWrongColour()
{
	double brightness = opticalSensor.get_brightness(); // 0.0-1.0 in PROS 3
	// Convert to 0-255 range for comparison with OPTICAL_MIN_BRIGHTNESS
	if ((brightness * 255.0) < OPTICAL_MIN_BRIGHTNESS)
		return false; // no ball

	double hue = opticalSensor.get_hue(); // 0-360

	if (teamColour == TeamColour::RED)
	{
		// We want red, reject blue
		return (hue >= HUE_BLUE_LOW && hue <= HUE_BLUE_HIGH);
	}
	else
	{
		// We want blue, reject red
		return (hue >= HUE_RED_LOW && hue <= HUE_RED_HIGH);
	}
}

void Passthrough::handleColourSort()
{
	uint32_t now = pros::millis();

	if (colourReverseActive)
	{
		if ((now - colourReverseStart) < COLOUR_REVERSE_MS)
		{
			// Reverse middle + top, hold preroller forward to keep feeding
			middleMotor.move_voltage(-12000);
			topMotor.move_voltage(-12000);
			return; // don't apply normal state voltages this tick
		}
		// Reverse done -- transition to SCORE_MIDDLE
		colourReverseActive = false;
		colourScoreActive = true;
		colourScoreStart = now;
		currentState = State::SCORE_MIDDLE;
		return;
	}

	if (colourScoreActive)
	{
		if ((now - colourScoreStart) < COLOUR_RETURN_MS)
		{
			return; // stay in SCORE_MIDDLE (applyMotorState will handle it)
		}
		colourScoreActive = false;
		currentState = State::INTAKE;
		returnState = State::INTAKE;
		return;
	}

	bool canSort = (currentState == State::INTAKE || currentState == State::SCORE_LONG);
	if (!canSort)
		return;

	if (isWrongColour())
	{
		colourReverseActive = true;
		colourReverseStart = now;
		currentState = State::COLOUR_REVERSE;
		// Voltages handled in the Phase 1 block above on next tick
	}
}

// Anti-jam / anti-stall

bool Passthrough::isMotorJammed(pros::Motor &motor, int threshold, MotorId id)
{
	// Only flag if the motor is commanded to move
	if (motor.get_voltage() == 0)
		return false;

	int current = motor.get_current_draw(); // mA
	if (current < threshold)
		return false;

	// Motor is over threshold -- has it been for a meaningful period?
	uint32_t now = pros::millis();
	if (jamStartTime[static_cast<int>(id)] == 0)
	{
		jamStartTime[static_cast<int>(id)] = now;
		return false;
	}
	// Require sustained over-current for at least 100 ms to avoid transients
	return (now - jamStartTime[static_cast<int>(id)]) >= 100;
}

void Passthrough::handleJamStall()
{
	uint32_t now = pros::millis();

	if (currentState == State::JAM_REVERSE_SINGLE ||
		currentState == State::STALL_REVERSE_SINGLE)
	{
		uint32_t elapsed = now - jamReverseStart;
		if (elapsed < SINGLE_REVERSE_MS)
		{
			// Continue reversing the specific motor
			switch (jamMotor)
			{
			case MotorId::PREROLLER:
				prerollerMotor.move_voltage(-12000);
				break;
			case MotorId::MIDDLE:
				middleMotor.move_voltage(-12000);
				break;
			case MotorId::TOP:
				topMotor.move_voltage(-12000);
				break;
			}
			return;
		}
		// Reverse done -- if retry limit hit, escalate to full reverse
		if (jamRetryCount[static_cast<int>(jamMotor)] >= 2)
		{
			currentState = (currentState == State::JAM_REVERSE_SINGLE)
							   ? State::JAM_REVERSE_FULL
							   : State::STALL_REVERSE_FULL;
			jamReverseStart = now;
			jamFullReverse = true;
		}
		else
		{
			// Resume normal state
			jamStartTime[static_cast<int>(jamMotor)] = 0;
			currentState = returnState;
		}
		return;
	}

	if (currentState == State::JAM_REVERSE_FULL ||
		currentState == State::STALL_REVERSE_FULL)
	{
		uint32_t elapsed = now - jamReverseStart;
		if (elapsed < FULL_REVERSE_MS)
		{
			prerollerMotor.move_voltage(-12000);
			middleMotor.move_voltage(-12000);
			topMotor.move_voltage(-12000);
			return;
		}
		// Full reverse done -- reset and resume
		for (int i = 0; i < 3; i++)
		{
			jamStartTime[i] = 0;
			jamRetryCount[i] = 0;
		}
		jamFullReverse = false;
		currentState = returnState;
		return;
	}

	if (currentState == State::COLOUR_REVERSE ||
		currentState == State::IDLE ||
		currentState == State::OUTTAKE)
	{
		for (int i = 0; i < 3; i++)
			jamStartTime[i] = 0;
		return;
	}

	// Check each motor
	struct
	{
		pros::Motor *motor;
		int threshold;
		MotorId id;
	} motors[] = {
		{&prerollerMotor, PREROLLER_STALL_MA, MotorId::PREROLLER},
		{&middleMotor, MIDDLE_STALL_MA, MotorId::MIDDLE},
		{&topMotor, TOP_STALL_MA, MotorId::TOP},
	};

	for (auto &m : motors)
	{
		// Skip middle motor if it's intentionally stopped by distance sensor
		if (m.id == MotorId::MIDDLE && middleStoppedByBlock)
			continue;

		if (isMotorJammed(*m.motor, m.threshold, m.id))
		{
			jamRetryCount[static_cast<int>(m.id)]++;
			jamMotor = m.id;
			jamReverseStart = now;
			currentState = State::JAM_REVERSE_SINGLE;
			jamStartTime[static_cast<int>(m.id)] = 0; // reset for next detection
			return;									  // handle one jam at a time
		}
	}

	// Clear timers for motors that are no longer over threshold
	for (auto &m : motors)
	{
		if (!isMotorJammed(*m.motor, m.threshold, m.id))
		{
			jamStartTime[static_cast<int>(m.id)] = 0;
		}
	}
}

void Passthrough::setTeamColour(TeamColour colour) { teamColour = colour; }
void Passthrough::intake() { setState(State::INTAKE); }
void Passthrough::outtake() { setState(State::OUTTAKE); }
void Passthrough::stop() { setState(State::IDLE); }
void Passthrough::scoreMiddle() { setState(State::SCORE_MIDDLE); }
void Passthrough::scoreLow() { setState(State::SCORE_LOW); }
void Passthrough::scoreLong() { setState(State::SCORE_LONG); }