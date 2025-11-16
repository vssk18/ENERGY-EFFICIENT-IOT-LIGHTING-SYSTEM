#pragma once

// -------- Tunable thresholds --------

// Lux thresholds with hysteresis
// If measured lux goes above LUX_DAY_ON, we treat it as "day"
// If lux drops below LUX_NIGHT_ON, we treat it as "night"
constexpr int LUX_DAY_ON   = 80;   // go OFF above this
constexpr int LUX_NIGHT_ON = 60;   // go ON below this

// PWM levels (0–255)
constexpr int PWM_DIM      = 60;   // dim level
constexpr int PWM_ON       = 220;  // full brightness

// Timing (seconds)
constexpr int HOLD_SEC     = 90;   // stay bright after motion
constexpr int IDLE_SEC     = 120;  // drop to DIM after idle

// Ramp (milliseconds)
constexpr int RAMP_MS      = 800;  // smooth ramp duration

// -------- Pin mapping (adjust per board) --------
constexpr int PIN_LDR      = A0;   // analog light sensor
constexpr int PIN_PIR      = 2;    // PIR motion sensor
constexpr int PIN_PWM      = 9;    // LED control (PWM)
