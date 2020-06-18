// robot-bridge-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#include <Arduino.h>

#include "constants.hpp"

class StatusLED {
  public:
    struct LEDState {
      bool state;
      uint32_t time;
    };

    StatusLED();
    void init();
    void update(uint32_t t);
    void setState(bool state);
    void queueState(bool state, uint32_t time);
    void blink(uint16_t onTime, uint16_t offTime);

  private:
    static const uint8_t stateBufferSize = 4;
    LEDState stateBuffer[stateBufferSize];
    uint8_t stateBufferPos = 0;
};
