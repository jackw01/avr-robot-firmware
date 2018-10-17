// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include <Arduino.h>

#include "serial.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Test");
  Serial.print(pad(1, 3, "0"));
  Serial.println("end");
}

void loop() {

}
