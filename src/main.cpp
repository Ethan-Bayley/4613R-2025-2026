#include "main.h"
#include "ARMS/config.h"
#include "hardware.hpp"

constexpr float FORWARD_T = 10.0;
constexpr bool FORWARD_RED = false;

constexpr float TURN_T = 16.0;
constexpr bool TURN_RED = true;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  arms::init();
  pros::lcd::initialize();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

// linear to steep
float red_curve(float input, float t) {
  float weight = exp(-0.1 * t);
  return (weight + exp(0.1 * (abs(input) - 127)) * (1 - weight)) * input;
}

// S-curve
float blue_curve(float input, float t) {
  return exp(0.001 * (t * (abs(input) - 127))) * input;
}

// Pilons input scaling:
// https://www.desmos.com/calculator/rcfjjg83zx
float joystick_curves(float input, bool is_red, float t) {
  return is_red ? red_curve(input, t) / 1.27 : blue_curve(input, t) / 1.27;
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

  while (true) {
    float forward = master.get_analog(ANALOG_LEFT_Y);
    float turn = master.get_analog(ANALOG_RIGHT_X);

    float forward_curve = joystick_curves(forward, FORWARD_RED, FORWARD_T);
    float turn_curve = joystick_curves(turn, TURN_RED, TURN_T);

    arms::chassis::arcade(forward_curve, turn_curve);

    pros::delay(ez::util::DELAY_TIME);
  }
}
