#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

extern bool configuration_AP_open;
extern bool wifiInit_inProgress;

extern AsyncWebServer webServer;
extern IPAddress configurationAcessPoint_localIP;

#include <DNSServer.h>
extern DNSServer dnsServer;
#define DNS_PORT 53
#define DEVICE_NAME "Garage Door Sensor"

bool WiFiBegin(bool startWithConfigurationAcessPoint = false);
bool ConnectToAccessPoint(uint timeout_ms = 5000, bool openConfigurationAPOnFailure = true);
void CreateConfigurationAccessPoint();

void clearWifiCredentials();

#endif // WIFIMANAGER_H