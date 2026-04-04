#include "utils.h"

bool parseMacAddress(const String& macStr, uint8_t* mac)
{
    int values[6];

    if (sscanf(macStr.c_str(), "%x:%x:%x:%x:%x:%x",
                &values[0], &values[1], &values[2],
                &values[3], &values[4], &values[5]) != 6)
    {
        return false;
    }

    for (int i = 0; i < 6; i++)
    {
        mac[i] = (uint8_t)values[i];
    }

    return true;
}

/**********************************************************************/

void printMac(const uint8_t* mac)
{
    for (int i = 0; i < 6; i++)
    {
        if (mac[i] < 16) Serial.print("0");
        Serial.print(mac[i], HEX);
        if (i < 5) Serial.print(":");
    }
}

/**********************************************************************/

uint32_t calculateCRC32(const uint8_t* data, size_t length)
{
  uint32_t crc = 0xFFFFFFFF;

  while (length--)
  {
    uint8_t c = *data++;
    for (uint8_t i = 0; i < 8; i++)
    {
      bool bit = crc & 0x80000000;
      if (c & 0x80)
        bit = !bit;

      crc <<= 1;
      c <<= 1;

      if (bit)
        crc ^= 0x04C11DB7;
    }
  }

  return crc;
}