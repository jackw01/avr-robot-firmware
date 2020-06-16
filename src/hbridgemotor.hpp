// robot-bridge-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#include <stdint.h>
#include <Arduino.h>

// An DC motor being controlled by an H-bridge
class HBridgeMotor {
  public:
    HBridgeMotor(uint8_t p1, uint8_t p2);
    void setSpeed(float newSpeed);
    float getSpeed();
  private:
    uint8_t pinPWM;
    uint8_t pinDirection;
    float speed;
};
