// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include "comms.hpp"

void Comms::init() {
  Serial.begin(SerialBaudRate);
}

void Comms::writePacketStart(uint8_t dataType) {
  writeOut(PacketMarkerByte); // Tell the computer that the packet has started
  char buffer[18];
  leftPad(dataType, buffer, 3, '0');
  writeOut(buffer); // Tell the computer what is being sent
}

void Comms::writePacketData(const char* data) {
  writeOut(PacketSeparatorByte);
  writeOut(data); // Send the data
}

void Comms::writePacketData(int data) {
  char string[CommsNumberWidth];
  itoa(data, string, 10);
  writePacketData(string);
}

void Comms::writePacketData(float data) {
  char string[CommsNumberWidth];
  dtostrf(data, 0, CommsFloatPrecision, string);
  writePacketData(string);
}

void Comms::writePacketEnd() {
  writeOut(PacketMarkerByte); // Tell the computer that the packet has ended
  writeOut('\n');
}

void Comms::writePacket(uint8_t dataType, const char* data) {
  writePacketStart(dataType);
  writePacketData(data);
  writePacketEnd();
}

void Comms::writePacket(uint8_t dataType, int data) {
  writePacketStart(dataType);
  writePacketData(data);
  writePacketEnd();
}

void Comms::writePacket(uint8_t dataType, float data) {
  writePacketStart(dataType);
  writePacketData(data);
  writePacketEnd();
}

void Comms::writePacket(uint8_t dataType, int data[], uint8_t len) {
  writePacketStart(dataType);
  for (uint8_t i = 0; i < len; i++) writePacketData(data[i]);
  writePacketEnd();
}

void Comms::writePacket(uint8_t dataType, float data[], uint8_t len) {
  writePacketStart(dataType);
  for (uint8_t i = 0; i < len; i++) writePacketData(data[i]);
  writePacketEnd();
}

void Comms::leftPad(int n, char* buffer, int size, char padChar) {
  char string[8];
  itoa(n, string, 10);
  int len = strlen(string);
  if (len < size) {
    char padString[2];
    padString[0] = padChar;
    padString[1] = '\0';
    strcpy(buffer, padString);
    for (int i = 0; i < size - len - 1; i++) strcat(buffer, padString);
  }
  strcat(buffer, string);
}

int Comms::getAvailable() {
  return Serial.available();
}

uint8_t Comms::getNextByte() {
  return Serial.read();
}

void Comms::writeOut(uint8_t byte) {
  Serial.write(byte);
}

void Comms::writeOut(const char* bytes) {
  Serial.write(bytes);
}
