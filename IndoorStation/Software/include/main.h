#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "config.h"
#include "structures.h"

/**
 * This struct contains the system configuration. It contains e.g. the MAC addresses, modes and LMKs for all supported sensors.
 */
extern system_config_t sysConfig;

/**
 * Update the status leds for all sensors according to the latest received messages and the current mode of each sensor.
 */
void main_updateLeds_sensorStatus();

/**
 * Initialize the webserver endpoints for the indoor station.
 */
void main_initWebserverEndpoints();

/**
 * Set the mode for the given sensor index and save it in memory.
 * If the mode is set to pairing mode, also start the pairing AP. If the mode is changed from pairing mode to another mode, also stop the pairing AP.
 * @param sensorIndex The index of the sensor for which the mode should be set.
 * @param mode The mode to set for the sensor.
 */
void main_setSensorMode(uint8_t sensorIndex, SensorModes mode);

#endif