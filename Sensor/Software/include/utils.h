#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

bool parseMacAddress(const String& macStr, uint8_t* mac);
void printMac(const uint8_t* mac);
uint32_t calculateCRC32(const uint8_t* data, size_t length);

#endif