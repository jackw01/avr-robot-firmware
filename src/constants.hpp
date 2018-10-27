// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#pragma once

#include "util.hpp"
#include <FastLED.h>

// Pin mapping
const uint8_t PinMotorLPWM = 6;
const uint8_t PinMotorLDir = 7;
const uint8_t PinMotorRPWM = 11;
const uint8_t PinMotorRDir = 10;
const uint8_t PinLEDs = 13;

// Drive control
const long MainControlLoopIntervalUs = 25000;
const float MainControlLoopIntervalS = (float)MainControlLoopIntervalUs / 1000000.0;
const float DriveP = 200.0;
const float DriveI = 400.0;
const float DriveD = 0.0;

// IMU
// IMU axis mapping (0,1,2 represent x,y,z)
const uint8_t IMURollAxis = 2;
const uint8_t IMUPitchAxis = 0;
const uint8_t IMUHeadingAxis = 1;

// Calibration data
const vec3 GyroGain = {1.0, 1.0180888, 1.0};

// Misc features
// LEDs
const uint8_t LEDCount = 4;
const uint8_t LEDMasterBrightness = 32;
const CRGB ColorRed = CRGB(255, 0, 0);
const CRGB ColorOrange = CRGB(255, 60, 0);
const CRGB ColorYellow = CRGB(255, 237, 0);
const CRGB ColorGreen = CRGB(0, 255, 10);
const CRGB ColorCyan = CRGB(0, 247, 255);
const CRGB ColorBlue = CRGB(0, 21, 255);
const CRGB ColorMagenta = CRGB(190, 0, 255);
const CRGB ColorWhite = CRGB(255, 255, 255);
const CRGB ColorOff = CRGB(0, 0, 0);

// Communication
// Serial settings
const long SerialBaudRate = 115200;
const uint8_t PacketMarkerByte = '\\';
const uint8_t PacketSeparatorByte = ' ';
const uint8_t CommsNumberWidth = 8;
const uint8_t CommsFloatPrecision = 4;

// Datatypes
const unsigned char DataTypeHumanReadable = 0;    // Expected: string
const unsigned char DataTypeGyro = 2;             // Expected: float[3]
