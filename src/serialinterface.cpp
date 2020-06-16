// robot-bridge-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#include "serialinterface.hpp"

const char PacketStartByte = '{';
const char PacketEndByte = '}';
const char DataSeparatorByte = ':';
const char ArgsSeparatorByte = ',';
const char QuoteByte = '"';
const uint8_t NumberWidth = 8;
const uint8_t FloatPrecision = 4;

void SerialInterface::init(long baudRate) {
  Serial.begin(baudRate);
}

void SerialInterface::writePacketStart(uint8_t dataType) {
  writeOut(PacketStartByte);
  writeOut(QuoteByte);
  char buffer[18];
  leftPad(dataType, buffer, 3, '0');
  writeOut(buffer);
  writeOut(QuoteByte);
}

void SerialInterface::writePacketData(const char* data) {
  writeOut(DataSeparatorByte);
  writeOut(data); // Send the data
}

void SerialInterface::writePacketData(int data) {
  char string[NumberWidth];
  itoa(data, string, 10);
  writePacketData(string);
}

void SerialInterface::writePacketData(float data) {
  char string[NumberWidth];
  dtostrf(data, 0, FloatPrecision, string);
  writePacketData(string);
}

void SerialInterface::writePacketEnd() {
  writeOut(PacketEndByte);
  writeOut('\n');
}

void SerialInterface::writePacket(uint8_t dataType, const char* data) {
  writePacketStart(dataType);
  writePacketData(data);
  writePacketEnd();
}

void SerialInterface::writePacket(uint8_t dataType, int data) {
  writePacketStart(dataType);
  writePacketData(data);
  writePacketEnd();
}

void SerialInterface::writePacket(uint8_t dataType, float data) {
  writePacketStart(dataType);
  writePacketData(data);
  writePacketEnd();
}

void SerialInterface::writePacket(uint8_t dataType, int data[], uint8_t len) {
  writePacketStart(dataType);
  for (uint8_t i = 0; i < len; i++) writePacketData(data[i]);
  writePacketEnd();
}

void SerialInterface::writePacket(uint8_t dataType, float data[], uint8_t len) {
  writePacketStart(dataType);
  for (uint8_t i = 0; i < len; i++) writePacketData(data[i]);
  writePacketEnd();
}

void SerialInterface::leftPad(int n, char* buffer, int size, char padChar) {
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

int SerialInterface::getAvailable() {
  return Serial.available();
}

uint8_t SerialInterface::getNextByte() {
  return Serial.read();
}

void SerialInterface::writeOut(char byte) {
  Serial.write(byte);
}

void SerialInterface::writeOut(const char* bytes) {
  Serial.write(bytes);
}
