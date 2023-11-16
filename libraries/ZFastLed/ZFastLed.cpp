#include "ZFastLed.h"


void ZFastLed::loop() {
}


void ZFastLed::setAllColor(CRGB color) {
  for (unsigned i = 0; i < num_leds; ++i) {
    leds[i] = color;
  }

  FastLED.show();
}


void ZFastLed::setAllColor(uint8_t r, uint8_t g, uint8_t b) {
  CRGB color = 0;
  color.r = g;
  color.g = r;
  color.b = b;
  setAllColor(color);
}


CRGB ZFastLed::getLed(const int led) {
  return leds[led];
}
