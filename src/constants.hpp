// robot-bridge-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#pragma once

#include <math.h>
#include <FastLED.h>

#include "util.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Subsystems
const bool EnableDrive = false;
const bool EnableIMU = true;
const bool EnableVoltageMonitoring = true;
const bool EnableDebugMessaging = true;

// Pin mapping
const uint8_t PinMotorLPWM = 6;
const uint8_t PinMotorLDir = 7;
const uint8_t PinMotorRPWM = 11;
const uint8_t PinMotorRDir = 10;
const uint8_t PinLeftEncoderA = 2;
const uint8_t PinLeftEncoderB = 4;
const uint8_t PinRightEncoderA = 3;
const uint8_t PinRightEncoderB = 5;
const uint8_t PinLEDs = 12;
const uint8_t PinBoardStatusLED = 13;
const uint8_t PinBatteryVoltageDivider = 3;

// Drive control
const long MainControlLoopIntervalUs = 50000; // Was 25000
const float MainControlLoopIntervalS = (float)MainControlLoopIntervalUs / 1000000.0;
const float DriveP = 0.001;
const float DriveI = 0.0008;
const float DriveD = 0.0;

// Drive hardware constants
const float EncoderCPR = 3.0;
const float GearRatio = 51.44615;
const float LeftWheelDiameter = 33.6;
const float RightWheelDiameter = 32.0;
const float LeftWheelCircumference = LeftWheelDiameter * M_PI;
const float RightWheelCircumference = RightWheelDiameter * M_PI;
const float WheelCircumference = (LeftWheelCircumference + RightWheelCircumference) / 2.0;
const float LeftWheelCorrectionFactor = LeftWheelDiameter / fmin(LeftWheelDiameter, RightWheelDiameter);
const float RightWheelCorrectionFactor = RightWheelDiameter / fmin(LeftWheelDiameter, RightWheelDiameter);
const float TrackDiameter = 82.0;
const float SpeedDeadband = 0.0;

// IMU
// IMU axis mapping (0,1,2 represent x,y,z)
const uint8_t IMURollAxis = 2;
const uint8_t IMUPitchAxis = 0;
const uint8_t IMUHeadingAxis = 1;

// Calibration data
const vec3 GyroGain = {1.0, 1.0180888, 1.0};

// Misc features
// LEDs
const uint8_t LEDCount = 2;
const uint8_t LEDMasterBrightness = 10;
const CRGB ColorRed = CRGB(255, 0, 0);
const CRGB ColorOrange = CRGB(255, 64, 0);
const CRGB ColorYellow = CRGB(255, 237, 0);
const CRGB ColorGreen = CRGB(0, 255, 10);
const CRGB ColorAqua = CRGB(0, 255, 164);
const CRGB ColorCyan = CRGB(0, 247, 255);
const CRGB ColorBlue = CRGB(0, 21, 255);
const CRGB ColorMagenta = CRGB(255, 0, 96);
const CRGB ColorWhite = CRGB(255, 255, 255);
const CRGB ColorOff = CRGB(0, 0, 0);

// Battery
const bool EnableLowVoltageCutoff = false;
const float BatteryLowCellVoltage = 0.8;
const float BatteryCellCount = 5;
const float BatteryDividerRatio = 2.0;
const float ControllerSupplyVoltage = 5.0;

// Communication
// Serial settings
const long SerialBaudRate = 115200;
const long SystemStatusCheckIntervalUs = 1000000;

// Data and command types
typedef enum : uint8_t {
  DataTypeHumanReadable,         // Expected: string
  DataTypeBatteryVoltage,        // Expected: float
  DataTypeGyro,                  // Expected: float[3]
  DataTypeDriveDistance,         // Expected: float[2]
  DataTypeDriveControlData,      // Expected: float[7]
  DataTypeFreeRAM                // Expected: int
} DataType;

typedef enum : uint8_t {
  CmdTypeSetDriveOpenLoop,       // Expected: float[2]
  CmdTypeSetDriveClosedLoop,     // Expected: float[2]
  CmdTypeEnableDriveClosedLoop,  // Expected: none
  CmdTypeDisableDriveClosedLoop, // Expected: none
  CmdTypeSetDrivePIDTuning,      // Expected: float[3]
  CmdTypeSetAllStatusLEDs,       // Expected: int[3]
  CmdTypeCalibrateGyro,          // Expected: int
  CmdTypeResetDrive              // Expected: none
} CmdType;
