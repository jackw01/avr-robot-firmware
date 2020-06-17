// robot-bridge-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <Arduino.h>

class SerialInterface {
  public:
    static SerialInterface& getInstance() {
      static SerialInterface instance;
      return instance;
    }

    struct Packet {
      bool ready;
      uint8_t type;
      float contents[4];
    };

    void begin(long baudRate);
    void writePacket(uint8_t dataType, const char* data);
    void writePacket(uint8_t dataType, int data);
    void writePacket(uint8_t dataType, float data);
    void writePacket(uint8_t dataType, int data[], uint8_t len);
    void writePacket(uint8_t dataType, float data[], uint8_t len);
    void leftPad(int n, char* buffer, int size, char padChar);

    Packet readIncomingData();

  private:
    SerialInterface() {}

    uint8_t incomingPacket[24];
    uint8_t packetIndex = 0;
    uint8_t packetType = 0;
    float packetContents[4];

    void writePacketStart(uint8_t dataType);
    void writePacketData(const char* data);
    void writePacketData(int data);
    void writePacketData(float data);
    void writePacketEnd();
    void writeOut(char byte);
    void writeOut(const char* bytes);

    int getAvailable();
    uint8_t getNextByte();

    uint8_t pow10(uint8_t n);
};
