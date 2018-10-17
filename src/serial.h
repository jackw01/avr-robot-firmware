// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#ifndef SERIAL_H
#define SERIAL_H

#include <Arduino.h>

static char *pad(int n, int size, char *padChar) {
  char string[8];
  itoa(n, string, 10);
  int len = strlen(string);
  if (len >= size) return string;
  char buffer[1] = "";
  for (int i = 0; i < size - len; i++) strcat(buffer, padChar);
  strcat(buffer, string);
  return buffer;
}

#endif
