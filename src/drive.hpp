// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include <stdint.h>
#include <Arduino.h>
#include <FastGPIO.h>
#include <pidcontroller.h>
#include <pidautotuner.h>

#include "hbridgemotor.hpp"
#include "constants.hpp"
#include "util.hpp"
#include "comms.hpp"

// Types for drive distance and velocity signals
typedef struct {
  union {
    float v[2];
    struct {
      float left;
      float right;
    };
  };
} DriveSignal;

typedef struct {
  union {
    float v[3];
    struct {
      float left;
      float right;
      float deltaT;
    };
  };
} DriveVelocity;

typedef struct DriveControlLoopData {
  float leftSetpoint;
  float rightSetpoint;
  float leftVelocity;
  float rightVelocity;
  float leftMotorOutput;
  float rightMotorOutput;
  float deltaT;
} DriveControlLoopData;

typedef enum {
  DriveStateOpenLoop,
  DriveStateClosedLoop
} DriveState;

// A differential drivebase with one motor driving each side's wheels
class Drive {
  public:
    Drive();

    void init();
    void resetDistanceCounter();

    void setState(DriveState state);
    void setOpenLoopPower(float leftPower, float rightPower);
    void setVelocitySetpoint(DriveSignal velocity);

    void setPID(float p, float i, float d);

    void update();

    bool getMoving();
    DriveSignal getDistance();
    DriveVelocity getVelocity();

  private:
    // Motors
    HBridgeMotor leftMotor = HBridgeMotor(PinMotorLPWM, PinMotorLDir);
    HBridgeMotor rightMotor = HBridgeMotor(PinMotorRPWM, PinMotorRDir);

    // Encoder counter
    long prevLeftTicks = 0;
    long prevRightTicks = 0;

    // State
    long prevMicros = 0;
    bool moving = false;
    DriveState currentState = DriveStateOpenLoop;

    // Control loops
    PID leftVelocityPID = PID(DriveP, DriveI, DriveD, -1.0, 1.0);
    PID rightVelocityPID = PID(DriveP, DriveI, DriveD, -1.0, 1.0);

    // Internal update methods
    void closedLoopUpdate();
};
