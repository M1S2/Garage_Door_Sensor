#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <Arduino.h>
#include "utils.h"

// Packing of this structure reduces the size to 4 bytes
typedef struct __attribute__((packed)) message_sensor
{
    bool pinState;                  // This field contains the state of the sensor (door open / closed)
    uint16_t batteryVoltage_mV;     // This field contains the measured battery voltage in mV
    uint8_t numberSendLoops;        // This field contains the total number of loops needed to send the message to the indoor station
    uint8_t sensor_sw_version;      // This field contains the software version of the sensor in the following format: Upper 4 bits contain the Major number, lower 4 bits contain the Minor number (each number can be in the range from 0..15)
}message_sensor_t;

typedef struct __attribute__((packed)) message_sensor_timestamped
{
    message_sensor_t msg;
    time_t timestamp;
}message_sensor_timestamped_t;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

enum SensorModes
{
    SENSOR_MODE_NORMAL = 0,           // All messages are received, everything is saved, LED on
    SENSOR_MODE_DISABLED = 1,         // All messages are ignored, nothing is saved, LED off
    SENSOR_MODE_CHARGING = 2,         // All messages are ignored, nothing is saved, LED in different color
    SENSOR_MODE_ONLY_DISPLAY = 3,     // Messages are only displayed via the LED, nothing is saved
    SENSOR_MODE_PAIRING = 4,          // Message from Indoor Station to corresponding sensor to configure the MAC address in the sensor, LED flashes red and blue, nothing is saved
};

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

typedef struct sensor_config
{
    char name[64] = {0};
    uint8_t mac[6] = {0};
    SensorModes mode = SENSOR_MODE_NORMAL;
    uint8_t lmk[ESPNOW_KEY_LEN] = {0};
    bool isPaired = false;
    bool useEncryption = false;
} sensor_config_t;

typedef struct system_config
{
    sensor_config_t sensors[NUM_SUPPORTED_SENSORS];
    uint8_t pmk[ESPNOW_KEY_LEN] = {0};
} system_config_t;

#define MEMORY_PERSISTED_SYSTEM_CONFIG_MAGIC   0x53434647UL  // "SCFG"

typedef struct persisted_system_config
{
    uint32_t magic = MEMORY_PERSISTED_SYSTEM_CONFIG_MAGIC;
    system_config_t system_config;
    uint32_t crc32 = 0;
} persisted_system_config_t;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define PAIRING_MAGIC_NUMBER                0x50414952      // "PAIR" in ASCII. This magic number is used to identify pairing messages from the sensors.
typedef struct __attribute__((packed)) message_pairing
{
    uint32_t pairingMagicNumber;        // This field contains a fixed number. Only if this number is found, the MAC of the received message is saved by the Indoor Station.
    uint32_t pairingToken;              // This field contains a token for pairing validation. The indoor station generates a random token for each pairing session. The sensor must include this token in the pairing message. The indoor station only accepts the pairing message if the token is correct. This prevents that an attacker can send a pairing message with the correct magic number and a random MAC address to pair with the indoor station without knowing the token.
    uint8_t sensorMac[6] = {0};         // This field contains the MAC address of the sensor which wants to pair. The indoor station saves this MAC address in the system config to identify the sensor in the future and to display the correct MAC address in the web interface.
}message_pairing_t;

#endif