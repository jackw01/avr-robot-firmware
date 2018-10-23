// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include "drive.hpp"

Drive::Drive() {
}

// Init subystem and alibrate gyro
void Drive::init() {
  // Set up gyro
  gyro.enableAutoRange(false);
  if (!gyro.begin()) Comms::writePacket(0, "L3GD30 I2C gyro not detected");
  else Comms::writePacket(0, "L3GD30 I2C gyro connected");

  // Calibrate
  delay(200); // Wait first - gyro returns bad data if queried immediately after initialization
  uint16_t samples = 1000;  // Take a bunch of samples and average readings out
  for (uint16_t i = 0; i < samples; i++) {
    sensors_event_t event;
    gyro.getEvent(&event);
    gyroDriftX += event.gyro.x;
    gyroDriftY += event.gyro.y;
    gyroDriftZ += event.gyro.z;
    delay(10);
  }
  gyroDriftX /= (float)samples;
  gyroDriftY /= (float)samples;
  gyroDriftZ /= (float)samples;
  Comms::writePacket(1, gyroDriftY);
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
    lastGyroY = (event.gyro.y - gyroDriftY) * GyroGainY;
    gyroAngleY += lastGyroY * ((float)DriveControlLoopInterval / 1000000.0);

    Comms::writePacket(0, gyroAngleY);
  }
}

// Get whether robot is moving
bool Drive::getMoving() {
    return moving;
}
