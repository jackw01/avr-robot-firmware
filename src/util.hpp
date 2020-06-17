// robot-bridge-firmware
// Copyright 2020 jackw01. Released under the MIT License (see LICENSE for details).

#pragma once

// Type for 3D vector data - taken from Adafruit Unified Sensor with some fields removed
typedef struct{
  union {
    float v[3];
    struct {
      float x;
      float y;
      float z;
    };
    struct {
      float roll;
      float pitch;
      float heading;
    };
  };
} vec3;

// Get free RAM - found somewhere on Stackoverflow
static int getFreeRAM() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
