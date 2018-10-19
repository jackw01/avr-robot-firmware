// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include "comms.hpp"

void Comms::writePacket(uint8_t dataType, char* data) {
  writeOut(constants::PacketMarkerByte); // Tell the computer that the packet has started
  writeOut(leftPad(dataType, 3, '0')); // Tell the computer what is being sent
  writeOut(constants::PacketSeparatorByte);
  writeOut(data); // Send the data
  writeOut(constants::PacketMarkerByte); // Tell the computer that the packet has ended
}

void Comms::writePacket(uint8_t dataType, int data) {
  char* string = new char[8];
  itoa(data, string, 10);
  writePacket(dataType, string);
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

void Comms::writeOut(uint8_t byte) {
  Serial.write(byte);
}

void Comms::writeOut(const char* bytes) {
  Serial.write(bytes);
}
