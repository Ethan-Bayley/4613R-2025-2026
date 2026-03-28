#pragma once
#include "Subsystems.h"
#include "subsystems/Pneumatic.h"
#include "pros/motors.hpp"

class Passthrough : public Subsystems
{
public:
    enum class State
    {
        IDLE,
        INTAKE,
        OUTTAKE,
        SCORE_MIDDLE,
        SCORE_LOW,
        SCORE_LONG
    };

    Passthrough();
    Passthrough(int prerollerMotorPort, int middleMotorPort, int topMotorPort, int hoodPort);

    void setup() override;
    void registerHandlers(EventLoop &eventLoop) override;
    void update() override;

    void intake();
    void outtake();
    void scoreMiddle();
    void scoreLow();
    void scoreLong();
    void stop();
    void setState(State newState);

private:
    //Prototype ports
    //  static constexpr int PREROLLER_MOTOR_PORT = 19;
    //  static constexpr int MIDDLE_MOTOR_PORT = -18;
    //  static constexpr int TOP_MOTOR_PORT = 11;

    //New ports for comp bot
    static constexpr int PREROLLER_MOTOR_PORT = -3;
    static constexpr int MIDDLE_MOTOR_PORT = -9;
    static constexpr int TOP_MOTOR_PORT = 2;
    static constexpr int HOOD_PORT = 'H';

    pros::Motor prerollerMotor;
    pros::Motor middleMotor;
    pros::Motor topMotor;
    SingleActingPneumatic hood; // swapped from pros::adi::Pneumatics

    State currentState = State::IDLE;
};