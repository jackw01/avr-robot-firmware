// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <Arduino.h>
#include "constants.hpp"

class Comms {
  public:
    static void init();
    static char* leftPad(int n, int size, char padChar);
    static void writePacket(uint8_t dataType, char* data);
    static void writePacket(uint8_t dataType, int data);
    static int getAvailable();
    static uint8_t nextByte();
  private:
    static void writeOut(uint8_t byte);
    static void writeOut(const char* bytes);
};