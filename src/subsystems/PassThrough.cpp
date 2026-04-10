#include "subsystems/Passthrough.h"
#include "commands/ButtonCommand.h"
#include "pros/distance.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include <memory>

// ── Constructors
// ──────────────────────────────────────────────────────────────

Passthrough::Passthrough()
    : prerollerMotor(PREROLLER_MOTOR_PORT, pros::MotorGearset::blue),
      middleMotor(MIDDLE_MOTOR_PORT, pros::MotorGearset::blue),
      topMotor(TOP_MOTOR_PORT, pros::MotorGearset::blue),
      hood(HOOD_PORT, false), distance(DISTANCE_PORT), optical(OPTICAL_PORT) {}

Passthrough::Passthrough(int prerollerPort, int middlePort, int topPort,
                         int hoodPort, int distancePort, int opticalPort)
    : prerollerMotor(prerollerPort, pros::MotorGearset::blue),
      middleMotor(middlePort, pros::MotorGearset::blue),
      topMotor(topPort, pros::MotorGearset::blue), hood(hoodPort, false),
      distance(DISTANCE_PORT), optical(OPTICAL_PORT) {}

// ── Setup
// ─────────────────────────────────────────────────────────────────────

void Passthrough::setup() { optical.set_led_pwm(100); }

// ── Handlers
// ──────────────────────────────────────────────────────────────────

void Passthrough::registerHandlers(EventLoop &eventLoop) {
  // Intake
  eventLoop.registerCommand(std::unique_ptr<Command>(new ButtonCommand(
      eventLoop.controller, pros::E_CONTROLLER_DIGITAL_R1,
      [this]() { setState(State::INTAKE); },
      [this]() { setState(State::IDLE); })));

  // Outtake
  eventLoop.registerCommand(std::unique_ptr<Command>(new ButtonCommand(
      eventLoop.controller, pros::E_CONTROLLER_DIGITAL_R2,
      [this]() { setState(State::OUTTAKE); },
      [this]() { setState(State::IDLE); })));

  // Score Long
  eventLoop.registerCommand(std::unique_ptr<Command>(new ButtonCommand(
      eventLoop.controller, pros::E_CONTROLLER_DIGITAL_L1,
      [this]() { setState(State::SCORE_LONG); },
      [this]() { setState(State::IDLE); })));

  // Score Middle
  eventLoop.registerCommand(std::unique_ptr<Command>(new ButtonCommand(
      eventLoop.controller, pros::E_CONTROLLER_DIGITAL_L2,
      [this]() { setState(State::SCORE_MIDDLE_FAST); },
      [this]() { setState(State::IDLE); })));

  eventLoop.registerCommand(std::unique_ptr<Command>(new ButtonCommand(
      eventLoop.controller, pros::E_CONTROLLER_DIGITAL_A,
      [this]() { setState(State::SCORE_MIDDLE_SLOW); },
      [this]() { setState(State::IDLE); })));

  // Score Low
  eventLoop.registerCommand(std::unique_ptr<Command>(new ButtonCommand(
      eventLoop.controller, pros::E_CONTROLLER_DIGITAL_LEFT,
      [this]() { setState(State::SCORE_LOW); },
      [this]() { setState(State::IDLE); })));
}

// ── State
// ─────────────────────────────────────────────────────────────────────

void Passthrough::setState(State newState) {
  currentState = newState;
  stateEntryTime = pros::millis(); // add this
  prerollerStallStart = 0;
  middleStallStart = 0;
  topStallStart = 0;
}

// ── Antistall
// ───────────────────────────────────────────────────────────────── If motor
// current exceeds threshold, reverse it for STALL_REVERSE_MS. Passing voltage=0
// always stops the motor with no stall check.

void Passthrough::moveMotor(pros::Motor &motor, int voltage, int stallMa,
                            uint32_t &stallStart) {
  uint32_t now = pros::millis();

  // If currently in a stall reverse, keep reversing until timer expires
  if (stallStart != 0) {
    if ((now - stallStart) < (uint32_t)STALL_REVERSE_MS) {
      motor.move_voltage(-12000);
      return;
    }
    stallStart = 0; // reverse done, resume normal
  }

  if (voltage == 0) {
    motor.move_voltage(0);
    return;
  }

  // Detect stall: high current AND low velocity
  bool highCurrent = motor.get_current_draw() > stallMa;
  bool lowVelocity = std::abs(motor.get_actual_velocity()) < 10;

  if (highCurrent && lowVelocity) {
    stallStart = now;
    motor.move_voltage(-12000);
    return;
  }

  motor.move_voltage(voltage);
}

void Passthrough::lockMotor(pros::Motor &motor, int voltage, int stallMa,
                            uint32_t &stallStart) {
  uint32_t now = pros::millis();

  // Locked: stay stopped until caller passes voltage=0
  if (stallStart != 0) {
    motor.move_voltage(3000);
    return;
  }

  if (voltage == 0) {
    stallStart = 0; // reset lock so motor can run again next time
    motor.move_voltage(3000);
    return;
  }

  bool highCurrent = motor.get_current_draw() > stallMa;
  bool lowVelocity = std::abs(motor.get_actual_velocity()) < 10;

  if (highCurrent && lowVelocity) {
    stallStart = now;
    motor.move_voltage(3000);
    return;
  }

  motor.move_voltage(voltage);
}

void Passthrough::pulseMotor(pros::Motor &motor, int voltage,
                             uint32_t &pulseStart) {
  if (pulseStart == 0) {
    pulseStart = pros::millis();
    if (pulseStart == 0)
      pulseStart = 1; // guard against millis() == 0
  }

  if (voltage == 0) {
    pulseStart = 0;
    motor.move_voltage(0);
    return;
  }

  uint32_t elapsed = (pros::millis() - pulseStart) % 750;
  if (elapsed < 250) {
    motor.move_voltage(voltage);
  } else {
    motor.move_voltage(0);
  }
}

bool Passthrough::isWrongColour() {
  double brightness = optical.get_brightness(); // 0.0-1.0 in PROS 3
  pros::c::optical_rgb_s rgb = optical.get_rgb();
  //   if (optical.get_proximity() < 100) {
  Colour ballColour = Colour::RED;
  if (rgb.red > rgb.blue && rgb.red > rgb.green) {
    ballColour = Colour::RED;
  }
  if (rgb.blue > rgb.red && rgb.blue > rgb.green) {
    ballColour = Colour::BLUE;
  }
  if (currentTeam == ballColour) {
    return false;
  } else {
    return true;
  }
  //   } else {
  return false;
  //   }
}

// ── Update
// ────────────────────────────────────────────────────────────────────

void Passthrough::update() {
  // if (distance.get_distance() < 50 && !distanceBlocked) {
  //   distanceBlocked = true;
  //   distanceBlockedTime = pros::millis();
  // } else if (distance.get_distance() > 50 && distanceBlocked) {
  //   distanceBlocked = false;
  //   distanceBlockedTime = 0;
  // }

  bool wrongColour = isWrongColour();
  // bool wrongColour = false;
  if (wrongColour) {
    colourSortState = 1;
    colourSortTime = pros::millis();
  }
  if (!wrongColour && pros::millis() - colourSortTime > 250 &&
      colourSortState != 0) {
    colourSortState = 0;
    colourSortTime = 0;
  }

  switch (currentState) {
  case State::IDLE:
    moveMotor(prerollerMotor, 0, PREROLLER_STALL_MA, prerollerStallStart);
    moveMotor(middleMotor, 0, MIDDLE_STALL_MA, middleStallStart);
    moveMotor(topMotor, 0, TOP_STALL_MA, topStallStart);
    hood.retract();
    break;

  case State::INTAKE:
    moveMotor(prerollerMotor, 12000, PREROLLER_STALL_MA, prerollerStallStart);

    // Middle: run until stall, then stay stopped
    if (middleStallStart != 0) {
      middleMotor.move_voltage(0);
    } else {
      bool middleHighCurrent = middleMotor.get_current_draw() > MIDDLE_STALL_MA;
      bool middleLowVelocity = std::abs(middleMotor.get_actual_velocity()) < 10;
      if (middleHighCurrent && middleLowVelocity) {
        middleStallStart = pros::millis();
        middleMotor.move_voltage(0);
      } else {
        middleMotor.move_voltage(12000);
      }
    }

    // Top: run until stall, then stay stopped
    if (topStallStart != 0) {
      topMotor.move_voltage(6000);
    } else {
      bool topHighCurrent = topMotor.get_current_draw() > TOP_STALL_MA;
      bool topLowVelocity = std::abs(topMotor.get_actual_velocity()) < 10;
      if (topHighCurrent && topLowVelocity) {
        topStallStart = pros::millis();
        topMotor.move_voltage(0);
      } else {
        topMotor.move_voltage(12000);
      }
    }

    hood.retract();
    break;

  case State::INTAKE_PREROLLER:

    moveMotor(prerollerMotor, 12000, PREROLLER_STALL_MA, prerollerStallStart);
    moveMotor(middleMotor, 2000, MIDDLE_STALL_MA, middleStallStart);
    topMotor.move_voltage(0);
    hood.retract();
    break;

  case State::OUTTAKE:
    // No antistall on outtake -- high current is expected
    prerollerMotor.move_voltage(-12000);
    middleMotor.move_voltage(-12000);
    topMotor.move_voltage(-12000);
    break;

  case State::SCORE_MIDDLE_FAST: {
    bool reversing = (pros::millis() - stateEntryTime) < 250;
    if (reversing) {
      prerollerMotor.move_voltage(-12000);
      middleMotor.move_voltage(-12000);
      topMotor.move_voltage(-12000);
    } else if (colourSortState == 1) {
      moveMotor(prerollerMotor, 6000, PREROLLER_STALL_MA, prerollerStallStart);
      moveMotor(middleMotor, 6000, MIDDLE_STALL_MA, middleStallStart);
      moveMotor(topMotor, 12000, TOP_STALL_MA, topStallStart);
    } else {
      moveMotor(prerollerMotor, 12000, PREROLLER_STALL_MA, prerollerStallStart);
      moveMotor(middleMotor, 12000, MIDDLE_STALL_MA, middleStallStart);
      moveMotor(topMotor, -12000, TOP_STALL_MA, topStallStart);
    }
    hood.retract();
    break;
  }

  case State::SCORE_MIDDLE_SLOW: {
    bool reversing = (pros::millis() - stateEntryTime) < 250;
    if (reversing) {
      moveMotor(prerollerMotor, -12000, PREROLLER_STALL_MA,
                prerollerStallStart);
      moveMotor(middleMotor, -12000, MIDDLE_STALL_MA, middleStallStart);
      moveMotor(topMotor, -12000, TOP_STALL_MA, topStallStart);
    } else if (colourSortState == 1) {
      moveMotor(prerollerMotor, 6000, PREROLLER_STALL_MA, prerollerStallStart);
      moveMotor(middleMotor, 6000, MIDDLE_STALL_MA, middleStallStart);
      moveMotor(topMotor, 12000, TOP_STALL_MA, topStallStart);
    } else {
      moveMotor(prerollerMotor, 12000, PREROLLER_STALL_MA, prerollerStallStart);
      moveMotor(middleMotor, 9000, MIDDLE_STALL_MA, middleStallStart);
      moveMotor(topMotor, -6000, TOP_STALL_MA, topStallStart);
    }
    hood.retract();
    break;
  }

  case State::HOOD_RAISED: {
    moveMotor(prerollerMotor, 0, PREROLLER_STALL_MA, prerollerStallStart);
    moveMotor(middleMotor, 0, MIDDLE_STALL_MA, middleStallStart);
    moveMotor(topMotor, 0, TOP_STALL_MA, topStallStart);
    hood.extend();
    break;
  }

  case State::SCORE_LOW:
    moveMotor(prerollerMotor, 3000, PREROLLER_STALL_MA, prerollerStallStart);
    moveMotor(middleMotor, 12000, MIDDLE_STALL_MA, middleStallStart);
    moveMotor(topMotor, -12000, TOP_STALL_MA, topStallStart);
    break;

  case State::SCORE_LONG:
    if (colourSortState == 1) {
      moveMotor(prerollerMotor, 6000, PREROLLER_STALL_MA, prerollerStallStart);
      moveMotor(middleMotor, 6000, MIDDLE_STALL_MA, middleStallStart);
      moveMotor(topMotor, -12000, TOP_STALL_MA, topStallStart);
    } else {

      moveMotor(prerollerMotor, 12000, PREROLLER_STALL_MA, prerollerStallStart);
      moveMotor(middleMotor, 12000, MIDDLE_STALL_MA, middleStallStart);
      moveMotor(topMotor, 12000, TOP_STALL_MA, topStallStart);
    }

    hood.extend();
    break;

  case State::SCORE_LONG_NO_COLOUR: {
    hood.extend();

    // Run for 250ms regardless
    uint32_t startTime = pros::millis();
    while (pros::millis() - startTime < 250) {
      moveMotor(prerollerMotor, 12000, PREROLLER_STALL_MA, prerollerStallStart);
      moveMotor(middleMotor, 12000, MIDDLE_STALL_MA, middleStallStart);
      moveMotor(topMotor, 12000, TOP_STALL_MA, topStallStart);
      pros::delay(20);
    }

    // If stalled after 250ms, block until unstalled
    while (middleStallStart != 0) {
      moveMotor(prerollerMotor, 12000, PREROLLER_STALL_MA, prerollerStallStart);
      moveMotor(middleMotor, 12000, MIDDLE_STALL_MA, middleStallStart);
      moveMotor(topMotor, 12000, TOP_STALL_MA, topStallStart);
      pros::delay(20);
    }

    break;
  }
  case State::MIDDLE_GOAL_RECOVER:
    moveMotor(prerollerMotor, 0, PREROLLER_STALL_MA, prerollerStallStart);
    moveMotor(middleMotor, -12000, MIDDLE_STALL_MA, middleStallStart);
    moveMotor(topMotor, 12000, TOP_STALL_MA, topStallStart);
    hood.retract();
    break;
  }
}

int Passthrough::getColourSorting() { return colourSortState; }

// ── Public helpers
// ────────────────────────────────────────────────────────────

void Passthrough::intake() { setState(State::INTAKE); }
void Passthrough::intakePreroller() { setState(State::INTAKE_PREROLLER); }
void Passthrough::outtake() { setState(State::OUTTAKE); }
void Passthrough::stop() { setState(State::IDLE); }
void Passthrough::scoreMiddle() { setState(State::SCORE_MIDDLE_FAST); }
void Passthrough::scoreMiddleSlow() { setState(State::SCORE_MIDDLE_SLOW); }
void Passthrough::scoreLow() { setState(State::SCORE_LOW); }
void Passthrough::scoreLong() { setState(State::SCORE_LONG); }
void Passthrough::hoodRaised() { setState(State::HOOD_RAISED); }
void Passthrough::scoreLongNoColour() { setState(State::SCORE_LONG_NO_COLOUR); }
void Passthrough::middleGoalRecover() { setState(State::MIDDLE_GOAL_RECOVER); }