#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

/**
 * Length of the key used for encryption in ESP-NOW communication. Must be 16 bytes for AES-128 encryption.
 */
#define ESPNOW_KEY_LEN 16

/**
 * Calculate the CRC32 checksum of the given data.
 * @param data Pointer to the input data
 * @param length Length of the input data in bytes
 * @return The calculated CRC32 checksum
 */
uint32_t utils_calculateCRC32(const uint8_t* data, size_t length);

/**
 * Parse a MAC address string in the format "XX:XX:XX:XX:XX:XX" and convert it to a byte array.
 * @param str The input string containing the MAC address
 * @param mac The output byte array where the parsed MAC address will be stored (must have space for at least 6 bytes)
 * @return true if the parsing was successful, false if the input string was not in the correct format
 */
bool utils_parseMac(const char* str, uint8_t mac[6]);

/**
 * Initialize the random number generator with a seed based on the current CPU cycle count and microseconds.
 * This helps to ensure that the generated random numbers are different each time the device is restarted.
 */
void utils_initRandom();

/**
 * Check if a key is "empty" (all bytes == 0)
 * @param key Pointer to the key data
 * @param len Length of the key in bytes
 * @return true if the key is empty, false otherwise
 */
bool utils_isKeyEmpty(const uint8_t* key, size_t len);

/**
 * Generates a random key of the specified length.
 * @param key Pointer to the buffer where the generated key will be stored
 * @param len Length of the key to generate in bytes
 */
void utils_generateRandomKey(uint8_t* key, size_t len);

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