// robot-bridge-firmware
// Copyright 2018 jackw01. Released under the MIT License (see LICENSE for details).

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
    void calibrateGyro(uint16_t samples = 500);

    void update();
    vec3 getGyroOrientation();

  private:
    // Sensors
    Adafruit_L3GD20_Unified gyro;
    Adafruit_LSM303_Accel_Unified accel;
    Adafruit_LSM303_Mag_Unified mag;

    // Gyro
    vec3 gyroOffset;
    vec3 lastGyro;
    vec3 currentGyro;
};
