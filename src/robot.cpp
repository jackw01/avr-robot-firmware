// robot-bridge-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#include "robot.hpp"

Robot::Robot() {
}

// Initializes everything
void Robot::init() {
  leds.init(); // Turn on status lights before anything else
  leds.setAll(ColorOrange);

  SerialInterface::init(SerialBaudRate); // Start serial connection

  // Init subsystems
  imu.init();
  drive.init();
  imu.calibrateGyro();

  // Initialization complete
  leds.setAll(ColorCyan);
  SerialInterface::writePacket(DataTypeHumanReadable, "Initialization complete");
}

// Update function, called in a loop
void Robot::tick() {
  // Check for available data and parse packets
  while (SerialInterface::getAvailable() > 0) {
    if (parseIncomingPackets(SerialInterface::getNextByte())) { // While data is available, process next byte
      if (packetType == CmdTypeSetDriveOpenLoop) {
        drive.setOpenLoopPower(packetContents[0], packetContents[1]);
      } else if (packetType == CmdTypeSetDriveClosedLoop) {
        drive.setVelocitySetpoint({packetContents[0], packetContents[1]});
      } else if (packetType == CmdTypeEnableDriveClosedLoop) {
        drive.setState(DriveStateClosedLoop);
      } else if (packetType == CmdTypeDisableDriveClosedLoop) {
        drive.setState(DriveStateOpenLoop);
      } else if (packetType == CmdTypeSetDrivePIDTuning) {
        drive.setPID(packetContents[0], packetContents[1], packetContents[2]);
      } else if (packetType == CmdTypeSetAllStatusLEDs) {
        leds.setAll(CRGB(packetContents[0], packetContents[1], packetContents[2]));
      } else if (packetType == CmdTypeCalibrateGyro) {
        leds.setAll(ColorOrange);
        imu.calibrateGyro(packetContents[0]);
        leds.setAll(ColorCyan);
      } else if (packetType == CmdTypeResetDrive) {
        drive.reset();
      }
    }
  }

  // Do timing-sensitive things
  microseconds = micros();
  long deltaT = microseconds - lastMicroseconds; // Calculate time delta to rate limit the loop
  if (deltaT > MainControlLoopIntervalUs) {
    lastMicroseconds = microseconds;

    // Update subsystems
    imu.update();
    drive.update();

    // Send gyro data
    vec3 gyroOrientation = imu.getGyroOrientation();
    SerialInterface::writePacket(DataTypeGyro, (float*)&gyroOrientation, 3);
  }

  // Send status/battery voltage over serial
  if (microseconds - lastStatusCheckMicroseconds > SystemStatusCheckIntervalUs) {
    lastStatusCheckMicroseconds = microseconds;

    SerialInterface::writePacket(DataTypeFreeRAM, getFreeRAM()); // Send RAM first

    float voltage = measureBatteryVoltage();
    SerialInterface::writePacket(DataTypeBatteryVoltage, voltage); // Send battery
    if (voltage < BatteryLowCellVoltage * BatteryCellCount && !drive.getMoving()) { // Low battery warning
      SerialInterface::writePacket(DataTypeHumanReadable, "Battery critically low. Shutting down.");
      leds.blinkAll(ColorRed, ColorOff, 0, 500, 500);
    }
  }
}

// Parse packets in incoming data. Returns true if a packet is found.
bool Robot::parseIncomingPackets(uint8_t nextByte) {
  if (packetIndex == 0 && nextByte == PacketStartByte) { // Packet start
    memset(incomingPacket, 0, 24);
    packetIndex++;
    packetType = 0;
  } else if (packetIndex >= 1 && packetIndex <= 3) { // Packet type (3-digit number)
    if (nextByte >= 48 && nextByte <= 57) { // If digit, set correct place value
      packetType += pow10(3 - packetIndex) * (nextByte - 48);
      packetIndex++;
    } else { // If not, reject packet
      SerialInterface::writePacket(DataTypeHumanReadable, "Malformed packet received. Ignoring.");
      packetIndex = 0;
      packetType = 0;
    }
  } else if (packetIndex == 4) { // Packet separator
    if (nextByte == PacketDataSeparatorByte) { // Is expected character?
      packetIndex ++;
    } else { // If not, reject packet
      SerialInterface::writePacket(DataTypeHumanReadable, "Malformed packet received. Ignoring.");
      packetIndex = 0;
      packetType = 0;
    }
  } else if (packetIndex > 4 && nextByte != PacketEndByte) { // Read arguments, stop at packet marker
    incomingPacket[packetIndex - 5] = nextByte;
    packetIndex ++;
  } else if (packetIndex - 5 == sizeof(incomingPacket) || nextByte == PacketEndByte) { // Reached end of packet
    // Parse arguments
    for (uint8_t i = 0, startIndex = 0; i < sizeof(packetContents) / sizeof(float); i++) { // Go through args
      if (startIndex < packetIndex - 5) {
        char* argString = new char[8];
        memset(argString, 0, 8);
        uint8_t j;
        for (j = startIndex; j < packetIndex - 5; j++) { // Iterate through bytes until end of packet
          if (incomingPacket[j] == PacketArgsSeparatorByte) break; // Break if end of argument or packet detected
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

// Measures the battery voltage
float Robot::measureBatteryVoltage() {
  // Take multiple samples and average them to make the results more accurate
  float temp = 0.0;
  uint8_t samples = 5;
  for (uint8_t i = 0; i < samples; i++) {
    temp += (float)analogRead(PinBatteryVoltageDivider) / 1023.0 * ControllerSupplyVoltage * BatteryDividerRatio;
  }
  return temp / (float)samples;
}
