#include <Schedule.h>
#include <ESPAsyncWiFiManager.h>
#include "wifiHandling.h"
#include "leds.h"
#include "otaUpdate.h"
#include "config.h"
#include "main.h"

AsyncWebServer server(80);
AsyncEventSource events("/events");

DNSServer dns;
AsyncWiFiManager wifiManager(&server, &dns);

// The event handlers are initialized in the setup()
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

bool wifiConfig_isAPOpen = false;

void wifiHandling_eraseCredentials() 
{
    WiFi.disconnect(true);
    ESP.eraseConfig();
    Serial.println("WiFi credentials erased");
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

    Serial.print("Station IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Wi-Fi Channel: ");
    Serial.println(WiFi.channel());

    // https://randomnerdtutorials.com/solved-reconnect-esp8266-nodemcu-to-wifi/
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    initWebserverFiles();
    // events 
    events.onConnect([](AsyncEventSourceClient *client)
    {
        updateWebsite();      // Update the values on the website when it is opened (or reloaded in the browser)
    });
    server.addHandler(&events);
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
    wifiManager.setSaveConfigCallback(wifiHandling_wifiManagerSaveCB);
    wifiManager.setAPCallback(wifiHandling_wifiManagerAPOpenedCB);

    // https://randomnerdtutorials.com/solved-reconnect-esp8266-nodemcu-to-wifi/ 
    // https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/generic-examples.html
    wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
    wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

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
            Serial.println("Connection timed out");
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

/**********************************************************************/

void wps_status_cb(wps_cb_status status)
{
  Serial.printf("wps cb status: %d\r\n", status);
  switch(status)
  {
    case WPS_CB_ST_SUCCESS:
      if(!wifi_wps_disable())
      {
        Serial.println("wps disable failed");
      }
      //wifi_station_connect();
      wifiHandling_wifiManagerSaveCB();
      break;
    case WPS_CB_ST_FAILED:
      Serial.println("wps FAILED");
      leds_wifiAPOpen();
      break;
    case WPS_CB_ST_TIMEOUT:
      Serial.println("wps TIMEOUT");
      leds_wifiAPOpen();
      break;
    case WPS_CB_ST_WEP:
      Serial.println("wps WEP");
      leds_wifiAPOpen();
      break;
    case WPS_CB_ST_UNK:
      Serial.println("wps UNKNOWN");
      if(!wifi_wps_disable())
      {
        Serial.println("wps disable failed");
      }
      leds_wifiAPOpen();
      break;
  }
}

bool wps_start()
{
  if(!WiFi.enableSTA(true))
  {
    // enable STA failed
    Serial.println("wps enable STA failed");
    return false;
  }

  WiFi.disconnect();

  if(!wifi_wps_disable())
  {
    Serial.println("wps disable failed");
    return false;
  }

  // so far only WPS_TYPE_PBC is supported (SDK 1.2.0)
  if(!wifi_wps_enable(WPS_TYPE_PBC))
  {
    Serial.println("wps enable failed");
    return false;
  }

  if(!wifi_set_wps_cb((wps_st_cb_t) &wps_status_cb))
  {
    Serial.println("wps cb failed");
    return false;
  }

  if(!wifi_wps_start())
  {
    Serial.println("wps start failed");
    return false;
  }
  
  leds_wifiPairing();

  return true;
}