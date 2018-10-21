// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include <stdint.h>
#include "hbridgemotor.hpp"
#include "constants.hpp"

// A differential drivebase with one motor driving each side's wheels
class Drive {
  public:
    Drive();

    void setOpenLoopPower(float leftPower, float rightPower);

    bool getMoving();

  private:
    HBridgeMotor leftMotor = HBridgeMotor(PinMotorLPWM, PinMotorLDir);
    HBridgeMotor rightMotor = HBridgeMotor(PinMotorRPWM, PinMotorRDir);

    bool moving;
};
