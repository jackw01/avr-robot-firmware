// avr-robot-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#include "robot.hpp"

Robot robot;

void setup() {
  robot.init();
}

void loop() {
  robot.tick();
}
