#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define MCU_LATCH_PIN       12      // GPIO 12
#define DOOR_SWITCH_PIN     14      // GPIO 14
#define MAX_SEND_RETRIES    10      // Number of send retries for each channel before changing to the next channel
#define MAX_WIFI_CHANNELS   13      // 13 in Europe, 11 for North America (adapt the following list accordingly)
uint8_t const wifi_channel_order[] = { 1, 6, 11, 2, 3, 4, 5, 7, 8, 9, 10, 12, 13};      // First try to use the channels 1, 6 and 11 to send (these are the most assigned ones by routers)

#define ADC_RESISTOR_R8     390.0f  // Resistor value in kOhm, from the battery voltage to the ADC input
#define ADC_RESISTOR_R9     100.0f  // Resistor value in kOhm, from the ADC input to GND

#define PAIRING_AP_NAME_BASE            "GaragenTorPairing-"        // Base name for the pairing access point (should be the same as in the indoor station software to find the correct AP)
#define PAIRING_AP_PW                   ""                          // Password for the pairing access point (should be the same as in the indoor station software)
#define PAIRING_CONNECT_TIMEOUT_MS      15000                       // Timeout for connecting to the pairing access point in milliseconds

//#define DEBUG_OUTPUT                                      // enable this define to print debugging output on the serial. If this is disabled, no serial output is used at all (to save power)

#endif