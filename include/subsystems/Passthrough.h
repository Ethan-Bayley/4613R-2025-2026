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
        int HOOD_PORT);

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
//prototype ports
    // static constexpr int INTAKE_MOTOR_PORT = 19;
    // static constexpr int MIDGOAL_MOTOR_PORT = -18;
    // static constexpr int LONGGOAL_MOTOR_PORT = 11;

    //new robot ports
    static constexpr int INTAKE_MOTOR_PORT = -3;
    static constexpr int MIDGOAL_MOTOR_PORT = -9;
    static constexpr int LONGGOAL_MOTOR_PORT = 2;
    static constexpr int HOOD_PORT = 'H';

    pros::Motor intakeMotor;
    pros::Motor midGoalMotor;
    pros::Motor longGoalMotor;
    pros::adi::Pneumatics hood;

    State currentState = State::IDLE;
};