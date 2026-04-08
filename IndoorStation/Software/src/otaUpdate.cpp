#include "otaUpdate.h"
#include "leds.h"
#include "memory.h"
#include "main.h"

unsigned long ota_progress_millis = 0;

system_config_t sysConfig_backup;

void onOTAStart() 
{
    // Backup the current system config before OTA update.
    memcpy(&sysConfig_backup, &sysConfig, sizeof(system_config_t));

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
        // Restore the system config after OTA update. The OTA update overwrites the whole flash, so the system config is lost.
        // By restoring the system config, the sensor MACs and modes are preserved during OTA update.
        memcpy(&sysConfig, &sysConfig_backup, sizeof(system_config_t));
        memory_saveSystemConfig(sysConfig);
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