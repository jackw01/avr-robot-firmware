// robot-bridge-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#include "statusled.hpp"

StatusLED::StatusLED() {
}

void StatusLED::init() {
  pinMode(PinBoardStatusLED, OUTPUT);
}

void StatusLED::update(uint32_t t) {
  if (stateBufferPos > 0 && stateBuffer[0].time <= t) {
    setState(stateBuffer[0].state);
    for (uint8_t i = 0; i < stateBufferSize - 1; i++) stateBuffer[i] = stateBuffer[i + 1];
    stateBufferPos--;
  }
}

void StatusLED::setState(bool state) {
  digitalWrite(PinBoardStatusLED, state);
}

void StatusLED::queueState(bool state, uint32_t time) {
  stateBuffer[stateBufferPos] = {state, time};
  stateBufferPos++;
}

void StatusLED::blink(uint16_t onTime, uint16_t offTime) {
  uint32_t t;
  if (stateBufferPos == 0) t = micros();
  else t = stateBuffer[stateBufferPos - 1].time + offTime;
  queueState(true, t);
  queueState(false, t + onTime);
}
