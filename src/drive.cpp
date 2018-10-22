// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include "drive.hpp"

Drive::Drive() {
}

// Init subystem and alibrate gyro
void Drive::init() {
  // Set up gyro
  gyro.enableAutoRange(true);
  if (!gyro.begin()) Comms::writePacket(0, "L3GD30 I2C gyro not detected");
  else Comms::writePacket(0, "L3GD30 I2C gyro connected");

  // Calibrate
  uint8_t samples = 100;  // Take a bunch of samples and average readings out
  for (uint8_t i = 0; i < samples; i++) {
    sensors_event_t event;
    gyro.getEvent(&event);
    gyroDriftX += event.gyro.x * 0.025;
    gyroDriftY += event.gyro.y * 0.025;
    gyroDriftZ += event.gyro.z * 0.025;
    delay(25);
  }
  gyroDriftX /= 100;
  gyroDriftY /= 100;
  gyroDriftZ /= 100;
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
  float deltaT = microseconds - lastMicroseconds; // Calculate time delta to rate limit the loop
  if (deltaT > DriveControlLoopInterval) {
    lastMicroseconds = microseconds;

    sensors_event_t event;
    gyro.getEvent(&event);
    lastGyroY = event.gyro.y * GyroGainY * ((float)DriveControlLoopInterval / 1000000.0) - gyroDriftY;

    Comms::writePacket(0, getFreeRAM());
  }
}

// Get whether robot is moving
bool Drive::getMoving() {
    return moving;
}
