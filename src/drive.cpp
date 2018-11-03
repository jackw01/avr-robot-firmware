// robot-bridge-firmware
// Copyright 2018 jackw01. Released under the MIT License (see LICENSE for details).

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

// Set state
void Drive::setState(DriveState state) {
  currentState = state;

  // Reset things when entering/exiting closed loop mode
  if (state == DriveStateClosedLoop) {
    moving = true;
    resetDistanceCounter();
    sei(); // Make sure encoder interrupts on
    prevMicros = micros();
    leftVelocityPID.reset();
    rightVelocityPID.reset();
  } else if (state == DriveStateOpenLoop) {
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
    moving = false;
  }
}

// Set power of motors in open loop mode
void Drive::setOpenLoopPower(float leftPower, float rightPower) {
  moving = !(leftPower == 0 && rightPower == 0);
  leftMotor.setSpeed(leftPower);
  rightMotor.setSpeed(rightPower);
}

// Set velocities of motors in closed loop mode
void Drive::setVelocitySetpoint(DriveSignal velocity) {
  leftVelocityPID.setSetpoint(velocity.left);// / LeftWheelCircumference * GearRatio * EncoderCPR / 1000000.0);
  rightVelocityPID.setSetpoint(velocity.right);// / RightWheelCircumference * GearRatio * EncoderCPR / 1000000.0);
}

// Set PID
void Drive::setPID(float p, float i, float d) {
  leftVelocityPID.setPID(p, i, d);
  rightVelocityPID.setPID(p, i, d);
}

// Update drivebase
void Drive::update() {
  if (currentState == DriveStateClosedLoop) {
    closedLoopUpdate();
  }
}

// Run a cycle in closed loop mode1
void Drive::closedLoopUpdate() {
  DriveVelocity vel = getVelocity(); // Calculate velocity and run PID
  float outputL = leftVelocityPID.calculate(vel.left);
  float outputR = rightVelocityPID.calculate(vel.right);

  // Calculate final speeds - deadband may be required to prevent stalling
  // Zero speeds if setpoint is zero
  float newOutputL, newOutputR;
  if (leftVelocityPID.getSetpoint() == 0) newOutputL = 0;
  else {
    if (outputL < 0) newOutputL = constrain(outputL, min(-1, -SpeedDeadband), max(-1, -SpeedDeadband));
    else if (outputL > 0) newOutputL = constrain(outputL, min(SpeedDeadband, 1), max(SpeedDeadband, 1));
  }
  if (rightVelocityPID.getSetpoint() == 0) newOutputR = 0;
  else {
    if (outputR < 0) newOutputR = constrain(outputR, min(-1, -SpeedDeadband), max(-1, -SpeedDeadband));
    else if (outputR > 0) newOutputR = constrain(outputR, min(SpeedDeadband, 1), max(SpeedDeadband, 1));
  }

  leftMotor.setSpeed(newOutputL);
  rightMotor.setSpeed(newOutputR);

  // Send odometry data
  Comms::writePacket(DataTypeDriveDistance, (float*)&getDistance(), 2);

  // Send loop telemetry
  DriveControlLoopData data = {
    leftVelocityPID.getSetpoint(), rightVelocityPID.getSetpoint(), vel.left, vel.right,
    newOutputL, newOutputR, vel.deltaT
  };
  Comms::writePacket(DataTypeDriveControlData, (float*)&data, 6);
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
DriveVelocity Drive::getVelocity() {
  long currentMicros = micros();
  long dT = currentMicros - prevMicros;
  DriveVelocity ret = {
    ((float)leftTicks - (float)prevLeftTicks) / EncoderCPR / GearRatio * LeftWheelCircumference / dT * 1000000.0,
    ((float)rightTicks - (float)prevRightTicks) / EncoderCPR / GearRatio * RightWheelCircumference / dT * 1000000.0,
    dT
  };
  prevLeftTicks = leftTicks;
  prevRightTicks = rightTicks;
  prevMicros = currentMicros;
  return ret;
}
