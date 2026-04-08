#include "utils.h"

uint32_t utils_calculateCRC32(const uint8_t* data, size_t length)
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

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

bool utils_parseMac(const char* str, uint8_t mac[6])
{
    unsigned int tmp[6];

    if (sscanf(str, "%2X:%2X:%2X:%2X:%2X:%2X", &tmp[0], &tmp[1], &tmp[2], &tmp[3], &tmp[4], &tmp[5]) != 6)
    {
        return false;
    }

    for (uint8_t i = 0; i < 6; i++)
    {
        mac[i] = (uint8_t)tmp[i];
    }

    return true;
}