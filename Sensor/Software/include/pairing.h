#ifndef PAIRING_H
#define PAIRING_H

#include <Arduino.h>
#include "utils.h"

#define PAIRING_MAGIC_NUMBER        0x50414952      // "PAIR" in ASCII. This magic number is used to identify pairing messages from the sensors.

typedef struct pairing_info
{
    uint32_t magic = 0;
    uint8_t indoor_station_mac[6] = {0};
    uint8_t pmk[ESPNOW_KEY_LEN] = {0};
    uint8_t lmk[ESPNOW_KEY_LEN] = {0};
    uint32_t crc32 = 0;
}pairing_info_t;

extern pairing_info_t PairingInfo;
extern uint8_t pairing_last_wifi_channel;
extern uint32_t pairing_token;

bool pairing_runPairing();

bool pairing_savePairingInfo();
bool pairing_loadPairingInfo();
bool pairing_clearPairingInfo();
bool pairing_isPairingInfoValid();

#endif