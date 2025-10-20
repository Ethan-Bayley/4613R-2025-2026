#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "autoLongGoal.h"
#include "pros/adi.hpp"
#include "pros/motors.hpp"

extern pros::Controller controller;
extern lemlib::Chassis chassis;
extern pros::Motor bottomIntake;
extern pros::adi::Pneumatics matchLoader;