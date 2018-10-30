// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include <Arduino.h>
#include <FastLED.h>

#include "constants.hpp"

// Container for LEDs
class LEDs {
  public:
    LEDs();
    void init();
    void setAll(CRGB color);
    void blinkAll(CRGB color1, CRGB color2, uint16_t count, uint16_t time1, uint16_t time2);
  private:
    CRGB leds[LEDCount];
};
