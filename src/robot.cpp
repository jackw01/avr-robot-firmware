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
      // Turn args into floats?

      // Run command

      packetIndex = 0;
      packetType = 0;
    }
  }
}
