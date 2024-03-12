#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "config.h"
#include "structures.h"

void updateLeds_sensorStatus();
void initWebserverFiles();
void updateWebsiteMainForSensor(uint8_t sensor_id, message_sensor_timestamped_t sensor_message_timestamped);
void updateWebsiteMain();
void updateWebsiteSensorHistory();

#endif