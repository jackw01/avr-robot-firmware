// robot-bridge-firmware
// Copyright (c) 2018 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#pragma once

namespace serial {

  // Left pad number with a character
  static char* leftPad(int n, int size, char padChar) {
    char* string = new char[8];
    itoa(n, string, 10);
    int len = strlen(string);
    if (len >= size) return string;
    char* buffer = new char[18];
    char* padString = new char[2];
    padString[0] = padChar;
    padString[1] = '\0';
    strcpy(buffer, padString);
    for (int i = 0; i < size - len - 1; i++) strcat(buffer, padString);
    strcat(buffer, string);
    return buffer;
  }
}
