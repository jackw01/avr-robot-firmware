// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include "comms.hpp"

void Comms::init() {
  Serial.begin(SerialBaudRate);
}

void Comms::writePacket(uint8_t dataType, char* data) {
  writeOut(PacketMarkerByte); // Tell the computer that the packet has started
  writeOut(leftPad(dataType, 3, '0')); // Tell the computer what is being sent
  writeOut(PacketSeparatorByte);
  writeOut(data); // Send the data
  writeOut(PacketMarkerByte); // Tell the computer that the packet has ended
}

void Comms::writePacket(uint8_t dataType, int data) {
  writePacket(dataType, toString(data));
}

void Comms::writePacket(uint8_t dataType, float data) {
  writePacket(dataType, toString(data));
}

void Comms::writePacket(uint8_t dataType, int data[], size_t len) {
  writeOut(PacketMarkerByte);
  writeOut(leftPad(dataType, 3, '0'));
  for (uint8_t i = 0; i < len; i++) {
    writeOut(PacketSeparatorByte);
    writeOut(toString(data[i]));
  }
  writeOut(PacketMarkerByte);
}

void Comms::writePacket(uint8_t dataType, float data[], size_t len) {
  writeOut(PacketMarkerByte);
  writeOut(leftPad(dataType, 3, '0'));
  for (uint8_t i = 0; i < len; i++) {
    writeOut(PacketSeparatorByte);
    writeOut(toString(data[i]));
  }
  writeOut(PacketMarkerByte);
}

char* Comms::leftPad(int n, int size, char padChar) {
  char* string = new char[8];
  itoa(n, string, 10);
  int len = strlen(string);
  if (len >= size) return string;
  char* buffer = new char[18];
  char* padString = new char[2];
  padString[0] = padChar;
  padString[1] = '\0';
  strcpy(buffer, padString);
  for (int i = 0; i < size - len - 1; i++) strcat(buffer, padString);
  strcat(buffer, string);
  return buffer;
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

char* Comms::toString(int n) {
  char* string = new char[CommsNumberWidth];
  itoa(n, string, 10);
  return string;
}

char* Comms::toString(float n) {
  char* string = new char[CommsNumberWidth];
  dtostrf(n, 0, CommsFloatPrecision, string);
  return string;
}
