// avr-robot-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#include <Arduino.h>
#include <Servo.h>

#include "constants.hpp"
#include "util.hpp"
#include "serialinterface.hpp"
#include "leds.hpp"
#include "imu.hpp"
#include "drive.hpp"

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
    LEDs leds = LEDs();
    IMU imu = IMU();
    Drive drive = Drive();
    Servo servos[sizeof(ServoPins)];

    // Timing
    uint32_t microseconds = 0;
    uint32_t lastMicroseconds = 0;

    // Servo timeout
    bool servosAttached = false;
    uint32_t lastServoAttachMicroseconds = 0;

    // System things
    long lastStatusCheckMicroseconds = 0;
    float measureBatteryVoltage();
};
