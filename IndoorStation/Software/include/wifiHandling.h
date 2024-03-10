#ifndef WIFI_HANDLING_H
#define WIFI_HANDLING_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

extern AsyncWebServer server;
extern AsyncEventSource events;

extern bool wifiConfig_isAPOpen;

void wifiHandling_wifiManagerLoop();
void wifiHandling_init();
void wifiHandling_eraseCredentials();

bool wps_start();

#endif