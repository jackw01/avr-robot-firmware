// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

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

// Calibrate gyro
void IMU::calibrateGyro() {
  uint16_t samples = 1000;  // Take a bunch of samples and average readings out
  for (uint16_t i = 0; i < samples; i++) {
    sensors_event_t event;
    gyro.getEvent(&event);
    gyroDrift.x += event.gyro.x;
    gyroDrift.y += event.gyro.y;
    gyroDrift.z += event.gyro.z;
    delay(10);
  }
  gyroDrift.x /= (float)samples;
  gyroDrift.y /= (float)samples;
  gyroDrift.z /= (float)samples;
}

// Get latest values from sensors
void IMU::update() {
  sensors_event_t event;
  gyro.getEvent(&event);
  lastGyro.roll = (event.gyro.v[IMURollAxis] - gyroDrift.v[IMURollAxis]) * GyroGain.v[IMURollAxis];
  lastGyro.pitch = (event.gyro.v[IMUPitchAxis] - gyroDrift.v[IMUPitchAxis]) * GyroGain.v[IMUPitchAxis];
  lastGyro.heading = (event.gyro.v[IMUHeadingAxis] - gyroDrift.v[IMUHeadingAxis]) * GyroGain.v[IMUHeadingAxis];
  currentGyro.roll += lastGyro.roll * MainControlLoopIntervalS;
  currentGyro.pitch += lastGyro.pitch * MainControlLoopIntervalS;
  currentGyro.heading += lastGyro.heading * MainControlLoopIntervalS;
}

// Get orientation
float IMU::getGyroOrientation() {
  return currentGyro;
}
