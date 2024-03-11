#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define BTN_SHOW_STATUS_PIN     12                        // The pin which is used for the show status button
#define BTN_WIFI_PIN            13                        // The pin which is used for the Wifi button
#define BTN_RESET_PIN           16                        // The pin which is used for the reset button

#define SENSOR_PIN_STATE_OPEN   LOW                       // This pin state of the sensor is interpreted as open door
#define NUM_SUPPORTED_SENSORS   2                         // The number of supported sensors

#define CONNECTION_TIMEOUT_MS   10000                     // Timeout in ms for connection to router
#define WIFI_HOSTNAME           "Garagen Tor Sensor"      // Name that is displayed for this device by the router
#define CONFIGURATION_AP_NAME   "Garagen Tor Sensor AP"   // Name for the configuration access point
#define CONFIGURATION_AP_PW     ""                        // Password for the configuration access point

// https://forum.arduino.cc/t/finding-the-size-of-multi-dimensional-array/395465/8
#define ARRAY_ELEMENT_COUNT(array) (sizeof array / sizeof array[0])

#endif