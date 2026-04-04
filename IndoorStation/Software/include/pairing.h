#ifndef PAIRING_H
#define PAIRING_H

#include <Arduino.h>

extern bool pairing_isAPOpen;
extern String pairing_ApSsid;

bool pairing_startPairingAP();
bool pairing_stopPairingAP();
bool pairing_handlePairingAPTimeout();

#endif