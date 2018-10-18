// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#include <Arduino.h>
#include "constants.hpp"
#include "util.hpp"
#include "serial.hpp"

// Main robot class with methods for initializing and updating subsystems
class Robot {
    public:
        Robot();
        void init();
        void tick();
};
