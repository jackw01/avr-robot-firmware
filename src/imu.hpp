// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303_U.h>
#include "constants.hpp"
#include "util.hpp"
#include "comms.hpp"

// Container for all inertial sensors
class IMU {
  public:
    IMU();

    void init();

    void update();

    float getHeading();

  private:
    // Gyro
    Adafruit_L3GD20_Unified gyro;
    Adafruit_LSM303_Accel_Unified accel;
    Adafruit_LSM303_Mag_Unified mag;
    float gyroDriftX = 0;
    float gyroDriftY = 0;
    float gyroDriftZ = 0;
    float lastGyroY;
    float gyroAngleY = 0;
};
