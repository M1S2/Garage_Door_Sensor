#ifndef PAIRING_H
#define PAIRING_H

#include <Arduino.h>

extern bool pairing_isAPOpen;
extern String pairing_ApSsid;
extern uint32_t pairing_currentToken;

bool pairing_startPairingAP();
bool pairing_stopPairingAP();
bool pairing_handlePairingAPTimeout();

int pairing_findSensorIndexInPairingMode();
void pairing_enablePairingModeForSensor(int sensorIndex);
void pairing_disablePairingModeForSensor(int sensorIndex);
void pairing_stopAllSensorsPairingMode();

#endif