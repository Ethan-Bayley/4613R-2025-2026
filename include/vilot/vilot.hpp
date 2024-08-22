#pragma once

namespace joystick {

// https://www.desmos.com/calculator/rcfjjg83zx - blue
float s_curve(float input, float strength);

// https://www.desmos.com/calculator/rcfjjg83zx - red
float ramp_curve(float input, float strength);

float deadzone(float input, float min_abs_output, float max_abs_output);

}  // namespace joystick
