// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include "robot.hpp"

Robot::Robot() {
}

// Initializes everything
void Robot::init() {
    // Start serial connection
    Serial.begin(constants::SerialBaudRate);
    Comms::writePacket(255, "abc");
}

// Update function, called in a loop
void Robot::tick() {
}
