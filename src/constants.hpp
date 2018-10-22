// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#pragma once

const uint8_t PinMotorLPWM = 6;
const uint8_t PinMotorLDir = 7;
const uint8_t PinMotorRPWM = 11;
const uint8_t PinMotorRDir = 10;

const long DriveControlLoopInterval = 250000; // in microseconds

const float GyroGainY = 1.102313;

const long SerialBaudRate = 115200;
const uint8_t PacketMarkerByte = '\\';
const uint8_t PacketSeparatorByte = ' ';
const uint8_t CommsNumberWidth = 8;
const uint8_t CommsFloatPrecision = 4;
