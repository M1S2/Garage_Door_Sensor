#include "memory.h"
#include <FS.h>
#include <LittleFS.h>

eeprom_content_t eeprom_content_buffered;

void memory_initBufferedContent()
{
    File memoryFile = LittleFS.open(FILENAME_HISTORY, "r");
    memoryFile.read((byte*)&eeprom_content_buffered, sizeof(eeprom_content_buffered));
    memoryFile.close();
    Serial.println("Memory content initialized");
}

void memory_persistBufferedContent()
{
    File memoryFile = LittleFS.open(FILENAME_HISTORY, "w");
    memoryFile.write((byte*)&eeprom_content_buffered, sizeof(eeprom_content_buffered));
    memoryFile.close();
}

void memory_reset()
{
    LittleFS.remove(FILENAME_HISTORY);
    for(int i = 0; i < NUM_SUPPORTED_SENSORS; i++)
    {
        eeprom_content_buffered.sensor_messages_nextElement_Indices[i] = 0;
    }
}

void memory_showMemoryContent()
{
    for(int i = 0; i < NUM_SUPPORTED_SENSORS; i++)
    {
        uint16_t numberMessages = memory_getNumberSensorMessages(i);
        Serial.printf("--- Data for sensor %d (%d messages)\n", i, numberMessages);
        message_sensor_timestamped_t* messages = memory_getSensorMessagesForSensor(i);
        for(int j = 0; j < numberMessages; j++)
        {
            Serial.printf("time=%lld, pinState=%d, voltage_mV=%d\n", messages[j].timestamp, messages[j].msg.pinState, messages[j].msg.batteryVoltage_mV);
        }
    }
}

uint16_t memory_getNumberSensorMessages(uint8_t sensorIndex)
{
    if(sensorIndex >= NUM_SUPPORTED_SENSORS)
    {
        sensorIndex = NUM_SUPPORTED_SENSORS - 1;
    }
    return eeprom_content_buffered.sensor_messages_nextElement_Indices[sensorIndex];
}

message_sensor_timestamped_t* memory_getSensorMessagesForSensor(uint8_t sensorIndex)
{
    if(sensorIndex >= NUM_SUPPORTED_SENSORS)
    {
        sensorIndex = NUM_SUPPORTED_SENSORS - 1;
    }
    return eeprom_content_buffered.sensor_messages[sensorIndex];
}

message_sensor_timestamped_t memory_getLatestSensorMessagesForSensor(uint8_t sensorIndex)
{
    if(sensorIndex >= NUM_SUPPORTED_SENSORS)
    {
        sensorIndex = NUM_SUPPORTED_SENSORS - 1;
    }
    uint16_t nextIndex = eeprom_content_buffered.sensor_messages_nextElement_Indices[sensorIndex];
    if(nextIndex > 0)
    {
        return eeprom_content_buffered.sensor_messages[sensorIndex][nextIndex - 1];
    }
    else
    {
        // no messages available yet. Return the first message. The user should use the memory_getNumberSensorMessages() method to see, if this message is valid.
        return eeprom_content_buffered.sensor_messages[sensorIndex][0];
    }
}

eeprom_general_settings_t memory_getGeneralSettings()
{
    return eeprom_content_buffered.general_settings;
}

void memory_addSensorMessage(uint8_t sensorIndex, message_sensor_timestamped_t sensorMessage)
{
    if(sensorIndex >= NUM_SUPPORTED_SENSORS)
    {
        sensorIndex = NUM_SUPPORTED_SENSORS - 1;
    }

    uint16_t nextIndex = eeprom_content_buffered.sensor_messages_nextElement_Indices[sensorIndex];
    eeprom_content_buffered.sensor_messages[sensorIndex][nextIndex] = sensorMessage;
    eeprom_content_buffered.sensor_messages_nextElement_Indices[sensorIndex]++;

    memory_persistBufferedContent();
}

void memory_setGeneralSettings(eeprom_general_settings_t generalSettings)
{
    eeprom_content_buffered.general_settings = generalSettings;
    memory_persistBufferedContent();
}