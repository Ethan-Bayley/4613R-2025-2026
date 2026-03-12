#pragma once
#include "Subsystems.h"
#include "pros/motors.hpp"
#include "pros/adi.hpp"

class Passthrough : public Subsystems {
public:                          // <-- constructors were missing this
    enum class State {
        IDLE,
        INTAKE,
        OUTTAKE,
        SCORE_MIDDLE,
        SCORE_LONG
    };

    Passthrough();
    Passthrough(
        int intakeMotorPort, int midGoalMotorPort, int longGoalMotorPort,
        int matchLoaderPistonPort, int lWingPistonPort, int rWingPistonPort, int odomPistonPort);

    void setup() override;
    void registerHandlers(EventLoop &eventLoop) override;
    void update() override;

    void intake();
    void outtake();
    void scoreMiddle();
    void scoreLong();
    void stop();
    void setState(State newState);

private:
    static constexpr int INTAKE_MOTOR_PORT = 19;
    static constexpr int MIDGOAL_MOTOR_PORT = 18;
    static constexpr int LONGGOAL_MOTOR_PORT = 11;

    pros::Motor intakeMotor;
    pros::Motor midGoalMotor;
    pros::Motor longGoalMotor;

    State currentState = State::IDLE;
};