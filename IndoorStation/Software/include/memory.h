#ifndef MEMORY_H
#define MEMORY_H

#include <Arduino.h>
#include "config.h"
#include "structures.h"

#define FILENAME_HISTORY_SENSOR_FORMAT		"/dataSensor%d.bin"
#define FILENAME_PERSISTED_SYSTEM_CONFIG    "/system_config.bin"

/**
 * Delete all saved data files (sensor history, sensor MACs).
 */
void memory_removeAllData();

/**
 * Delete the sensor history data for the requested sensor.
 * @param sensorIndex Index of the sensor for which the data file is deleted. Use -1 to delete all sensor history data files.
 */
void memory_removeSensorHistory(int8_t sensorIndex);

/**
 * Construct a string that describes how much memory of the LittleFS is used.
 * @return String describing the memory usage (used bytes, total bytes, percentage).
 */
const char* memory_getMemoryUsageString();

/**
 * Read all sensor history files and show the decoded content on the Serial output.
 */
void memory_showMemoryContent();

/**
 * Get the number of available sensor messages for the requested sensor.
 * @param sensorIndex Index of the sensor, for which the number of messages is returned. If lager than NUM_SUPPORTED_SENSORS it is limited to this value.
 * @return Number of sensor messages for the requested sensor.
 */
uint16_t memory_getNumberSensorMessages(uint8_t sensorIndex);

/**
 * Get all available sensor messages for the requested sensor.
 * @param sensorIndex Index of the sensor, for which the messages are returned. If lager than NUM_SUPPORTED_SENSORS it is limited to this value.
 * @param sensorMessagesBuffer Pointer to the buffer, to which the sensor messages are returned. Make sure, the array is large enough (minimum the number of message returned by memory_getNumberSensorMessages) !!!
 */
void memory_getSensorMessagesForSensor(uint8_t sensorIndex, message_sensor_timestamped_t* sensorMessagesBuffer);

/**
 * Get the latest available sensor message for the requested sensor.
 * @param sensorIndex Index of the sensor, for which the last message is returned. If lager than NUM_SUPPORTED_SENSORS it is limited to this value.
 * @return Last sensor message for the requested sensor. If no message is available, a message with timestamp -1 is returned.
 */
message_sensor_timestamped_t memory_getLatestSensorMessagesForSensor(uint8_t sensorIndex);

/**
 * Add the given message to the history file for the requested sensor.
 * @param sensorIndex Index of the sensor, for which the sensor message is saved. If lager than NUM_SUPPORTED_SENSORS it is limited to this value.
 * @param sensorMessage Sensor messages that is saved.
 * @return True if message was added; otherwise false
 */
bool memory_addSensorMessage(uint8_t sensorIndex, message_sensor_timestamped_t sensorMessage);

/**
 * Save the system config (MAC addresses, modes and LMKs for all supported sensors) to the LittleFS.
 * The system config is used to save the configuration across device restarts.
 * @param sysConfig The system config struct, which is saved.
 * @return True if the system config was successfully saved; otherwise false.
 */
bool memory_saveSystemConfig(system_config_t& sysConfig);

/**
 * Load the system config (MAC addresses, modes and LMKs for all supported sensors) from the LittleFS.
 * The system config is used to save the configuration across device restarts.
 * @param sysConfig The system config struct, which is loaded.
 * @return True if the system config was successfully loaded and is valid; otherwise false.
 */
bool memory_loadSystemConfig(system_config_t& sysConfig);

#endif