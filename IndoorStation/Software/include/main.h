#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "config.h"
#include "structures.h"

/**
 * This struct contains the system configuration. It contains e.g. the MAC addresses, modes and LMKs for all supported sensors.
 */
extern system_config_t sysConfig;

void updateLeds_sensorStatus();
void initWebserverFiles();
void setSensorMode(uint8_t sensorIndex, SensorModes mode);

#endif