// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include "drive.hpp"

Drive::Drive() {
}

// Init subystem
void Drive::init() {
}

// Set power of motors in open loop mode
void Drive::setOpenLoopPower(float leftPower, float rightPower) {
  moving = !(leftPower == 0 && rightPower == 0);
  leftMotor.setSpeed(leftPower);
  rightMotor.setSpeed(rightPower);
}

// Update drivebase
void Drive::update() {
  microseconds = micros();
  long deltaT = microseconds - lastMicroseconds; // Calculate time delta to rate limit the loop
  if (deltaT > MainControlLoopInterval) {
    lastMicroseconds = microseconds;

  }
}

// Get whether robot is moving
bool Drive::getMoving() {
    return moving;
}
