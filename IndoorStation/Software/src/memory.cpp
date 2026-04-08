#include "memory.h"
#include "utils.h"
#include <FS.h>
#include <LittleFS.h>

// https://github.com/esp8266/Arduino/blob/master/cores/esp8266/FS.h

void memory_removeAllData()
{
    memory_removeSensorHistory(-1);

    LittleFS.remove(FILENAME_PERSISTED_SYSTEM_CONFIG);
    // The system config file will be automatically recreated with default values when the device is restarted.
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void memory_removeSensorHistory(int8_t sensorIndex)
{
    if(sensorIndex < 0)
    {
        for(int i = 0; i < NUM_SUPPORTED_SENSORS; i++)
        {
            char strBuf[32];
            sprintf(strBuf, FILENAME_HISTORY_SENSOR_FORMAT, i);
            LittleFS.remove(strBuf);
        }
    }
    else
    {
        char strBuf[32];
        sprintf(strBuf, FILENAME_HISTORY_SENSOR_FORMAT, sensorIndex);
        LittleFS.remove(strBuf);
    }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

const char* memory_getMemoryUsageString()
{
    FSInfo info;
    LittleFS.info(info);

    static char buffer[100];
    snprintf(buffer, sizeof(buffer), "%d von %d Bytes belegt (%.2f %%)", info.usedBytes, info.totalBytes, (info.usedBytes * 100.0f) / info.totalBytes); 
    return buffer;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void memory_showMemoryContent()
{
    #ifdef DEBUG_OUTPUT
        FSInfo info;
        LittleFS.info(info);
        Serial.println("--- File System usage");
        Serial.println(memory_getMemoryUsageString());
        
        Serial.println("--- MACs");
        MacArrayStruct_t macStruct = memory_getSensorMacs();
        for(int sensorIdx = 0; sensorIdx < NUM_SUPPORTED_SENSORS; sensorIdx++)
        {
            Serial.printf("MAC Sensor #%d: %02X:%02X:%02X:%02X:%02X:%02X \n", sensorIdx + 1, macStruct.macs[sensorIdx][0], macStruct.macs[sensorIdx][1], macStruct.macs[sensorIdx][2], macStruct.macs[sensorIdx][3], macStruct.macs[sensorIdx][4], macStruct.macs[sensorIdx][5]);    
        }

        for(int sensorIdx = 0; sensorIdx < NUM_SUPPORTED_SENSORS; sensorIdx++)
        {
            uint16_t numberMessages = memory_getNumberSensorMessages(sensorIdx);
            Serial.printf("--- Data for sensor %d (%d messages)\n", sensorIdx, numberMessages);
            message_sensor_timestamped_t sensorMessages[numberMessages];
            memory_getSensorMessagesForSensor(sensorIdx, sensorMessages);
            for(int msgIdx = 0; msgIdx < numberMessages; msgIdx++)
            {
                Serial.printf("time=%lld, pinState=%d, voltage_mV=%d\n", sensorMessages[msgIdx].timestamp, sensorMessages[msgIdx].msg.pinState, sensorMessages[msgIdx].msg.batteryVoltage_mV);
            }
        }
    #endif
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

uint16_t memory_getNumberSensorMessages(uint8_t sensorIndex)
{
    if(sensorIndex >= NUM_SUPPORTED_SENSORS)
    {
        sensorIndex = NUM_SUPPORTED_SENSORS - 1;
    }
	
	char strBuf[32];
	sprintf(strBuf, FILENAME_HISTORY_SENSOR_FORMAT, sensorIndex);
    if(LittleFS.exists(strBuf))
    {
        File memoryFile = LittleFS.open(strBuf, "r");
        size_t fileSize = memoryFile.size();
        memoryFile.close();
        
        return fileSize / sizeof(message_sensor_timestamped_t);
    }
    else
    {
        return 0;
    }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void memory_getSensorMessagesForSensor(uint8_t sensorIndex, message_sensor_timestamped_t* sensorMessagesBuffer)
{
    if(sensorMessagesBuffer == NULL) { return; }

    if(sensorIndex >= NUM_SUPPORTED_SENSORS)
    {
        sensorIndex = NUM_SUPPORTED_SENSORS - 1;
    }
	
	uint16_t numberSensorMessages = memory_getNumberSensorMessages(sensorIndex);
	
    if(numberSensorMessages > 0)
    {
        char strBuf[32];
        sprintf(strBuf, FILENAME_HISTORY_SENSOR_FORMAT, sensorIndex);
        File memoryFile = LittleFS.open(strBuf, "r");

        for(int i = 0; i < numberSensorMessages; i++)
        {
            message_sensor_timestamped_t sensorMessage;
            memoryFile.read((uint8_t*)&sensorMessage, sizeof(message_sensor_timestamped_t));
            sensorMessagesBuffer[i] = sensorMessage;
        }
        memoryFile.close();
    }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

message_sensor_timestamped_t memory_getLatestSensorMessagesForSensor(uint8_t sensorIndex)
{
    if(sensorIndex >= NUM_SUPPORTED_SENSORS)
    {
        sensorIndex = NUM_SUPPORTED_SENSORS - 1;
    }
	
	char strBuf[32];
	sprintf(strBuf, FILENAME_HISTORY_SENSOR_FORMAT, sensorIndex);
    if(LittleFS.exists(strBuf))
    {
        File memoryFile = LittleFS.open(strBuf, "r");
        memoryFile.seek(sizeof(message_sensor_timestamped_t), SeekEnd);		// move file pointer to beginning of last entry

        message_sensor_timestamped_t sensorMessage;
        memoryFile.read((uint8_t*)&sensorMessage, sizeof(message_sensor_timestamped_t));
        memoryFile.close();
        return sensorMessage;
    }
    else
    {
        // return an invalid message if no message exists for the requested sensor
        message_sensor_timestamped_t invalidMessage;
        invalidMessage.timestamp = -1;
        return invalidMessage;
    }

}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

bool memory_addSensorMessage(uint8_t sensorIndex, message_sensor_timestamped_t sensorMessage)
{
    if(sensorIndex >= NUM_SUPPORTED_SENSORS)
    {
        sensorIndex = NUM_SUPPORTED_SENSORS - 1;
    }
	
	char strBuf[32];
	sprintf(strBuf, FILENAME_HISTORY_SENSOR_FORMAT, sensorIndex);
	File memoryFile = LittleFS.open(strBuf, "a");
	size_t writtenSize = memoryFile.write((uint8_t*)&sensorMessage, sizeof(message_sensor_timestamped_t));
	memoryFile.close();

    return writtenSize == sizeof(message_sensor_timestamped_t);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void memory_setDefaultSystemConfig(system_config_t& sysConfig)
{
    for(uint8_t i = 0; i < NUM_SUPPORTED_SENSORS; i++)
    {
        memset(sysConfig.sensors[i].mac, 0, sizeof(sysConfig.sensors[i].mac));
        memset(sysConfig.sensors[i].lmk, 0, sizeof(sysConfig.sensors[i].lmk));
        sysConfig.sensors[i].mode = SENSOR_MODE_NORMAL;
    }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

bool memory_saveSystemConfig(system_config_t& sysConfig)
{
    persisted_system_config_t persistedConfig;
    persistedConfig.magic = MEMORY_PERSISTED_SYSTEM_CONFIG_MAGIC;
    persistedConfig.system_config = sysConfig;
    persistedConfig.crc32 = utils_calculateCRC32((uint8_t*)&persistedConfig, sizeof(persisted_system_config_t) - sizeof(persistedConfig.crc32));

    File memoryFile = LittleFS.open(FILENAME_PERSISTED_SYSTEM_CONFIG, "w");
    if(!memoryFile)
    {
        return false;
    }

    size_t written = memoryFile.write((uint8_t*)&persistedConfig, sizeof(persisted_system_config_t));

    memoryFile.close();
    return (written == sizeof(persisted_system_config_t));
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

bool memory_loadSystemConfig(system_config_t& sysConfig)
{
    File memoryFile = LittleFS.open(FILENAME_PERSISTED_SYSTEM_CONFIG, "r");
    if(!memoryFile)
    {
        return false;
    }
    
    if(memoryFile.size() != sizeof(persisted_system_config_t))
    {
        memoryFile.close();
        return false;
    }

    persisted_system_config_t persistedConfig;
    size_t bytesRead = memoryFile.read((uint8_t*)&persistedConfig, sizeof(persisted_system_config_t));

    memoryFile.close();

    if(bytesRead != sizeof(persisted_system_config_t))
    {
        return false;
    }
    
    if(persistedConfig.magic != MEMORY_PERSISTED_SYSTEM_CONFIG_MAGIC)
    {
        return false;
    }
    
    uint32_t expectedCRC = utils_calculateCRC32((uint8_t*)&persistedConfig, sizeof(persisted_system_config_t) - sizeof(persistedConfig.crc32));
    if(persistedConfig.crc32 != expectedCRC)
    {
        return false;
    }

    sysConfig = persistedConfig.system_config;
    return true;
}