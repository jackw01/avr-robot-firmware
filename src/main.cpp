// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include "robot.hpp"

Robot robot;

void setup() {
  robot.init();
}

void loop() {
  robot.tick();
}
