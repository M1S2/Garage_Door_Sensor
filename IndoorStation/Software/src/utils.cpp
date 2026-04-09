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

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void utils_initRandom()
{
    randomSeed(ESP.getCycleCount() ^ micros());
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

bool utils_isKeyEmpty(const uint8_t* key, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        if (key[i] != 0x00)
        {
            return false;
        }
    }
    return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void utils_generateRandomKey(uint8_t* key, size_t len)
{
    do
    {
        for (size_t i = 0; i < len; i++)
        {
            key[i] = (uint8_t)random(0, 256);
        }
    }
    while (utils_isKeyEmpty(key, len)); // prevent all "0" keys, which are not valid for encryption and also used as "empty" key value in the code
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void utils_bytesToHex(const uint8_t* data, size_t len, char* out)
{
    for (size_t i = 0; i < len; i++)
    {
        sprintf(&out[i * 2], "%02X", data[i]);
    }
    out[len * 2] = '\0';
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

bool utils_hexToBytes(const char* hex, uint8_t* out, size_t len)
{
    if (strlen(hex) != len * 2) return false;

    for (size_t i = 0; i < len; i++)
    {
        sscanf(&hex[i * 2], "%2hhX", &out[i]);
    }
    return true;
}