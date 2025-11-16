#include "lighting_state.h"

void Lighting::begin() {
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_PWM, OUTPUT);
  analogWrite(PIN_PWM, 0);
  current_pwm_ = 0;
  last_motion_ms_ = 0;
}

int Lighting::readLux() {
  int raw = analogRead(PIN_LDR);
  // Map 0–1023 ADC to a coarse 0–200 "lux-like" scale
  return map(raw, 0, 1023, 0, 200);
}

bool Lighting::readMotion() {
  return digitalRead(PIN_PIR) == HIGH;
}

void Lighting::setPWM(int target, int ramp_ms) {
  target = constrain(target, 0, 255);

  int start = current_pwm_;
  if (ramp_ms <= 0) {
    analogWrite(PIN_PWM, target);
    current_pwm_ = target;
    return;
  }

  int steps = ramp_ms / 20;
  if (steps < 1) steps = 1;

  for (int i = 1; i <= steps; ++i) {
    int v = start + (target - start) * i / steps;
    analogWrite(PIN_PWM, v);
    delay(20);  // small blocking delay, acceptable for this simple controller
  }

  current_pwm_ = target;
}

Telemetry Lighting::step() {
  static bool night = false;

  unsigned long now = millis();
  int lux = readLux();
  bool motion = readMotion();

  // Hysteresis on lux signal
  if (!night && lux < LUX_NIGHT_ON) {
    night = true;
  } else if (night && lux > LUX_DAY_ON) {
    night = false;
  }

  if (motion) {
    last_motion_ms_ = now;
  }

  if (!night) {
    // Daytime → force OFF
    if (mode_ != Mode::OFF) {
      setPWM(0, RAMP_MS);
      mode_ = Mode::OFF;
    }
  } else {
    // Night logic
    unsigned long since_motion = now - last_motion_ms_;

    if (motion || since_motion < HOLD_SEC * 1000UL) {
      // recent motion → ON_ACTIVE
      if (mode_ != Mode::ON_ACTIVE) {
        setPWM(PWM_ON, RAMP_MS);
        mode_ = Mode::ON_ACTIVE;
      }
    } else if (since_motion > IDLE_SEC * 1000UL) {
      // idle long enough → DIM
      if (mode_ != Mode::DIM) {
        setPWM(PWM_DIM, RAMP_MS);
        mode_ = Mode::DIM;
      }
    } else {
      // short idle → keep current mode
    }
  }

  Telemetry t;
  t.ms = now;
  t.lux = lux;
  t.motion = motion;
  t.mode = mode_;
  t.pwm = current_pwm_;
  return t;
}
