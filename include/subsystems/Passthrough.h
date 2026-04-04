#pragma once
#include "Subsystems.h"
#include "pros/adi.hpp"
#include "pros/distance.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"

class Passthrough : public Subsystems {
public:
  enum class State {
    IDLE,
    INTAKE,
    OUTTAKE,
    SCORE_MIDDLE,
    SCORE_LOW,
    SCORE_LONG
  };

  enum class Colour { RED, BLUE, NONE };

  Passthrough();
  Passthrough(int prerollerMotorPort, int middleMotorPort, int topMotorPort,
              int hoodPort, int distancePort, int opticalPort);

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

  void setTeam(Colour newTeam);

  int getColourSorting();

private:
  static constexpr int PREROLLER_MOTOR_PORT = -3;
  static constexpr int MIDDLE_MOTOR_PORT = -9;
  static constexpr int TOP_MOTOR_PORT = 2;
  static constexpr int HOOD_PORT = 'H';
  static constexpr int DISTANCE_PORT = 7;
  static constexpr int OPTICAL_PORT = 6;

  // Stall thresholds (mA)
  static constexpr int PREROLLER_STALL_MA = 2500; // 11W blue motor
  static constexpr int MIDDLE_STALL_MA = 1200;    // 5.5W 200rpm
  static constexpr int TOP_STALL_MA = 1200;       // 5.5W 200rpm
  static constexpr int STALL_REVERSE_MS = 100;

  int wrongBallsQueued = 0;
  bool colourDetectCooldown = false;
  uint32_t lastDetectTime = 0;

  pros::Motor prerollerMotor;
  pros::Motor middleMotor;
  pros::Motor topMotor;
  pros::adi::Pneumatics hood;
  pros::Distance distance;
  pros::Optical optical;

  bool distanceBlocked = false;
  uint32_t distanceBlockedTime = 0;

  int colourSortState = 0;
  uint32_t colourSortTime = 0;

  State currentState = State::IDLE;
  Colour currentTeam = Colour::RED;

  // Stall tracking (0 = not stalling)
  uint32_t prerollerStallStart = 0;
  uint32_t middleStallStart = 0;
  uint32_t topStallStart = 0;

  // Moves a motor at voltage with antistall -- reverses for STALL_REVERSE_MS if
  // jammed
  void moveMotor(pros::Motor &motor, int voltage, int stallMa,
                 uint32_t &stallStart);
  void lockMotor(pros::Motor &motor, int voltage, int stallMa,
                 uint32_t &stallStart);
  void pulseMotor(pros::Motor &motor, int voltage, int stallMa,
                  uint32_t &stallStart);
  bool isWrongColour();
};