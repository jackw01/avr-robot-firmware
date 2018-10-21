// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include "robot.hpp"

Robot::Robot() {
}

// Initializes everything
void Robot::init() {
  // Start serial connection
  Comms::init();
  Comms::writePacket(255, "abc");
}

// Update function, called in a loop
void Robot::tick() {
  lastMicroseconds = microseconds;
  microseconds = micros();

  // Check for available data and parse packets
  while (Comms::getAvailable() > 0) {
    if (parseIncomingPackets(Comms::getNextByte())) { // While data is available, process next byte
      Serial.println();
      Serial.println(packetType);
      Serial.println(packetContents[0], 4);
      Serial.println(packetContents[1], 4);
      Serial.println(packetContents[2], 4);
      Serial.println(packetContents[3], 4);
    }
  }
}

// Parse packets in incoming data. Returns true if a packet is found.
bool Robot::parseIncomingPackets(uint8_t nextByte) {
  Serial.write(nextByte);
  if (packetIndex == 0 && nextByte == PacketMarkerByte) { // Packet start
    memset(incomingPacket, 0, 24);
    packetIndex++;
    packetType = 0;
  } else if (packetIndex >= 1 && packetIndex <= 3) { // Packet type (3-digit number)
    if (nextByte >= 48 && nextByte <= 57) { // If digit, set correct place value
      packetType += util::pow10(3 - packetIndex) * (nextByte - 48);
      packetIndex++;
    } else { // If not, reject packet
      packetIndex = 0;
      packetType = 0;
    }
  } else if (packetIndex == 4) { // Packet separator
    if (nextByte == PacketSeparatorByte) { // Is expected character?
      packetIndex ++;
    } else { // If not, reject packet
      packetIndex = 0;
      packetType = 0;
    }
  } else if (packetIndex > 4 && nextByte != PacketMarkerByte) { // Read arguments, stop at packet marker
    incomingPacket[packetIndex - 5] = nextByte;
    packetIndex ++;
  } else if (packetIndex - 5 == sizeof(incomingPacket) || nextByte == PacketMarkerByte) { // Reached end of packet
    // Parse arguments
    for (uint8_t i = 0, startIndex = 0; i < sizeof(packetContents) / sizeof(float); i++) { // Go through args
      if (startIndex < packetIndex - 5) {
        char* argString = new char[8];
        memset(argString, 0, 8);
        uint8_t j;
        for (j = startIndex; j < packetIndex - 5; j++) { // Iterate through bytes until end of packet
          if (incomingPacket[j] == PacketSeparatorByte) break; // Break if end of argument or packet detected
          else argString[j - startIndex] = incomingPacket[j]; // Copy the next byte
        }
        argString[j] = '\0';
        startIndex = j + 1; // Set start index after each argument
        packetContents[i] = atof((const char *)argString); // Convert to float
      } else { // If end of packet reached, just set remaining args to 0
        packetContents[i] = 0;
      }
    }
    packetIndex = 0;
    return true;
  }
  return false;
}
