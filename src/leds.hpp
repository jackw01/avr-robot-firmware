// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include <FastLED.h>
#include "constants.hpp"

// Container for LEDs
class LEDs {
  public:
    LEDs();
    void init();
    void setAll(CRGB color);
  private:
    CRGB leds[LEDCount];
};
