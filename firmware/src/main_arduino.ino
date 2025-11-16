#include "lighting_state.h"

Lighting sys;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for USB Serial on some boards
  }
  sys.begin();
  Serial.println("ms,lux,motion,mode,pwm");
}

void loop() {
  Telemetry t = sys.step();

  static unsigned long last_log = 0;
  unsigned long now = millis();

  // Log once per second
  if (now - last_log > 1000) {
    Serial.print(t.ms);
    Serial.print(',');
    Serial.print(t.lux);
    Serial.print(',');
    Serial.print(t.motion ? 1 : 0);
    Serial.print(',');
    Serial.print(static_cast<int>(t.mode));
    Serial.print(',');
    Serial.println(t.pwm);
    last_log = now;
  }
}
