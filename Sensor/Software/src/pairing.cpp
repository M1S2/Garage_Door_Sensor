#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include "pairing.h"
#include "utils.h"
#include "config.h"

pairing_info_t PairingInfo;

String pairing_findPairingAP()
{
    #ifdef DEBUG_OUTPUT
        Serial.println("[Pairing] Scan for Pairing-AP...");
    #endif

    int networkCount = WiFi.scanNetworks();
    if (networkCount <= 0)
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[Pairing] No WiFi networks found.");
        #endif
        return "";
    }

    String bestMatch = "";
    int bestRssi = -9999;
    for (int i = 0; i < networkCount; i++)
    {
        String ssid = WiFi.SSID(i);
        int32_t rssi = WiFi.RSSI(i);

        #ifdef DEBUG_OUTPUT
            uint8_t channel = WiFi.channel(i);
            Serial.print("[Pairing] Found: ");
            Serial.print(ssid);
            Serial.print(" | RSSI: ");
            Serial.print(rssi);
            Serial.print(" | Channel: ");
            Serial.println(channel);
        #endif

        if (ssid.startsWith(PAIRING_AP_NAME_BASE))
        {
            if (rssi > bestRssi)
            {
                bestRssi = rssi;
                bestMatch = ssid;
            }
        }
    }

    #ifdef DEBUG_OUTPUT
        if (bestMatch.length() > 0)
        {
            Serial.print("[Pairing] Pairing-AP choosen: ");
            Serial.println(bestMatch);
        }
        else
        {
            Serial.println("[Pairing] No matching Pairing-AP found.");
        }
    #endif
    return bestMatch;
}

/**********************************************************************/

bool pairing_connectToPairingAP(const String& ssid)
{
    #ifdef DEBUG_OUTPUT
        Serial.print("[Pairing] Connect with AP: ");
        Serial.println(ssid);
    #endif

    if (PAIRING_AP_PW == nullptr || strlen(PAIRING_AP_PW) == 0)
    {
        WiFi.begin(ssid.c_str());
    }
    else
    {
        WiFi.begin(ssid.c_str(), PAIRING_AP_PW);
    }

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < PAIRING_CONNECT_TIMEOUT_MS)
    {
        delay(250);
        #ifdef DEBUG_OUTPUT
            Serial.print(".");
        #endif
    }

    #ifdef DEBUG_OUTPUT
        Serial.println();
    #endif

    if (WiFi.status() == WL_CONNECTED)
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[Pairing] Connected with Pairing-AP.");
            Serial.print("[Pairing] My IP: ");
            Serial.println(WiFi.localIP());
            Serial.print("[Pairing] Gateway IP: ");
            Serial.println(WiFi.gatewayIP());
        #endif
        return true;
    }

    #ifdef DEBUG_OUTPUT
        Serial.println("[Pairing] Paring with Pairing-AP failed.");
    #endif
    return false;
}

/**********************************************************************/

bool pairing_fetchPairingInfo(pairing_info_t& pairingInfo)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[Pairing] Not connected to a WiFi network.");
        #endif
        return false;
    }

    WiFiClient client;
    HTTPClient http;

    IPAddress gatewayIp = WiFi.gatewayIP();
    String url = "http://" + gatewayIp.toString() + "/get_pairing_info";

    #ifdef DEBUG_OUTPUT
        Serial.print("[Pairing] Call Endpoint: ");
        Serial.println(url);
    #endif

    if (!http.begin(client, url))
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[Pairing] HTTP begin() failed.");
        #endif
        return false;
    }

    int httpCode = http.GET();
    if (httpCode <= 0)
    {
        #ifdef DEBUG_OUTPUT
            Serial.print("[Pairing] HTTP Error: ");
            Serial.println(http.errorToString(httpCode));
        #endif
        http.end();
        return false;
    }

    #ifdef DEBUG_OUTPUT 
        Serial.print("[Pairing] HTTP Status: ");
        Serial.println(httpCode);
    #endif
    if (httpCode != HTTP_CODE_OK)
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[Pairing] Unexpected HTTP Status.");
        #endif
        http.end();
        return false;
    }

    String payload = http.getString();
    http.end();

    #ifdef DEBUG_OUTPUT
        Serial.print("[Pairing] answer:");
        Serial.println(payload);
    #endif
    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, payload);
    if (error)
    {
        #ifdef DEBUG_OUTPUT
            Serial.print("[Pairing] JSON Error: ");
            Serial.println(error.c_str());
        #endif
        return false;
    }

    if (!doc.containsKey("indoor_station_mac"))
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[Pairing] JSON doesn't contain all required fields.");
        #endif
        return false;
    }

    String macStr = doc["indoor_station_mac"].as<String>();
    if (!parseMacAddress(macStr, pairingInfo.indoor_station_mac))
    {
        #ifdef DEBUG_OUTPUT
            Serial.print("[Pairing] Invalid MAC format: ");
            Serial.println(macStr);
        #endif
        return false;
    }

    pairingInfo.magic = PAIRING_MAGIC_NUMBER;

    #ifdef DEBUG_OUTPUT
        Serial.println("[Pairing] Pairing-Info successfully read:");
        Serial.println("[Pairing] Server MAC: ");
        printMac(pairingInfo.indoor_station_mac);
    #endif
    return true;
}

/**********************************************************************/

bool pairing_runPairing()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    String pairingSsid = pairing_findPairingAP();
    if (pairingSsid.length() == 0)
    {
        return false;
    }
    if (!pairing_connectToPairingAP(pairingSsid))
    {
        return false;
    }
    if (!pairing_fetchPairingInfo(PairingInfo))
    {
        return false;
    }
    return true;
}

/**********************************************************************/

uint32_t pairing_calculatePairingInfoCRC(const pairing_info_t& info)
{
    return calculateCRC32(reinterpret_cast<const uint8_t*>(&info), sizeof(pairing_info_t) - sizeof(info.crc32));
}

/**********************************************************************/

bool pairing_savePairingInfo()
{
    PairingInfo.crc32 = pairing_calculatePairingInfoCRC(PairingInfo);

    EEPROM.begin(sizeof(pairing_info_t));
    EEPROM.put(0, PairingInfo);

    bool success = EEPROM.commit();
    EEPROM.end();

    #ifdef DEBUG_OUTPUT
        Serial.println("[Pairing] Saved pairing info");
    #endif

    return success;
}

/**********************************************************************/

bool pairing_loadPairingInfo()
{
    EEPROM.begin(sizeof(pairing_info_t));
    EEPROM.get(0, PairingInfo);
    EEPROM.end();

    #ifdef DEBUG_OUTPUT
        Serial.println("[Pairing] Loaded pairing info");
    #endif

    return pairing_isPairingInfoValid();
}

/**********************************************************************/

bool pairing_clearPairingInfo()
{
    pairing_info_t emptyInfo;

    EEPROM.begin(sizeof(pairing_info_t));
    EEPROM.put(0, emptyInfo);

    bool success = EEPROM.commit();
    EEPROM.end();

    #ifdef DEBUG_OUTPUT
        Serial.println("[Pairing] Cleared pairing info.");
    #endif

    return success;
}

/**********************************************************************/

bool pairing_isPairingInfoValid()
{
    if (PairingInfo.magic != PAIRING_MAGIC_NUMBER)
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[Pairing] Pairing info invalid: Magic number mismatch.");
        #endif
        return false;
    }

    uint32_t expectedCRC = pairing_calculatePairingInfoCRC(PairingInfo);
    if (PairingInfo.crc32 != expectedCRC)
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[Pairing] Pairing info invalid: CRC mismatch.");
        #endif
        return false;
    }

    #ifdef DEBUG_OUTPUT
        Serial.println("[Pairing] Pairing info valid");
    #endif
    return true;
}