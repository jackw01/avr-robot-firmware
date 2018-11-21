// robot-bridge-firmware
// Copyright 2018 jackw01. Released under the MIT License (see LICENSE for details).

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

void LEDs::blinkAll(CRGB color1, CRGB color2, uint16_t count, uint16_t time1, uint16_t time2) {
  uint16_t counter = 0;
  while (counter <= count) { // Blink forever if count == 0
    setAll(color1);
    delay(time1);
    setAll(color2);
    delay(time2);
    if (count > 0) counter++;
  }
}
