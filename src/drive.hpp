// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include "hbridgemotor.hpp"
#include "constants.hpp"
#include "util.hpp"
#include "comms.hpp"

// A differential drivebase with one motor driving each side's wheels
class Drive {
  public:
    Drive();

    void init();

    void setOpenLoopPower(float leftPower, float rightPower);

    void update();

    bool getMoving();

  private:
    HBridgeMotor leftMotor = HBridgeMotor(PinMotorLPWM, PinMotorLDir);
    HBridgeMotor rightMotor = HBridgeMotor(PinMotorRPWM, PinMotorRDir);

    bool moving;

    // Loop variables
    long microseconds = 0;
    long lastMicroseconds = 0;

    // Gyro
    Adafruit_L3GD20_Unified gyro;
    float gyroDriftX = 0;
    float gyroDriftY = 0;
    float gyroDriftZ = 0;
    float lastGyroY;
    float gyroAngleY = 0;
};
