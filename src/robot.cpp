// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include "robot.hpp"

// Constructor
Robot::Robot() {
}

// Initializes everything
void Robot::init() {
    // Start serial connection
    Serial.begin(constants::SerialBaudRate);
    Serial.println("Test");
    Serial.println(serial::leftPad(3, 3, '0'));
}

// Update function, called in a loop
void Robot::tick() {
}
