#include "vilot/vilot.hpp"
#include <cmath>

float joystick::s_curve(float input, float strength) {
  float weight = std::exp(-0.1 * strength);
  return (weight + exp(0.1 * (std::abs(input) - 127.0)) * (1.0 - weight)) *
         input;
}

float joystick::ramp_curve(float input, float strength) {
  return exp(0.001 * (strength * (std::abs(input) - 127.0))) * input;
}

float joystick::deadzone(float input, float min_abs_output,
                         float max_abs_output) {
  if (input < min_abs_output && input > -min_abs_output) {
    return 0.0;
  }

  return std::copysign((input - std::copysign(min_abs_output, input)) /
                           (1 - min_abs_output / max_abs_output),
                       input);
}
