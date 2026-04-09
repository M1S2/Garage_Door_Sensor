#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

/**
 * Length of the key used for encryption in ESP-NOW communication. Must be 16 bytes for AES-128 encryption.
 */
#define ESPNOW_KEY_LEN 16

bool utils_parseMacAddress(const String& macStr, uint8_t* mac);
void utils_printMac(const uint8_t* mac);
uint32_t utils_calculateCRC32(const uint8_t* data, size_t length);

/**
 * Check if a key is "empty" (all bytes == 0)
 * @param key Pointer to the key data
 * @param len Length of the key in bytes
 * @return true if the key is empty, false otherwise
 */
bool utils_isKeyEmpty(const uint8_t* key, size_t len);

/**
 * Convert a byte array to a hexadecimal string representation.
 * @param data Pointer to the input byte array
 * @param len Length of the input byte array in bytes
 * @param out Pointer to the output buffer where the hexadecimal string will be stored (must have space for at least len*2 + 1 bytes)
 */
void utils_bytesToHex(const uint8_t* data, size_t len, char* out);

/**
 * Convert a hexadecimal string representation to a byte array.
 * @param hex The input hexadecimal string
 * @param out Pointer to the output byte array (must have space for at least len bytes)
 * @param len Length of the output byte array in bytes
 * @return true if the conversion was successful, false otherwise
 */
bool utils_hexToBytes(const char* hex, uint8_t* out, size_t len);

#endif