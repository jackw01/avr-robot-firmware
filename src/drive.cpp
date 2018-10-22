// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include "drive.hpp"

Drive::Drive() {

}

// Calibrate gyro
void Drive::calibrateGyro() {
    for (int i = 0; i < 100; i++) { // Take a bunch of samples and average it out.
        sensors_event_t event;
        gyro.getEvent(&event);
        gyroDriftX += event.gyro.x * 0.025;
        gyroDriftY += event.gyro.y * 0.025;
        gyroDriftZ += event.gyro.z * 0.025;
        delay(25);
    }
    gyroDriftX /= 100;
    gyroDriftY /= 100;
    gyroDriftZ /= 100;
}


// Set power of motors in open loop mode
void Drive::setOpenLoopPower(float leftPower, float rightPower) {
  moving = !(leftPower == 0 && rightPower == 0);
  leftMotor.setSpeed(leftPower);
  rightMotor.setSpeed(rightPower);
}

// Get whether robot is moving
bool Drive::getMoving() {
    return moving;
}
