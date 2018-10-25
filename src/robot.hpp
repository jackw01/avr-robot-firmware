// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include <Arduino.h>
#include "constants.hpp"
#include "util.hpp"
#include "imu.hpp"
#include "drive.hpp"
#include "comms.hpp"

// Main robot class with methods for initializing and updating subsystems
class Robot {
  public:
    Robot();
    void init();
    void tick();

  private:
    // Subsystems
    IMU imu = IMU();
    Drive drive = Drive();

    // Timing
    long microseconds = 0;
    long lastMicroseconds = 0;

    // Communication
    bool parseIncomingPackets(uint8_t nextByte);
    uint8_t incomingPacket[24];
    uint8_t packetIndex = 0;
    uint8_t packetType = 0;
    float packetContents[4];
};
