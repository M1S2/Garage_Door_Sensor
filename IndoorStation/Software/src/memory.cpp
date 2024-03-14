#include "memory.h"
#include <FS.h>
#include <LittleFS.h>

// https://github.com/esp8266/Arduino/blob/master/cores/esp8266/FS.h

void memory_reset()
{
	for(int i = 0; i < NUM_SUPPORTED_SENSORS; i++)
    {
		char strBuf[32];
		sprintf(strBuf, FILENAME_HISTORY_SENSOR_FORMAT, i);
		LittleFS.remove(strBuf);
	}
    LittleFS.remove(FILENAME_SENSOR_MACS);
}

void memory_showMemoryContent()
{
    FSInfo info;
    LittleFS.info(info);
    Serial.println("--- File System usage");
    Serial.printf("%d of %d bytes used (%.2f %%)\n", info.usedBytes, info.totalBytes, (info.usedBytes * 100.0f) / info.totalBytes);
        
    Serial.println("--- MACs");
    MacArrStruct_t macStruct = memory_getSensorMacs();
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
}

uint16_t memory_getNumberSensorMessages(uint8_t sensorIndex)
{
    if(sensorIndex >= NUM_SUPPORTED_SENSORS)
    {
        sensorIndex = NUM_SUPPORTED_SENSORS - 1;
    }
	
	char strBuf[32];
	sprintf(strBuf, FILENAME_HISTORY_SENSOR_FORMAT, sensorIndex);
	File memoryFile = LittleFS.open(strBuf, "r");
    size_t fileSize = memoryFile.size();
    memoryFile.close();
	
    return fileSize / sizeof(message_sensor_timestamped_t);
}

void memory_getSensorMessagesForSensor(uint8_t sensorIndex, message_sensor_timestamped_t* sensorMessagesBuffer)
{
    if(sensorMessagesBuffer == NULL) { return; }

    if(sensorIndex >= NUM_SUPPORTED_SENSORS)
    {
        sensorIndex = NUM_SUPPORTED_SENSORS - 1;
    }
	
	uint16_t numberSensorMessages = memory_getNumberSensorMessages(sensorIndex);
	
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

message_sensor_timestamped_t memory_getLatestSensorMessagesForSensor(uint8_t sensorIndex)
{
    if(sensorIndex >= NUM_SUPPORTED_SENSORS)
    {
        sensorIndex = NUM_SUPPORTED_SENSORS - 1;
    }
	
	char strBuf[32];
	sprintf(strBuf, FILENAME_HISTORY_SENSOR_FORMAT, sensorIndex);
	File memoryFile = LittleFS.open(strBuf, "r");
	memoryFile.seek(sizeof(message_sensor_timestamped_t), SeekEnd);		// move file pointer to beginning of last entry

	message_sensor_timestamped_t sensorMessage;
	memoryFile.read((uint8_t*)&sensorMessage, sizeof(message_sensor_timestamped_t));
	memoryFile.close();
	return sensorMessage;
}

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

void memory_saveSensorMacs(uint8_t sensor_macs[NUM_SUPPORTED_SENSORS][6])
{
    File memoryFile = LittleFS.open(FILENAME_SENSOR_MACS, "w+");
    memoryFile.write((uint8_t*)sensor_macs, NUM_SUPPORTED_SENSORS * 6 * 1);
	memoryFile.close();
}

MacArrStruct_t memory_getSensorMacs()
{
    MacArrStruct_t macStruct;

    File memoryFile = LittleFS.open(FILENAME_SENSOR_MACS, "r");
    memoryFile.read((uint8_t*)macStruct.macs, NUM_SUPPORTED_SENSORS * 6 * 1);
	memoryFile.close();

    return macStruct;
}