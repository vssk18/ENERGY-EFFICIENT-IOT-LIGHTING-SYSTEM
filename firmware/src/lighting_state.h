#pragma once

#include <Arduino.h>
#include "config.h"

// Simple mode for the light
enum class Mode : uint8_t {
  OFF = 0,
  DIM = 1,
  ON_ACTIVE = 2
};

// For logging to Serial / CSV
struct Telemetry {
  unsigned long ms;
  int lux;
  bool motion;
  Mode mode;
  int pwm;
};

class Lighting {
public:
  void begin();
  Telemetry step();

private:
  Mode mode_ = Mode::OFF;
  unsigned long last_motion_ms_ = 0;
  int current_pwm_ = 0;

  int  readLux();
  bool readMotion();
  void setPWM(int target, int ramp_ms);
};
