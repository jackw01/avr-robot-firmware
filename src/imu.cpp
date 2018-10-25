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

// Get latest values from sensors
void IMU::update() {
  sensors_event_t event;
  gyro.getEvent(&event);
  lastGyroY = (event.gyro.y - gyroDriftY) * GyroGainY;
  gyroAngleY += lastGyroY * ((float)MainControlLoopInterval / 1000000.0);

  Comms::writePacket(0, gyroAngleY);
}

// Get heading
float IMU::getHeading() {
  return gyroAngleY;
}
