#include "ZFastLed.h"


void ZFastLed::loop() {
}

void ZFastLed::setAllColor(CRGB color) {
  for (unsigned i = 0; i < num_leds; ++i) {
    leds[i] = color;
  }

  FastLED.show();
}
