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
  while (Comms::getAvailable() > 0) {
    uint8_t nextByte = Comms::nextByte(); // While data is available, process next byte

    if (packetIndex == 0 && nextByte == PacketMarkerByte) { // Packet start
      packetIndex++;
    } else if (packetIndex >= 1 && packetIndex <= 3) { // Packet type (3-digit number)
      if (nextByte >= 48 && nextByte <= 57) { // If digit, set correct place value
        packetType += (uint8_t)pow(10, 3 - packetIndex) * (nextByte - 48);
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
      float args[4];
      for (uint8_t i = 0, startIndex = 0; i < sizeof(args) / sizeof(float); i++) { // Iterate through possible args
        if (startIndex < packetIndex - 5) {
          char* argString = new char[8];
          uint8_t j;
          for (j = startIndex; j < packetIndex - 5; j++) { // Iterate through bytes until end of packet
            if (incomingPacket[j] == PacketSeparatorByte) break; // Break if end of argument or packet detected
            else argString[j - startIndex] = incomingPacket[j]; // Copy the next byte
          }
          argString[j] = '\0';
          startIndex = j + 1; // Set start index after each argument
          args[i] = atof((const char *)argString); // Convert to float
        } else { // If end of packet reached, just set remaining args to 0
          args[i] = 0;
        }
      }

      // Run command
      Serial.println("a");
      Serial.println(args[0]);
      Serial.println(args[1]);
      Serial.println(args[2]);
      Serial.println(args[3]);

      packetIndex = 0;
      packetType = 0;
    }
  }
}
