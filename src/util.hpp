// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#pragma once

namespace util {

  static uint8_t pow10(uint8_t n) {
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
}

static int getFreeRAM() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
