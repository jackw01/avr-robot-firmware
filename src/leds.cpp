// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include "leds.hpp"

LEDs::LEDs() {
}

void LEDs::init() {
  FastLED.addLeds<NEOPIXEL, PinLEDs>(leds, LEDCount);
  FastLED.setBrightness(LEDMasterBrightness);
  FastLED.show();
  setAll(ColorOff);
}

void LEDs::setAll(CRGB color) {
  for (int i = 0; i < LEDCount; i++) leds[i] = color;
  FastLED.show();
}
