// robot-bridge-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <Arduino.h>

class SerialInterface {
  public:
    static void init(long baudRate);
    static void writePacket(uint8_t dataType, const char* data);
    static void writePacket(uint8_t dataType, int data);
    static void writePacket(uint8_t dataType, float data);
    static void writePacket(uint8_t dataType, int data[], uint8_t len);
    static void writePacket(uint8_t dataType, float data[], uint8_t len);
    static void leftPad(int n, char* buffer, int size, char padChar);
    static int getAvailable();
    static uint8_t getNextByte();

  private:
    static void writePacketStart(uint8_t dataType);
    static void writePacketData(const char* data);
    static void writePacketData(int data);
    static void writePacketData(float data);
    static void writePacketEnd();
    static void writeOut(char byte);
    static void writeOut(const char* bytes);
};
