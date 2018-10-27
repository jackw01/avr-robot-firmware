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

// Resets tick counts for both encoders
void Drive::resetDistanceCounter() {
}


// Set power of motors in open loop mode
void Drive::setOpenLoopPower(float leftPower, float rightPower) {
  moving = !(leftPower == 0 && rightPower == 0);
  leftMotor.setSpeed(leftPower);
  rightMotor.setSpeed(rightPower);
}

// Set velocities of motors in closed loop mode
void Drive::setVelocitySetpoint(DriveSignal velocity) {
}

// Set PID
void Drive::setPID(float p, float i, float d) {
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

// Get distance
Drive::DriveSignal Drive::getDistance() {
    return {0, 0};
}

// Get velocity
Drive::DriveSignal Drive::getVelocity() {
    return {0, 0};
}

// Convert between encoder ticks and distance
float Drive::encoderTicksToDistance(long ticks) {
    return (float)ticks / EncoderCPR / GearRatio * WheelCircumference;
}

// Convert between encoder ticks and distance
long Drive::distanceToEncoderTicks(float distance) {
    return (long)(distance / WheelCircumference * GearRatio * EncoderCPR);
}
