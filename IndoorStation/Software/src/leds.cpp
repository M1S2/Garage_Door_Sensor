#include "leds.h"

WS2812FX leds = WS2812FX(NUM_LEDS, LEDS_PIN, NEO_GRB + NEO_KHZ800);

void setLedStatic(uint8_t index, const uint32_t color)
{
    leds.setSegment(index, index, index, FX_MODE_STATIC, color);
    leds.trigger();
    leds.service();                     // update the segment and remove it directly (no need to update the whole time)
    leds.removeActiveSegment(index);
}

/**********************************************************************/

void leds_singleOff(uint8_t index)
{
    setLedStatic(index, COLOR_OFF);
}

void leds_allOff()
{
    leds_singleOff(SENSOR1_LED_INDEX);
    leds_singleOff(SENSOR2_LED_INDEX);
    leds_singleOff(WIFI_LED_INDEX);
}

/**********************************************************************/

void leds_sensorStatus(uint8_t index, bool isOpen, bool batteryLow)
{
    if(isOpen)
    {
        if(batteryLow)
        {
            leds.setSegment(index, index, index, FX_MODE_FADE, COLORS(COLOR_DOOR_OPEN, DARK(COLOR_DOOR_OPEN)), 2000);
            leds.addActiveSegment(index);
        }
        else
        {
            setLedStatic(index, COLOR_DOOR_OPEN);
        }
    }
    else
    {
        if(batteryLow)
        {
            leds.setSegment(index, index, index, FX_MODE_FADE, COLORS(COLOR_DOOR_CLOSED, DARK(COLOR_DOOR_CLOSED)), 2000);
            leds.addActiveSegment(index);
        }
        else
        {
            setLedStatic(index, COLOR_DOOR_CLOSED);
        }
    }
}

void leds_sensorPairing(uint8_t index)
{
    leds.setSegment(index, index, index, FX_MODE_BLINK, COLORS(COLOR_PAIRING_1, COLOR_PAIRING_2), 800);
    leds.addActiveSegment(index);
}

void leds_sensorCharging(uint8_t index)
{
    setLedStatic(index, COLOR_SENSOR_CHARGING);
}

/**********************************************************************/

void leds_wifiPairing()
{
    leds.setSegment(WIFI_LED_INDEX, WIFI_LED_INDEX, WIFI_LED_INDEX, FX_MODE_BLINK, COLORS(COLOR_PAIRING_1, COLOR_PAIRING_2), 800);
    leds.addActiveSegment(WIFI_LED_INDEX);
}

void leds_wifiConnecting()
{
    leds.setSegment(WIFI_LED_INDEX, WIFI_LED_INDEX, WIFI_LED_INDEX, FX_MODE_FADE, COLORS(COLOR_WIFI_CONNECTED, COLOR_OFF), 1000);
    leds.addActiveSegment(WIFI_LED_INDEX);
}

void leds_wifiConnected()
{
    setLedStatic(WIFI_LED_INDEX, COLOR_WIFI_CONNECTED);
}

void leds_wifiFailed()
{
    setLedStatic(WIFI_LED_INDEX, COLOR_WIFI_FAILED);
}

void leds_wifiAPOpen()
{
    setLedStatic(WIFI_LED_INDEX, COLOR_WIFI_CFG_AP_OPEN);
}

/**********************************************************************/

void leds_otaStart()
{
    leds.setSegment(SENSOR1_LED_INDEX, SENSOR1_LED_INDEX, SENSOR1_LED_INDEX, FX_MODE_BLINK, COLORS(COLOR_OTA, COLOR_OFF), 1000);
    leds.setSegment(SENSOR2_LED_INDEX, SENSOR2_LED_INDEX, SENSOR2_LED_INDEX, FX_MODE_BLINK, COLORS(COLOR_OTA, COLOR_OFF), 1000);
    leds.setSegment(WIFI_LED_INDEX, WIFI_LED_INDEX, WIFI_LED_INDEX, FX_MODE_BLINK, COLORS(COLOR_OTA, COLOR_OFF), 1000);
    leds.addActiveSegment(SENSOR1_LED_INDEX);
    leds.addActiveSegment(SENSOR2_LED_INDEX);
    leds.addActiveSegment(WIFI_LED_INDEX);
}

void leds_otaEnd(bool success)
{
    if(success)
    {
        setLedStatic(SENSOR1_LED_INDEX, COLOR_OTA_SUCCESS);
        setLedStatic(SENSOR2_LED_INDEX, COLOR_OTA_SUCCESS);
        setLedStatic(WIFI_LED_INDEX, COLOR_OTA_SUCCESS);
    }
    else
    {
        setLedStatic(SENSOR1_LED_INDEX, COLOR_OTA_FAILURE);
        setLedStatic(SENSOR2_LED_INDEX, COLOR_OTA_FAILURE);
        setLedStatic(WIFI_LED_INDEX, COLOR_OTA_FAILURE);
    }
}