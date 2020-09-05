// avr-robot-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#include "leds.hpp"

LEDs::LEDs() {
}

void LEDs::init() {
  pinMode(PinBoardStatusLED, OUTPUT);
  pinMode(PinLEDs, OUTPUT);
  FastLED.addLeds<WS2812B, PinLEDs, GRB>(leds, LEDCount);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(LEDMasterBrightness);
  setAll(LEDOff);
}

void LEDs::update(uint32_t t) {
  if (stateBufferPos > 0 && stateBuffer[0].time <= t) {
    setBoardLEDState(stateBuffer[0].state);
    for (uint8_t i = 0; i < stateBufferSize - 1; i++) stateBuffer[i] = stateBuffer[i + 1];
    stateBufferPos--;
  }
}

void LEDs::setBoardLEDState(bool state) {
  digitalWrite(PinBoardStatusLED, state);
}

void LEDs::queueBoardLEDState(bool state, uint32_t time) {
  stateBuffer[stateBufferPos] = {state, time};
  stateBufferPos++;
}

void LEDs::blinkBoardLED(uint16_t onTime, uint16_t offTime) {
  uint32_t t;
  if (stateBufferPos == 0) t = micros();
  else t = stateBuffer[stateBufferPos - 1].time + offTime;
  queueBoardLEDState(true, t);
  queueBoardLEDState(false, t + onTime);
}

void LEDs::setAll(CRGB color) {
  for (uint8_t i = 0; i < LEDCount; i++) leds[i] = color;
  FastLED.show();
}
