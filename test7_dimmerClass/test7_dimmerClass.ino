
#include "Zchaos.h"
#include "Zledcontroller.h"

const byte pin = 11;

Zledcontroller led(pin);

void setup() {
  led.setup();
}

void loop() {
  led.loop();
  led.set(0);
}
