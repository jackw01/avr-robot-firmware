// robot-bridge-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#include "robot.hpp"

// Get free RAM - found somewhere on Stackoverflow
static int getFreeRAM() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

Robot::Robot() {
}

// Initializes everything
void Robot::init() {
  led.init();
  led.setState(true);
  serial.begin(SerialBaudRate); // Start serial connection

  // Init subsystems
  if (EnableGPIO) {
    for (uint8_t i = 0; i < sizeof(ExtraGPIOPins); i++) pinMode(ExtraGPIOPins[i], INPUT);
  }
  if (EnableIMU) {
    imu.init();
    imu.calibrateGyro();
  }
  if (EnableDrive) drive.init();

  // Initialization complete
  led.setState(false);
  serial.writePacket(DataTypeHumanReadable, "Initialization complete");
}

// Update function, called in a loop
void Robot::tick() {
  // Check for available data and parse packets
  SerialInterface::Packet packet;
  while (true) { // Loop forever for now, but quit if nothing is ready
    packet = serial.readIncomingData();
    if (packet.ready) { // While data is available, process next byte
      if (EnableIMU) {
        if (packet.type == CmdTypeCalibrateGyro) {
          led.setState(true);
          imu.calibrateGyro(packet.contents[0]);
          led.setState(false);
        }
      }

      if (EnableGPIO) {
        if (packet.type == CmdTypeGPIOModeSet) {
          pinMode(packet.contents[0], packet.contents[1]); // Cast float to bool, 1 for output
        } else if (packet.type == CmdTypeGPIOStateSet) {
          digitalWrite(packet.contents[0], packet.contents[1]);
        } else if (packet.type == CmdTypeGPIOStateGet) {
          serial.writePacket(DataTypeGPIOState, (int16_t)digitalRead(packet.contents[0]));
        } else if (packet.type == CmdTypeGPIOPWMSet) {
          analogWrite(packet.contents[0], packet.contents[1]);
        } else if (packet.type == CmdTypeGPIOAnalogRead) {
          serial.writePacket(DataTypeGPIOState, analogRead(packet.contents[0]));
        }
      }

      if (EnableDrive) {
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
        } else if (packet.type == CmdTypeResetDrive) {
          drive.reset();
        }
      }

      if (EnableSerialEcho) serial.writePacket(packet.type, packet.contents, 4);
    } else break;
  };

  // Do timing-sensitive things
  microseconds = micros();
  uint32_t deltaT = microseconds - lastMicroseconds; // Rate limit the loop
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

    led.update(microseconds);
  }

  // Status check loop runs at a lower speed
  if (microseconds - lastStatusCheckMicroseconds > SystemStatusCheckIntervalUs) {
    lastStatusCheckMicroseconds = microseconds;
    led.blink(100, 100);

    if (EnableDebugMessaging) serial.writePacket(DataTypeFreeRAM, getFreeRAM());

    if (EnableVoltageMonitoring) {
      float voltage = measureBatteryVoltage();
      serial.writePacket(DataTypeBatteryVoltage, voltage);
      if (EnableLowVoltageCutoff &&
          voltage < BatteryLowCellVoltage * BatteryCellCount && !drive.getMoving()) {
        serial.writePacket(DataTypeHumanReadable, "Battery critically low. Shutting down.");
        led.setState(true);
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
