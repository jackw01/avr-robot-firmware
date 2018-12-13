// robot-bridge-firmware
// Copyright 2018 jackw01. Released under the MIT License (see LICENSE for details).

#include "imu.hpp"

IMU::IMU() {
}

// Init subystem and calibrate sensors
void IMU::init() {
  // Set up gyro
  gyro.enableAutoRange(false);
  if (!gyro.begin()) Comms::writePacket(0, "L3GD30 I2C gyro not detected");
  else Comms::writePacket(0, "L3GD30 I2C gyro connected");
  if (!accel.begin()) Comms::writePacket(0, "LSM303D I2C accel not detected");
  else Comms::writePacket(0, "LSM303D I2C accel connected");
  if (!mag.begin()) Comms::writePacket(0, "LSM303D I2C mag not detected");
  else Comms::writePacket(0, "LSM303D I2C mag connected");
  delay(200); // Wait first - gyro returns bad data if queried immediately after initialization
}

// Calibrate gyro - Take a bunch of samples and average readings out
void IMU::calibrateGyro(uint16_t samples) {
  gyroOffset.x = 0;
  gyroOffset.y = 0;
  gyroOffset.z = 0;
  for (uint16_t i = 0; i < samples; i++) {
    sensors_event_t event;
    gyro.getEvent(&event);
    gyroOffset.x += event.gyro.x;
    gyroOffset.y += event.gyro.y;
    gyroOffset.z += event.gyro.z;
    delay(10);
  }
  gyroOffset.x /= (float)samples;
  gyroOffset.y /= (float)samples;
  gyroOffset.z /= (float)samples;
  lastGyro.roll = 0;
  lastGyro.pitch = 0;
  lastGyro.heading = 0;
  currentGyro.roll = 0;
  currentGyro.pitch = 0;
  currentGyro.heading = 0;
}

// Get latest values from sensors
void IMU::update() {
  sensors_event_t event;
  gyro.getEvent(&event);
  lastGyro.roll = (event.gyro.v[IMURollAxis] - gyroOffset.v[IMURollAxis]) * GyroGain.v[IMURollAxis];
  lastGyro.pitch = (event.gyro.v[IMUPitchAxis] - gyroOffset.v[IMUPitchAxis]) * GyroGain.v[IMUPitchAxis];
  lastGyro.heading = (event.gyro.v[IMUHeadingAxis] - gyroOffset.v[IMUHeadingAxis]) * GyroGain.v[IMUHeadingAxis];
  currentGyro.roll += lastGyro.roll * MainControlLoopIntervalS;
  currentGyro.pitch += lastGyro.pitch * MainControlLoopIntervalS;
  currentGyro.heading += lastGyro.heading * MainControlLoopIntervalS;
}

// Get orientation
vec3 IMU::getGyroOrientation() {
  return currentGyro;
}
