#ifndef SENSOR_PAIRING_H
#define SENSOR_PAIRING_H

#include <ESPAsyncWebServer.h>

enum Sensor_Pairing_Modes { PAIRING_MODE_SENSOR1, PAIRING_MODE_SENSOR2, PAIRING_MODE_NONE };
extern Sensor_Pairing_Modes sensorPairingMode;

void sensorPairing_Start(int sensorIndex, AsyncEventSource* events);
void sensorPairing_Stop(AsyncEventSource* events);
void sensorPairing_MoveToNext(AsyncEventSource* events);

void updateWebsiteForSensorPairingStatus(AsyncEventSource* events);

#endif