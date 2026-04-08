#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

uint32_t utils_calculateCRC32(const uint8_t* data, size_t length);
bool utils_parseMac(const char* str, uint8_t mac[6]);

#endif