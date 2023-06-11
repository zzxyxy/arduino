#ifndef ZFASTLED
#define ZFASTLED

#include "FastLED.h"

class ZFastLed {
  CRGB *leds;
  int num_leds = 0;

public:
  template<template<uint8_t DATA_PIN, EOrder RGB_ORDER>class CHIPSET,  uint8_t DATA_PIN>
  void setup(const int numleds, const int datapin) {
    num_leds = numleds;
    leds = new CRGB[num_leds];
    FastLED.addLeds<CHIPSET, DATA_PIN>(leds, numleds);
  }
  void loop();
  void setAllColor(CRGB color);
  void setAllColor(uint8_t r, uint8_t g, uint8_t b);

};

#endif
