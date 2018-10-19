// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include "hbridgemotor.hpp"

HBridgeMotor::HBridgeMotor(uint8_t p1, uint8_t p2) {
  // Assign pins and set as output
  pinPWM = p1;
  pinDirection = p2;
  pinMode(pinPWM, OUTPUT);
  pinMode(pinDirection, OUTPUT);
}

void HBridgeMotor::setSpeed(float newSpeed) {
  speed = newSpeed;
  // Set direction pin and PWM value based on speed
  if (speed > 0) {
    digitalWrite(pinDirection, 0);
    analogWrite(pinPWM, speed * 255.0);
  } else if (speed < 0) {
    digitalWrite(pinDirection, 1);
    analogWrite(pinPWM, 255 - (speed * -255.0));
  } else {
    digitalWrite(pinDirection, 0);
    analogWrite(pinPWM, 0);
  }
}

float HBridgeMotor::getSpeed() {
  return speed;
}
