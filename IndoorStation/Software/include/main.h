#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "config.h"
#include "structures.h"

extern SensorModes sensor_modes[NUM_SUPPORTED_SENSORS];

void updateLeds_sensorStatus();
void initWebserverFiles();
void setSensorMode(uint8_t sensorIndex, SensorModes mode);

#endif