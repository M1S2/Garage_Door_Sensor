#ifndef LEDS_h
#define LEDS_h

#include <WS2812FX.h>

#define NUM_LEDS                3                         // Number of WS2812B leds
#define LEDS_PIN                14                        // The pin which is used to control the WS2812 leds

#define SENSOR1_LED_INDEX       0
#define SENSOR2_LED_INDEX       1
#define WIFI_LED_INDEX          2

#define LIGHT_BLUE              (uint32_t)0x0030ff

#define COLOR_DOOR_OPEN         GREEN       // LED color used to indicate an open door
#define COLOR_DOOR_CLOSED       RED         // LED color used to indicate a closed door
#define COLOR_WIFI_CONNECTED    LIGHT_BLUE  // LED color used to indicate that the Wifi is connected
#define COLOR_WIFI_CFG_AP_OPEN  WHITE       // LED color used to indicate that the Wifi configuration access point is running
#define COLOR_WIFI_FAILED       RED         // LED color used to indicate that the Wifi connection failed
#define COLOR_PAIRING_1         RED         // First LED color for pairing indication (sensor or wifi)
#define COLOR_PAIRING_2         BLUE        // Second LED color for pairing indication (sensor or wifi)
#define COLOR_OTA               YELLOW      // LED color used to indicate a OTA update
#define COLOR_OTA_FAILURE       RED         // LED color used to indicate a OTA update failure
#define COLOR_OTA_SUCCESS       GREEN       // LED color used to indicate a OTA update success
#define COLOR_SENSOR_CHARGING   PURPLE      // LED color used to indicate that the sensor is charging
#define COLOR_OFF               BLACK       // LED color off

extern WS2812FX leds;

void leds_singleOff(uint8_t index);
void leds_allOff();

void leds_sensorStatus(uint8_t index, bool isOpen, bool batteryLow = false);
void leds_sensorPairing(uint8_t index);
void leds_sensorCharging(uint8_t index);

void leds_wifiConnecting();
void leds_wifiConnected();
void leds_wifiFailed();
void leds_wifiAPOpen();

void leds_otaStart();
void leds_otaEnd(bool success);

#endif