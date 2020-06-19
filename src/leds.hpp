// robot-bridge-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#include <Arduino.h>
#include <FastLED.h>

#include "constants.hpp"

static const CRGB LEDOff = CRGB(0, 0, 0);
static const CRGB ColorRed = CRGB(255, 0, 0);
static const CRGB ColorOrange = CRGB(255, 64, 0);
static const CRGB ColorYellow = CRGB(255, 237, 0);
static const CRGB ColorGreen = CRGB(0, 255, 10);
static const CRGB ColorAqua = CRGB(0, 255, 164);
static const CRGB ColorCyan = CRGB(0, 247, 255);
static const CRGB ColorBlue = CRGB(0, 21, 255);
static const CRGB ColorMagenta = CRGB(255, 0, 96);
static const CRGB ColorWhite = CRGB(255, 255, 255);

class LEDs {
  public:
    struct LEDState {
      bool state;
      uint32_t time;
    };

    LEDs();
    void init();
    void update(uint32_t t);

    void setBoardLEDState(bool state);
    void queueBoardLEDState(bool state, uint32_t time);
    void blinkBoardLED(uint16_t onTime, uint16_t offTime);

    void setAll(CRGB color);

  private:
    CRGB leds[LEDCount];
    static const uint8_t stateBufferSize = 4;
    LEDState stateBuffer[stateBufferSize];
    uint8_t stateBufferPos = 0;
};
