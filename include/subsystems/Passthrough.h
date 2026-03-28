#pragma once

#include "commands/EventLoop.h"
#include "Subsystems/Hood.h"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/distance.hpp"
#include "pros/rtos.hpp"
#include <cstdint>

#include "Subsystems/Subsystems.h"

// Port configuration
#define PREROLLER_MOTOR_PORT 1
#define MIDDLE_MOTOR_PORT 2
#define TOP_MOTOR_PORT 3
#define HOOD_PORT 4
#define OPTICAL_SENSOR_PORT 5  // above the long/middle switchover point
#define DISTANCE_SENSOR_PORT 6 // between preroller and middle motor

// Stall / jam thresholds (mA)
constexpr int PREROLLER_STALL_MA = 2500; // 11 W blue motor
constexpr int MIDDLE_STALL_MA = 1200;    // 5.5 W blue motor
constexpr int TOP_STALL_MA = 1200;       // 5.5 W blue motor

// Timing constants (ms)
constexpr uint32_t SINGLE_REVERSE_MS = 300;   // single-motor reverse
constexpr uint32_t FULL_REVERSE_MS = 600;     // whole-intake reverse
constexpr uint32_t COLOUR_REVERSE_MS = 100;   // colour-sort top+mid reverse
constexpr uint32_t BLOCK_STOP_DELAY_MS = 500; // distance sensor block timeout
constexpr uint32_t COLOUR_RETURN_MS = 400;    // time in SCORE_MIDDLE before auto-return

// Colour sort
constexpr int HUE_RED_LOW = 0;
constexpr int HUE_RED_HIGH = 20;
constexpr int HUE_BLUE_LOW = 200;
constexpr int HUE_BLUE_HIGH = 240;
constexpr int OPTICAL_MIN_BRIGHTNESS = 100; // 0-255, ignore if too dark (no ball)

// Distance sensor
constexpr int BLOCK_PRESENT_MM = 50; // distance below which a block is detected

class Passthrough : public Subsystems
{
public:
    enum class State
    {
        IDLE,
        INTAKE,
        OUTTAKE,
        SCORE_LONG,
        SCORE_MIDDLE,
        SCORE_LOW,
        // Internal recovery states (not user-triggered)
        JAM_REVERSE_SINGLE,
        JAM_REVERSE_FULL,
        STALL_REVERSE_SINGLE,
        STALL_REVERSE_FULL,
        COLOUR_REVERSE,
    };

    enum class TeamColour
    {
        RED,
        BLUE
    };

    Passthrough();
    void setup();
    void registerHandlers(EventLoop &eventLoop);
    void update();

    void intake();
    void outtake();
    void stop();
    void scoreMiddle();
    void scoreLow();
    void scoreLong();
    void setTeamColour(TeamColour colour);
    TeamColour getTeamColour() const { return teamColour; }

private:
    pros::Motor prerollerMotor;
    pros::Motor middleMotor;
    pros::Motor topMotor;
    Hood hood;
    pros::Optical opticalSensor;
    pros::Distance distanceSensor;

    State currentState = State::IDLE;
    State returnState = State::IDLE; // state to return to after recovery
    TeamColour teamColour = TeamColour::RED;

    // Anti-jam / anti-stall tracking
    enum class MotorId
    {
        PREROLLER,
        MIDDLE,
        TOP
    };

    uint32_t jamStartTime[3] = {0, 0, 0};
    int jamRetryCount[3] = {0, 0, 0};
    bool inJamReverse = false;
    uint32_t jamReverseStart = 0;
    bool jamFullReverse = false;
    MotorId jamMotor = MotorId::PREROLLER;

    // Distance sensor block detection
    uint32_t blockDetectedSince = 0;
    bool blockWasPresent = false;
    bool middleStoppedByBlock = false;

    // Colour sort state
    bool colourReverseActive = false;
    uint32_t colourReverseStart = 0;
    bool colourScoreActive = false;
    uint32_t colourScoreStart = 0;

    // Helpers
    void setState(State newState);
    void applyMotorState();

    bool isWrongColour();
    void handleColourSort();
    void handleJamStall();
    void handleDistanceSensor();

    bool isMotorJammed(pros::Motor &motor, int threshold, MotorId id);
};