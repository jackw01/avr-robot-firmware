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
    static void writePacket(uint8_t dataType, char* data);
    static void writePacket(uint8_t dataType, int data);
    static void writePacket(uint8_t dataType, float data);
    static void writePacket(uint8_t dataType, int data[], size_t len);
    static void writePacket(uint8_t dataType, float data[], size_t len);
    static char* leftPad(int n, int size, char padChar);
    static int getAvailable();
    static uint8_t getNextByte();

  private:
    static void writeOut(uint8_t byte);
    static void writeOut(const char* bytes);
    static char* toString(int n);
    static char* toString(float n);
};
