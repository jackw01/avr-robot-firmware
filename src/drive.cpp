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
  cli(); // Disable interrupts before attaching and then enable
  attachInterrupt(digitalPinToInterrupt(PinLeftEncoderA), leftEncoderISR, RISING);
  attachInterrupt(digitalPinToInterrupt(PinRightEncoderA), rightEncoderISR, RISING);
  sei();
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

// Start closed loop mode
void Drive::closedLoopBegin() {
  moving = true;
  resetDistanceCounter();
  sei(); // Make sure encoder interrupts on
  microseconds = micros();
  leftVelocityPID.reset();
  rightVelocityPID.reset();
}

// Run a cycle in closed loop mode1
void Drive::closedLoopUpdate() {
    // Calculate time delta
    long prevMicroseconds = microseconds;
    microseconds = micros();
    float deltaT = microseconds - prevMicroseconds;

    DriveSignal vel = getVelocity(); // Calculate velocity

    float outputL = leftVelocityPID.calculate(vel.leftVelocity);
    float outputR = rightVelocityPID.calculate(vel.rightVelocity);

    // Calculate final speeds - minimum speed is required to prevent stalling
    float newLeftSpeed, newRightSpeed;

    if (outputL < 0) newLeftSpeed = constrain(outputL, min(-1, -leftMinMoveSpeed), max(-1, -leftMinMoveSpeed));
    else if (outputL > 0) newLeftSpeed = constrain(outputL, min(leftMinMoveSpeed, 1), max(leftMinMoveSpeed, 1));
    if (outputR < 0) newRightSpeed = constrain(outputR, min(-1, -rightMinMoveSpeed), max(-1, -rightMinMoveSpeed));
    else if (outputR > 0) newRightSpeed = constrain(outputR, min(rightMinMoveSpeed, 1), max(rightMinMoveSpeed, 1));

    // Zero speeds if setpoint is zero
    if (leftVelocityPID.getSetpoint() == 0) newLeftSpeed = 0;
    if (rightVelocityPID.getSetpoint() == 0) newRightSpeed = 0;
}

// End closed loop mode
void Drive::closedLoopEnd() {
  leftMotor.setSpeed(0);
  rightMotor.setSpeed(0);
  moving = false;
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
