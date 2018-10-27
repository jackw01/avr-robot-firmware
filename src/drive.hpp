// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include <stdint.h>
#include <Arduino.h>
#include <pidcontroller.h>
#include <pidautotuner.h>
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

    // PID
    void setPID(double p, double i, double d);

    void update();

    bool getMoving();

  private:
    // Motors/encoders
    HBridgeMotor leftMotor = HBridgeMotor(PinMotorLPWM, PinMotorLDir);
    HBridgeMotor rightMotor = HBridgeMotor(PinMotorRPWM, PinMotorRDir);

    bool moving;

    // Control loops
    PID leftVelocityPID = PID(DriveP, DriveI, DriveD, -1.0, 1.0);
    PID rightVelocityPID = PID(DriveP, DriveI, DriveD, -1.0, 1.0);
};
