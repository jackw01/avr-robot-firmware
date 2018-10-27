// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include "drive.hpp"

Drive::Drive() {
}

// Init subystem
void Drive::init() {
  moving = false;
}

// Set power of motors in open loop mode
void Drive::setOpenLoopPower(float leftPower, float rightPower) {
  moving = !(leftPower == 0 && rightPower == 0);
  leftMotor.setSpeed(leftPower);
  rightMotor.setSpeed(rightPower);
}

// Set PID
void Drive::setPID(double p, double i, double d) {
    leftVelocityPID.setPID(p, i, d);
    rightVelocityPID.setPID(p, i, d);
}

// Update drivebase
void Drive::update() {

}

// Get whether robot is moving
bool Drive::getMoving() {
  return moving;
}
