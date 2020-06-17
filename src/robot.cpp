// robot-bridge-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#include "robot.hpp"

Robot::Robot() {
}

// Initializes everything
void Robot::init() {
  leds.init(); // Turn on status lights before anything else
  leds.setAll(ColorOrange);

  serial.begin(SerialBaudRate); // Start serial connection

  // Init subsystems
  if (EnableDrive) drive.init();
  if (EnableIMU) {
    imu.init();
    imu.calibrateGyro();
  }

  // Initialization complete
  leds.setAll(ColorCyan);
  serial.writePacket(DataTypeHumanReadable, "Initialization complete");
}

// Update function, called in a loop
void Robot::tick() {
  // Check for available data and parse packets
  SerialInterface::Packet packet;
  while (true) {
    packet = serial.readIncomingData();
    if (packet.ready) { // While data is available, process next byte
      if (packet.type == CmdTypeSetDriveOpenLoop) {
        drive.setOpenLoopPower(packet.contents[0], packet.contents[1]);
      } else if (packet.type == CmdTypeSetDriveClosedLoop) {
        drive.setVelocitySetpoint({packet.contents[0], packet.contents[1]});
      } else if (packet.type == CmdTypeEnableDriveClosedLoop) {
        drive.setState(DriveStateClosedLoop);
      } else if (packet.type == CmdTypeDisableDriveClosedLoop) {
        drive.setState(DriveStateOpenLoop);
      } else if (packet.type == CmdTypeSetDrivePIDTuning) {
        drive.setPID(packet.contents[0], packet.contents[1], packet.contents[2]);
      } else if (packet.type == CmdTypeSetAllStatusLEDs) {
        leds.setAll(CRGB(packet.contents[0], packet.contents[1], packet.contents[2]));
      } else if (packet.type == CmdTypeCalibrateGyro) {
        leds.setAll(ColorOrange);
        imu.calibrateGyro(packet.contents[0]);
        leds.setAll(ColorCyan);
      } else if (packet.type == CmdTypeResetDrive) {
        drive.reset();
      }
    } else break;
  };

  // Do timing-sensitive things
  microseconds = micros();
  long deltaT = microseconds - lastMicroseconds; // Calculate time delta to rate limit the loop
  if (deltaT > MainControlLoopIntervalUs) {
    lastMicroseconds = microseconds;

    // Update subsystems
    if (EnableIMU) {
      imu.update();
      // Send gyro data
      vec3 gyroOrientation = imu.getGyroOrientation();
      serial.writePacket(DataTypeGyro, (float*)&gyroOrientation, 3);
    }

    if (EnableDrive) drive.update();
  }

  // Status check loop runs at a lower speed
  if (microseconds - lastStatusCheckMicroseconds > SystemStatusCheckIntervalUs) {
    lastStatusCheckMicroseconds = microseconds;

    if (EnableDebugMessaging) serial.writePacket(DataTypeFreeRAM, getFreeRAM());

    if (EnableVoltageMonitoring) {
      float voltage = measureBatteryVoltage();
      serial.writePacket(DataTypeBatteryVoltage, voltage);
      if (EnableLowVoltageCutoff &&
          voltage < BatteryLowCellVoltage * BatteryCellCount && !drive.getMoving()) {
        serial.writePacket(DataTypeHumanReadable, "Battery critically low. Shutting down.");
        leds.blinkAll(ColorRed, ColorOff, 0, 500, 500);
      }
    }
  }
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
