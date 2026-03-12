#include "pros/motors.h"
#include "subsystems/Passthrough.h"

Passthrough::Passthrough() :
    intakeMotor(INTAKE_MOTOR_PORT, pros::MotorGearset::blue),
    midGoalMotor(MIDGOAL_MOTOR_PORT, pros::MotorGearset::blue),
    longGoalMotor(LONGGOAL_MOTOR_PORT, pros::MotorGearset::blue)
{
}

void Passthrough::setup() {
}

void Passthrough::registerHandlers(EventLoop &eventLoop) {
    // Intake
    eventLoop.registerCommand(
        std::unique_ptr<Command>(new ButtonCommand(
            eventLoop.controller,
            pros::E_CONTROLLER_DIGITAL_R1,
            [this]() { setState(State::INTAKE); },
            [this]() { setState(State::IDLE); }
        ))
    );

    // Outtake
    eventLoop.registerCommand(
        std::unique_ptr<Command>(new ButtonCommand(
            eventLoop.controller,
            pros::E_CONTROLLER_DIGITAL_R2,
            [this]() { setState(State::OUTTAKE); },
            [this]() { setState(State::IDLE); }
        ))
    );

    // Score Long
    eventLoop.registerCommand(
        std::unique_ptr<Command>(new ButtonCommand(
            eventLoop.controller,
            pros::E_CONTROLLER_DIGITAL_L1,
            [this]() { setState(State::SCORE_LONG); },
            [this]() { setState(State::IDLE); }
        ))
    );

    // Score Middle
    eventLoop.registerCommand(
        std::unique_ptr<Command>(new ButtonCommand(
            eventLoop.controller,
            pros::E_CONTROLLER_DIGITAL_L2,
            [this]() { setState(State::SCORE_MIDDLE); },
            [this]() { setState(State::IDLE); }
        ))
    );
}

void Passthrough::setState(State newState) {
    currentState = newState;
}

void Passthrough::update() {
    switch (currentState) {
        case State::IDLE:
            intakeMotor.move_voltage(0);
            midGoalMotor.move_voltage(0);
            longGoalMotor.move_voltage(0);
            break;

        case State::INTAKE:
            intakeMotor.move_voltage(12000);
            midGoalMotor.move_voltage(0);
            longGoalMotor.move_voltage(0);
            break;

        case State::OUTTAKE:
            intakeMotor.move_voltage(-12000);
            midGoalMotor.move_voltage(0);
            longGoalMotor.move_voltage(0);
            break;

        case State::SCORE_MIDDLE:
            intakeMotor.move_voltage(12000);
            midGoalMotor.move_voltage(12000);
            longGoalMotor.move_voltage(0);
            break;

        case State::SCORE_LONG:
            intakeMotor.move_voltage(12000);
            midGoalMotor.move_voltage(0);
            longGoalMotor.move_voltage(12000);
            break;
    }
}

void Passthrough::intake() {
    setState(State::INTAKE);
}

void Passthrough::outtake() {
    setState(State::OUTTAKE);
}

void Passthrough::stop() {
    setState(State::IDLE);
}

void Passthrough::scoreMiddle() {
    setState(State::SCORE_MIDDLE);
}

void Passthrough::scoreLong() {
    setState(State::SCORE_LONG);
}