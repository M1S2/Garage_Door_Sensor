#include <Schedule.h>
#include <ESPAsyncWiFiManager.h>
#include "wifiHandling.h"
#include "leds.h"
#include "otaUpdate.h"
#include "config.h"
#include "main.h"

AsyncWebServer server(80);

DNSServer dns;
AsyncWiFiManager wifiManager(&server, &dns);

bool wifiConfig_isAPOpen;

// The event handlers are initialized in the setup()
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

void wifiHandling_eraseCredentials() 
{
    leds_wifiFailed();
    WiFi.disconnect(true);
    ESP.eraseConfig();
    #ifdef DEBUG_OUTPUT
        Serial.println("WiFi credentials erased");
    #endif
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) 
{
  // https://github.com/esp8266/Arduino/issues/5722
  // The wifi callbacks execute in the SYS context, and you can't yield/delay in there.
  // If you need to do that, I suggest to use the wifi callback to schedule another callback with your code that requires yield/delay.
  // Scheduled functions execute as though they were called from the loop, I.e. in CONT context.
  // Same applies to the onWifiDisconnect event
  schedule_function(leds_wifiConnected);
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) 
{
  // This function gets called cyclic as long as the Wifi is disconnected
  if(!wifiConfig_isAPOpen)
  {
    schedule_function(leds_wifiFailed);
  }
}

void wifiHandling_wifiManagerSaveCB()
{
    wifiConfig_isAPOpen = false;
    WiFi.softAPdisconnect(true);
    WiFi.persistent(false);
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin();

    #ifdef DEBUG_OUTPUT
        Serial.print("Station IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.print("Wi-Fi Channel: ");
        Serial.println(WiFi.channel());
    #endif

    // https://randomnerdtutorials.com/solved-reconnect-esp8266-nodemcu-to-wifi/
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    initWebserverFiles();
    otaUpdate_init(&server);

    // start webserver
    server.begin();

    leds_wifiConnected();
}

void wifiHandling_wifiManagerAPOpenedCB(AsyncWiFiManager* manager)
{
    wifiConfig_isAPOpen = true;
    WiFi.persistent(true);
    leds_wifiAPOpen();
    manager->setConnectTimeout(CONNECTION_TIMEOUT_MS / 1000);
}

void wifiHandling_wifiManagerLoop()
{
    wifiManager.loop();
}

void wifiHandling_init()
{
    #ifdef DEBUG_OUTPUT
        wifiManager.setDebugOutput(true);
    #else
        wifiManager.setDebugOutput(false);
    #endif

    wifiManager.setSaveConfigCallback(wifiHandling_wifiManagerSaveCB);
    wifiManager.setAPCallback(wifiHandling_wifiManagerAPOpenedCB);

    // https://randomnerdtutorials.com/solved-reconnect-esp8266-nodemcu-to-wifi/ 
    // https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/generic-examples.html
    wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
    wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

    WiFi.hostname(WIFI_HOSTNAME);

    WiFi.mode(WIFI_AP_STA);         // Set the device as a Station and Soft Access Point simultaneously
    WiFi.begin();
    boolean keepConnecting = true;
    uint8_t connectionStatus;
    unsigned long start = millis();
    while (keepConnecting)
    {
        connectionStatus = WiFi.status();
        if (millis() > start + CONNECTION_TIMEOUT_MS)
        {
            keepConnecting = false;
            #ifdef DEBUG_OUTPUT
                Serial.println("Connection timed out");
            #endif
        }
        if (connectionStatus == WL_CONNECTED || connectionStatus == WL_CONNECT_FAILED)
        {
            keepConnecting = false;
        }
        leds.service();
    }
    if(connectionStatus != WL_CONNECTED)
    {
        wifiManager.setConnectTimeout(1);
        wifiManager.startConfigPortalModeless(CONFIGURATION_AP_NAME, CONFIGURATION_AP_PW);
    }
    else
    {
        wifiHandling_wifiManagerSaveCB();
    }
}