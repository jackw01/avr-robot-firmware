// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include "drive.hpp"

// Encoder tick counts
static long leftTicks = 0;
static long rightTicks = 0;

// Interrupt Service Routines for the encoders
static void leftEncoderISR() {
  // Determine whether signal B is high - if it is, the shaft is turning in reverse.
  // If not, it is turning forwards.
  bool b = FastGPIO::Pin<PinLeftEncoderB>::isInputHigh();
  if (b) leftTicks ++;
  else leftTicks --;
}

static void rightEncoderISR() {
  bool b = FastGPIO::Pin<PinRightEncoderB>::isInputHigh();
  if (b) rightTicks --;
  else rightTicks ++;
}

Drive::Drive() {
}

// Init subystem
void Drive::init() {
  // Set pin modes
  FastGPIO::Pin<PinLeftEncoderA>::setInput();
  FastGPIO::Pin<PinLeftEncoderB>::setInput();
  FastGPIO::Pin<PinRightEncoderA>::setInput();
  FastGPIO::Pin<PinRightEncoderB>::setInput();

  // Attach ISRs to encoder A pins
  cli();
  attachInterrupt(digitalPinToInterrupt(PinLeftEncoderA), leftEncoderISR, RISING);
  attachInterrupt(digitalPinToInterrupt(PinRightEncoderA), rightEncoderISR, RISING);
  sei();

  moving = false;
}

// Resets tick counts for both encoders
void Drive::resetDistanceCounter() {
  leftTicks = 0;
  rightTicks = 0;
  prevLeftTicks = 0;
  prevRightTicks = 0;
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
  DriveSignal vel = getDistance();

  DriveSignal v2 = {vel.left * 1.0, vel.right * 1.0};
  Comms::writePacket(DataTypeDriveDistance, (float*)&v2, 2);
}

// Get whether robot is moving
bool Drive::getMoving() {
  return moving;
}

// Get distance
DriveSignal Drive::getDistance() {
  return {(float)leftTicks / EncoderCPR / GearRatio * LeftWheelCircumference,
          (float)rightTicks / EncoderCPR / GearRatio * RightWheelCircumference};
}

// Get velocity
DriveSignal Drive::getVelocity() {
  long currentMicros = micros();
  long deltaT = currentMicros - prevMicros;
  DriveSignal ret = {
    ((float)leftTicks - (float)prevLeftTicks) / EncoderCPR / GearRatio * LeftWheelCircumference / deltaT,
    ((float)rightTicks - (float)prevRightTicks) / EncoderCPR / GearRatio * RightWheelCircumference / deltaT
  };
  prevLeftTicks = leftTicks;
  prevRightTicks = rightTicks;
  prevMicros = currentMicros;
  return ret;
}

// Convert between encoder ticks and distance
float Drive::encoderTicksToDistance(long ticks) {
  return (float)ticks / EncoderCPR / GearRatio * WheelCircumference;
}

// Convert between encoder ticks and distance
long Drive::distanceToEncoderTicks(float distance) {
  return (long)(distance / WheelCircumference * GearRatio * EncoderCPR);
}
