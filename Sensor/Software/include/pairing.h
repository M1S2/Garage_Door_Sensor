#ifndef PAIRING_H
#define PAIRING_H

#include <Arduino.h>

#define PAIRING_MAGIC_NUMBER    0xCAFEBEEF

typedef struct pairing_info
{
  uint32_t magic = 0;
  uint8_t indoor_station_mac[6] = {0};
  uint32_t crc32 = 0;
}pairing_info_t;

extern pairing_info_t PairingInfo;

bool pairing_runPairing();

bool pairing_savePairingInfo();
bool pairing_loadPairingInfo();
bool pairing_clearPairingInfo();
bool pairing_isPairingInfoValid();

#endif