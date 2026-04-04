#include <ESP8266WiFi.h>
#include "pairing.h"
#include "config.h"

bool pairing_isAPOpen = false;
String pairing_ApSsid;

unsigned long pairing_APStartedAt = 0;

bool pairing_startPairingAP()
{
    if (pairing_isAPOpen)
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[PairingAP] AP is already active.");
        #endif
        return true;
    }

    pairing_ApSsid = PAIRING_AP_NAME_BASE + String(ESP.getChipId(), HEX);

    #ifdef DEBUG_OUTPUT
        Serial.println();
        Serial.println("[PairingAP] Start Pairing Access Point...");
    #endif

    // Make sure, AP is supported
    WiFi.mode(WIFI_AP_STA);

    bool apStarted = false;
    if (PAIRING_AP_PW == nullptr || strlen(PAIRING_AP_PW) == 0)
    {
        apStarted = WiFi.softAP(pairing_ApSsid.c_str());
    }
    else
    {
        apStarted = WiFi.softAP(pairing_ApSsid.c_str(), PAIRING_AP_PW);
    }

    if (!apStarted)
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[PairingAP] ERROR: SoftAP couldn't be started.");
        #endif
        return false;
    }

    pairing_isAPOpen = true;
    pairing_APStartedAt = millis();

    #ifdef DEBUG_OUTPUT
        delay(100);    // delay a bit to ensure that the AP is fully started before printing the info
        Serial.println("[PairingAP] SoftAP successfully started.");
        Serial.print("[PairingAP] SSID: ");
        Serial.println(pairing_ApSsid);

        Serial.print("[PairingAP] AP IP: ");
        Serial.println(WiFi.softAPIP());

        Serial.print("[PairingAP] AP MAC: ");
        Serial.println(WiFi.softAPmacAddress());

        Serial.print("[PairingAP] STA IP: ");
        Serial.println(WiFi.localIP());

        Serial.print("[PairingAP] STA connected: ");
        Serial.println(WiFi.status() == WL_CONNECTED ? "YES" : "NO");

        Serial.print("[PairingAP] WiFi Channel: ");
        Serial.println(WiFi.channel());
    #endif

    return true;
}

/**********************************************************************/

bool pairing_stopPairingAP()
{
    if (!pairing_isAPOpen)
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[PairingAP] AP isn't active.");
        #endif
        return true;
    }

    #ifdef DEBUG_OUTPUT
        Serial.println();
        Serial.println("[PairingAP] Stop Pairing Access Point...");
    #endif

    bool result = WiFi.softAPdisconnect(true); // true = AP off + disconnect clients
    if (!result)
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[PairingAP] WARNING: softAPdisconnect() returns false.");
        #endif
    }

    pairing_isAPOpen = false;
    pairing_APStartedAt = 0;

    #ifdef DEBUG_OUTPUT
        Serial.println("[PairingAP] SoftAP stopped.");
    #endif

    return true;
}

/**********************************************************************/

bool pairing_handlePairingAPTimeout()
{
    if (pairing_isAPOpen && PAIRING_AP_TIMEOUT_MS > 0 && (millis() - pairing_APStartedAt >= PAIRING_AP_TIMEOUT_MS))
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[PairingAP] Timeout reached, AP is automatically stopped.");
        #endif
        pairing_stopPairingAP();
        return true;    // return true if AP was stopped due to timeout
    }
    return false;
}