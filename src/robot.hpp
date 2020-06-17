// robot-bridge-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#include <Arduino.h>

#include "constants.hpp"
#include "util.hpp"
#include "serialinterface.hpp"
#include "imu.hpp"
#include "drive.hpp"
#include "leds.hpp"

// Main robot class with methods for initializing and updating subsystems
class Robot {
  public:
    Robot();
    void init();
    void tick();

  private:
    // Serial
    SerialInterface serial = SerialInterface::getInstance();

    // Subsystems
    IMU imu = IMU();
    Drive drive = Drive();
    LEDs leds = LEDs();

    // Timing
    long microseconds = 0;
    long lastMicroseconds = 0;

    // System things
    long lastStatusCheckMicroseconds = 0;
    float measureBatteryVoltage();
};
