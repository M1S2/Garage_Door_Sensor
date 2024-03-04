#include "otaUpdate.h"
#include "leds.h"

unsigned long ota_progress_millis = 0;

void onOTAStart() 
{
    leds_otaStart();
    Serial.println("OTA update started!");
}

void onOTAProgress(size_t current, size_t final)
{
    // Log every 1 second
    if (millis() - ota_progress_millis > 1000)
    {
        ota_progress_millis = millis();
        Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
    }
}

void onOTAEnd(bool success)
{
    leds_otaEnd(success);
    if (success)
    {
        Serial.println("OTA update finished successfully!");
    } 
    else 
    {
        Serial.println("There was an error during OTA update!");
    }
}

void otaUpdate_init(AsyncWebServer* server)
{
    ElegantOTA.begin(server);
    ElegantOTA.onStart(onOTAStart);
    ElegantOTA.onProgress(onOTAProgress);
    ElegantOTA.onEnd(onOTAEnd);
}

void otaUpdate_loop()
{
    ElegantOTA.loop();
}