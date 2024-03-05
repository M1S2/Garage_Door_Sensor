#ifndef MEMORY_H
#define MEMORY_H

#include <Arduino.h>
#include "config.h"
#include "structures.h"

#define FILENAME_HISTORY    "/historyData.txt"

typedef struct __attribute__((packed)) eeprom_general_settings
{
	uint8_t Placeholder_for_any_general_settings_persisted_to_eeprom;			// change this...
}eeprom_general_settings_t;

typedef struct eeprom_content
{
	eeprom_general_settings_t general_settings;
	uint16_t sensor_messages_nextElement_Indices[NUM_SUPPORTED_SENSORS];
    message_sensor_timestamped_t sensor_messages[NUM_SUPPORTED_SENSORS][25];
//	message_sensor_timestamped_t sensor_messages[NUM_SUPPORTED_SENSORS][(4095 - sizeof(eeprom_general_settings_t) - sizeof(sensor_messages_nextElement_Indices)) / NUM_SUPPORTED_SENSORS];
}eeprom_content_t;

extern eeprom_content_t eeprom_content_buffered;

void memory_initBufferedContent();
void memory_persistBufferedContent();
void memory_reset();
void memory_showMemoryContent();

uint16_t memory_getNumberSensorMessages(uint8_t sensorIndex);
message_sensor_timestamped_t* memory_getSensorMessagesForSensor(uint8_t sensorIndex);
message_sensor_timestamped_t memory_getLatestSensorMessagesForSensor(uint8_t sensorIndex);
eeprom_general_settings_t memory_getGeneralSettings();

void memory_addSensorMessage(uint8_t sensorIndex, message_sensor_timestamped_t sensorMessage);
void memory_setGeneralSettings(eeprom_general_settings_t generalSettings);

#endif