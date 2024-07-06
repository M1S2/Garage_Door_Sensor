#include "otaUpdate.h"
#include "leds.h"
#include "memory.h"

unsigned long ota_progress_millis = 0;

MacArrayStruct_t persisted_macs;

void onOTAStart() 
{
    // Load saved sensor MACs to save after OTA again.
    // Otherwise, the saved MACs will be overwritten by OTA update.
    persisted_macs = memory_getSensorMacs();

    LittleFS.end();

    leds_otaStart();
    #ifdef DEBUG_OUTPUT
        Serial.println("OTA update started!");
    #endif
}

void onOTAProgress(size_t current, size_t final)
{
    // Log every 1 second
    if (millis() - ota_progress_millis > 1000)
    {
        ota_progress_millis = millis();
        #ifdef DEBUG_OUTPUT
            Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
        #endif
    }
}

void onOTAEnd(bool success)
{
    leds_otaEnd(success);

    if (LittleFS.begin())
    {
        // Save sensor MACs again. They are overwritten by OTA update.
        memory_saveSensorMacs(persisted_macs.macs);    
    }
    
    if (success)
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("OTA update finished successfully!");
        #endif
    } 
    else 
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("There was an error during OTA update!");
        #endif
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