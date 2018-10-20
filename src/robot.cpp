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
      if (nextByte >= 48 && nextByte <= 57) { // Is digit?
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
      uint8_t i = 0, lastIndex = 0;

      while (i < sizeof(args) && lastIndex < packetIndex - 5) {
        char* argString[8];
        uint8_t j;
        for (j = lastIndex; j < packetIndex - 5; j++) {
          if (incomingPacket[j] == PacketSeparatorByte) break; // Separator between args
          else argString[j - lastIndex] = incomingPacket[j];
        }
        argString[j] = '\0';
        lastIndex = j + 1;
        args[i] = atof((const char *)&argString);
        i++;
      }

      // Run command
      Serial.println(args[0]);

      packetIndex = 0;
      packetType = 0;
    }
  }
}
