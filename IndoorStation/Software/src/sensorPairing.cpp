#include "sensorPairing.h"
#include "leds.h"
#include <ArduinoJson.h>

Sensor_Pairing_Modes sensorPairingMode = PAIRING_MODE_NONE;

void sensorPairing_Stop(AsyncEventSource* events)
{
  sensorPairingMode = PAIRING_MODE_NONE;
  updateWebsiteForSensorPairingStatus(events);
}

// sensorIndex = 0..1
void sensorPairing_Start(int sensorIndex, AsyncEventSource* events)
{
  switch(sensorIndex)
  {
    case 0: sensorPairingMode = PAIRING_MODE_SENSOR1; leds_sensorPairing(SENSOR1_LED_INDEX); updateWebsiteForSensorPairingStatus(events); break;
    case 1: sensorPairingMode = PAIRING_MODE_SENSOR2; leds_sensorPairing(SENSOR2_LED_INDEX); updateWebsiteForSensorPairingStatus(events); break;
    default: sensorPairing_Stop(events); break;
  }
}

void sensorPairing_MoveToNext(AsyncEventSource* events)
{
  switch(sensorPairingMode)
  {
    case PAIRING_MODE_NONE: sensorPairing_Start(PAIRING_MODE_SENSOR1, events); break;
    case PAIRING_MODE_SENSOR1: sensorPairing_Start(PAIRING_MODE_SENSOR2, events); break;
    case PAIRING_MODE_SENSOR2: sensorPairing_Stop(events); break;
  }
}

void updateWebsiteForSensorPairingStatus(AsyncEventSource* events)
{
  // create a JSON document with the data and send it by event to the web page
  StaticJsonDocument<1000> root;
  String payload;
  root["pairing_id"] = sensorPairingMode;
  root["pairing_active"] = (sensorPairingMode != PAIRING_MODE_NONE);
  serializeJson(root, payload);
  Serial.printf("event send: %s\n", payload.c_str());
  events->send(payload.c_str(), "new_sensor_pairing_status", millis());
}