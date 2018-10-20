// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#pragma once

namespace constants {
  static const long SerialBaudRate = 115200;
  static const uint8_t PacketMarkerByte = '\\';
  static const uint8_t PacketSeparatorByte = ' ';
  static const uint8_t PinMotorLPWM = 6;
  static const uint8_t PinMotorLDir = 7;
  static const uint8_t PinMotorRPWM = 11;
  static const uint8_t PinMotorRDir = 10;
}
