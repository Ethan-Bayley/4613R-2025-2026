#include "subsystems/Passthrough.h"

#include "Subsystems/Hood.h"
#include "pros/motors.h"


Passthrough::Passthrough()
    : prerollerMotor(PREROLLER_MOTOR_PORT, pros::MotorGearset::blue),
      middleMotor(MIDDLE_MOTOR_PORT, pros::MotorGearset::blue),
      topMotor(TOP_MOTOR_PORT, pros::MotorGearset::blue),
      hood(HOOD_PORT, false) {}

void Passthrough::setup() {}

void Passthrough::registerHandlers(EventLoop& eventLoop) {
	// Intake
	eventLoop.registerCommand(std::unique_ptr<Command>(new ButtonCommand(
	    eventLoop.controller, pros::E_CONTROLLER_DIGITAL_R1, [this]() { setState(State::INTAKE); },
	    [this]() { setState(State::IDLE); })));

	// Outtake
	eventLoop.registerCommand(std::unique_ptr<Command>(new ButtonCommand(
	    eventLoop.controller, pros::E_CONTROLLER_DIGITAL_R2, [this]() { setState(State::OUTTAKE); },
	    [this]() { setState(State::IDLE); })));

	// Score Long
	eventLoop.registerCommand(std::unique_ptr<Command>(new ButtonCommand(
	    eventLoop.controller, pros::E_CONTROLLER_DIGITAL_L1,
	    [this]() { setState(State::SCORE_LONG); }, [this]() { setState(State::IDLE); })));

	// Score Middle
	eventLoop.registerCommand(std::unique_ptr<Command>(new ButtonCommand(
	    eventLoop.controller, pros::E_CONTROLLER_DIGITAL_L2,
	    [this]() { setState(State::SCORE_MIDDLE); }, [this]() { setState(State::IDLE); })));

	// Score Low
	eventLoop.registerCommand(std::unique_ptr<Command>(new ButtonCommand(
	    eventLoop.controller, pros::E_CONTROLLER_DIGITAL_LEFT,
	    [this]() { setState(State::SCORE_LOW); }, [this]() { setState(State::IDLE); })));
}

void Passthrough::setState(State newState) { currentState = newState; }

void Passthrough::update() {
	switch (currentState) {
		case State::IDLE:
			prerollerMotor.move_voltage(0);
			middleMotor.move_voltage(0);
			topMotor.move_voltage(0);
			hood.retract();
			break;

		case State::INTAKE:
			prerollerMotor.move_voltage(12000);
			middleMotor.move_voltage(12000);
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
			middleMotor.move_voltage(12000);
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
			middleMotor.move_voltage(12000);
			topMotor.move_voltage(12000);
			hood.extend();
			break;
	}
}

void Passthrough::intake() { setState(State::INTAKE); }

void Passthrough::outtake() { setState(State::OUTTAKE); }

void Passthrough::stop() { setState(State::IDLE); }

void Passthrough::scoreMiddle() { setState(State::SCORE_MIDDLE); }

void Passthrough::scoreLow() { setState(State::SCORE_LOW); }

void Passthrough::scoreLong() { setState(State::SCORE_LONG); }