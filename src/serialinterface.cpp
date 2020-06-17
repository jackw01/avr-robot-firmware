// robot-bridge-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#include "serialinterface.hpp"

const char PacketStartByte = '{';
const char PacketEndByte = '}';
const char ArrayStartByte = '[';
const char ArrayEndByte = ']';
const char TypeSeparatorByte = ':';
const char ContentSeparatorByte = ',';
const char QuoteByte = '"';
const uint8_t NumberWidth = 8;
const uint8_t FloatPrecision = 4;
const uint8_t MaxContentLength = 4;

void SerialInterface::begin(long baudRate) {
  Serial.begin(baudRate);
}

void SerialInterface::writePacket(uint8_t dataType, const char* data) {
  writePacketStart(dataType);
  writeOut(QuoteByte);
  writeOut(data);
  writeOut(QuoteByte);
  writePacketEnd();
}

void SerialInterface::writePacket(uint8_t dataType, int data) {
  writePacketStart(dataType);
  writeOut(ArrayStartByte);
  writePacketData(data);
  writeOut(ArrayEndByte);
  writePacketEnd();
}

void SerialInterface::writePacket(uint8_t dataType, float data) {
  writePacketStart(dataType);
  writeOut(ArrayStartByte);
  writePacketData(data);
  writeOut(ArrayEndByte);
  writePacketEnd();
}

void SerialInterface::writePacket(uint8_t dataType, int data[], uint8_t len) {
  writePacketStart(dataType);
  writeOut(ArrayStartByte);
  for (uint8_t i = 0; i < len; i++) {
    writePacketData(data[i]);
    if (i < len - 1) writeOut(ContentSeparatorByte);
  }
  writeOut(ArrayEndByte);
  writePacketEnd();
}

void SerialInterface::writePacket(uint8_t dataType, float data[], uint8_t len) {
  writePacketStart(dataType);
  writeOut(ArrayStartByte);
  for (uint8_t i = 0; i < len; i++) {
    writePacketData(data[i]);
    if (i < len - 1) writeOut(ContentSeparatorByte);
  }

  writeOut(ArrayEndByte);
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

void SerialInterface::writePacketStart(uint8_t dataType) {
  writeOut(PacketStartByte);
  writeOut(QuoteByte);
  char buffer[4] = "\0  ";
  leftPad(dataType, buffer, 3, '0');
  writeOut(buffer);
  writeOut(QuoteByte);
  writeOut(TypeSeparatorByte);
}

void SerialInterface::writePacketData(int data) {
  char string[NumberWidth];
  itoa(data, string, 10);
  writeOut(string);
}

void SerialInterface::writePacketData(float data) {
  char string[NumberWidth];
  dtostrf(data, 0, FloatPrecision, string);
  writeOut(string);
}

void SerialInterface::writePacketEnd() {
  writeOut(PacketEndByte);
  writeOut('\n');
}

void SerialInterface::writeOut(char byte) {
  Serial.write(byte);
}

void SerialInterface::writeOut(const char* bytes) {
  Serial.write(bytes);
}

SerialInterface::Packet SerialInterface::readIncomingData() {
  Packet p = {false, 0, {0, 0, 0, 0}};
  while (getAvailable() > 0) {
    uint8_t nextByte = getNextByte();
    if (packetIndex == 0 && nextByte == PacketStartByte) { // Packet start
      memset(incomingPacket, 0, 24);
      packetIndex++;
      packetType = 0;
    } else if (packetIndex == 1 || packetIndex == 5) { // Quote marks around string
      if (nextByte == QuoteByte) packetIndex++; // Is expected character?
      else packetIndex = 0; // If not, reject packet
    } else if (packetIndex >= 2 && packetIndex <= 4) { // Packet type (3-digit number)
      if (nextByte >= 48 && nextByte <= 57) { // If digit, set correct place value
        packetType += pow10(4 - packetIndex) * (nextByte - 48);
        packetIndex++;
      } else packetIndex = 0;
    } else if (packetIndex == 6) { // Type separator
      if (nextByte == TypeSeparatorByte) packetIndex ++;
      else packetIndex = 0;
    } else if (packetIndex == 7) { // Array Start
      if (nextByte == ArrayStartByte) packetIndex ++;
      else packetIndex = 0;
    } else if (packetIndex > 7 && nextByte != PacketEndByte) { // Read packet contents
      incomingPacket[packetIndex - 8] = nextByte;
      packetIndex ++;
    } else if (packetIndex - 7 == sizeof(incomingPacket) || nextByte == PacketEndByte) { // End
      for (uint8_t i = 0, startIndex = 0; i < sizeof(packetContents) / sizeof(float); i++) {
        if (startIndex < packetIndex - 7) {
          char* argString = new char[8];
          memset(argString, 0, 8);
          uint8_t j;
          for (j = startIndex; j < packetIndex - 7; j++) {
             // Break if end of argument or packet detected, else copy the next byte
            if (incomingPacket[j] == ContentSeparatorByte
                || incomingPacket[j] == ArrayEndByte) break;
            else argString[j - startIndex] = incomingPacket[j];
          }
          argString[j] = '\0';
          startIndex = j + 1; // Set start index after each argument
          packetContents[i] = atof((const char *)argString); // Convert to float
        } else { // If end of packet reached, just set remaining args to 0
          packetContents[i] = 0;
        }
      }
      packetIndex = 0;
      p.ready = true;
      p.type = packetType;
      for (uint8_t i = 0; i < MaxContentLength; i++) p.contents[i] = packetContents[i];
      return p;
    }
  }
  return p;
}

int SerialInterface::getAvailable() {
  return Serial.available();
}

uint8_t SerialInterface::getNextByte() {
  return Serial.read();
}

// Fast power of 10 for low numbers
uint8_t SerialInterface::pow10(uint8_t n) {
  switch (n) {
    case 0:
      return 1;
    case 1:
      return 10;
    case 2:
      return 100;
    default:
      return 1;
  }
}
