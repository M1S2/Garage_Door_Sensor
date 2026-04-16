// This is the code that receives all garage door states
// It differentiates between the sensors by their MAC addresses

// https://wolles-elektronikkiste.de/esp-now#multi_transm_1_recv  (Ein Transmitter, ein Receiver – Advanced (ESP32))
// https://randomnerdtutorials.com/esp-now-auto-pairing-esp32-esp8266/

#include <Arduino.h>
#include <espnow.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <Button2.h>
#include <Schedule.h>
#include "main.h"
#include "config.h"
#include "structures.h"
#include "leds.h"
#include "battery.h"
#include "wifiHandling.h"
#include "timeHandling.h"
#include "otaUpdate.h"
#include "memory.h"
#include "pairing.h"
#include "utils.h"
#include "version.h"

// To increase the FS size https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#flash-layout

system_config_t sysConfig;

Button2 btn_pairing, btn_reset;             // create button objects

message_sensor_t sensor_message;
message_pairing_t pairing_message;
bool sensor_message_received = false;
bool pairing_message_received = false;
uint8_t received_mac_addr[6];

message_sensor_timestamped_t sensor_messages_latest[NUM_SUPPORTED_SENSORS];

File serverGetDataMemoryFile;
int8_t serverGetDataSensorIndex;
time_t serverGetDataTimeFrom;
time_t serverGetDataTimeTo;
message_sensor_timestamped_t serverGetDataPendingMessage;
bool serverGetDataHasPendingMessage = false;

/**********************************************************************/

void main_updateLeds_sensorStatus()
{
    for(uint8_t i = 0; i < NUM_SUPPORTED_SENSORS; i++)
    {
        if(i >= NUM_SENSOR_LEDS)
        {
            // Only the first NUM_SENSOR_LEDS leds are used for sensor status.
            // If more sensors are supported than NUM_SENSOR_LEDS, the additional sensors are not indicated by leds.
            break;
        }

        switch(sysConfig.sensors[i].mode)
        {
            case SENSOR_MODE_DISABLED: leds_singleOff(i); break;
            case SENSOR_MODE_CHARGING: leds_sensorCharging(i); break;
            case SENSOR_MODE_PAIRING: leds_sensorPairing(i); break;
            case SENSOR_MODE_NORMAL:
            case SENSOR_MODE_ONLY_DISPLAY:
            {
                if(sensor_messages_latest[i].timestamp != -1)
                {
                    bool isOpen = (sensor_messages_latest[i].msg.pinState == SENSOR_PIN_STATE_OPEN);
                    leds_sensorStatus(i, isOpen, battery_isEmpty(sensor_messages_latest[i].msg.batteryVoltage_mV));
                }
                else
                {
                    leds_singleOff(i);
                }
            }
            default: break;
        }
    }
}

void updateLastSensorMessages()
{
    // invalidate all last sensor messages and get the saved ones
    for(uint8_t i=0; i < NUM_SUPPORTED_SENSORS; i++)
    {
        sensor_messages_latest[i].timestamp = -1;

        if(memory_getNumberSensorMessages(i) > 0)
        {
            sensor_messages_latest[i] = memory_getLatestSensorMessagesForSensor(i);
        }
    }

    main_updateLeds_sensorStatus();
}

/**********************************************************************/

void messageReceived(uint8_t* mac_addr, uint8_t* data, uint8 len)
{
    memcpy(&received_mac_addr, mac_addr, sizeof(received_mac_addr));

    // Get the first 4 bytes of the received data and check if it was a pairing message from the sensor
    uint32_t magicNumber = 0;
    if(len >= 4)
    {
        magicNumber = (data[3] << 24) + (data[2] << 16) + (data[1] << 8) + data[0];
    }

    pairing_message_received = false;
    sensor_message_received = false;
    if(magicNumber == PAIRING_MAGIC_NUMBER)
    {
        if (len >= sizeof(pairing_message))
        {
            memcpy(&pairing_message, data, sizeof(pairing_message));
            pairing_message_received = true;
        }
    }
    else
    {
        if (len >= sizeof(sensor_message))
        {
            memcpy(&sensor_message, data, sizeof(sensor_message));
            sensor_message_received = true;
        }
    }
}

/**********************************************************************/

void setSensorMode(uint8_t sensorIndex, SensorModes mode)
{
    if(sensorIndex >= 0 && sensorIndex < NUM_SUPPORTED_SENSORS)
    {
        if(mode == SENSOR_MODE_PAIRING)
        {
            pairing_enablePairingModeForSensor(sensorIndex);
        }
        else
        {
            pairing_disablePairingModeForSensor(sensorIndex);
            sysConfig.sensors[sensorIndex].mode = mode;
            memory_saveSystemConfig(sysConfig);   // Only save if not in pairing mode, because pairing mode is only temporary and should not be saved persistently
        }

        events.send("", SERVER_EVENT_SENSOR_MODE_CHANGED, millis());
        main_updateLeds_sensorStatus();
    }
}

/**********************************************************************/

void btnHandler_reset_longClick(Button2& btn)
{
    memory_removeAllData();
    wifiHandling_eraseCredentials();
    delay(2000);
    ESP.restart();
}

void btnHandler_pairing_longClick(Button2& btn)
{
    // button is pressed long:
    // - if no sensor is in pairing mode, set the first sensor to pairing mode
    // - if at least one sensor is in pairing mode, set all sensors that are in pairing mode to normal mode

    int indexFirstSensorInPairingMode = pairing_findSensorIndexInPairingMode();
    if(indexFirstSensorInPairingMode != -1)
    {
        // at least one sensor is in pairing mode: disable pairing mode for all sensors
        pairing_stopAllSensorsPairingMode();
        events.send("", SERVER_EVENT_SENSOR_MODE_CHANGED, millis());
        main_updateLeds_sensorStatus();
    }
    else
    {
        // no sensor is in pairing mode: enable pairing mode for the first sensor
        setSensorMode(0, SENSOR_MODE_PAIRING);
    }
}

void btnHandler_pairing_click(Button2& btn)
{
    // button is pressed short:
    // - if no sensor is in pairing mode, do nothing
    // - otherwise find the index of the first sensor with pairing mode. Set it to normal mode and set the next sensor to pairing mode. If it was the last sensor begin with the first again.

    int indexFirstSensorInPairingMode = pairing_findSensorIndexInPairingMode();
    if(indexFirstSensorInPairingMode != -1)
    {
        // increment to the next sensor and roll over at the last sensor.
        int indexNextSensor = indexFirstSensorInPairingMode + 1;
        if(indexNextSensor >= NUM_SENSOR_LEDS)
        {
            indexNextSensor = 0;
        }
        setSensorMode(indexNextSensor, SENSOR_MODE_PAIRING);
    }
}

/**********************************************************************/

/**
 * Set default values for the system config (e.g. empty MAC addresses, normal mode for all sensors).
 * @param sysConfig The system config struct, for which the default values are set.
 */
void main_setDefaultSystemConfig(system_config_t& sysConfig)
{
    for(uint8_t i = 0; i < NUM_SUPPORTED_SENSORS; i++)
    {
        memset(sysConfig.sensors[i].mac, 0, sizeof(sysConfig.sensors[i].mac));
        memset(sysConfig.sensors[i].lmk, 0, sizeof(sysConfig.sensors[i].lmk));  // set to all 0 to indicate that no LMK is set for this sensor. A valid LMK must be generated (e.g. with main_makeSureEncryptionKeysAreSetInSystemConfig()).
        sysConfig.sensors[i].mode = SENSOR_MODE_NORMAL;
        sysConfig.sensors[i].isPaired = false;
        sysConfig.sensors[i].useEncryption = false;
        memset(sysConfig.sensors[i].name, 0, sizeof(sysConfig.sensors[i].name));
    }
    memset(sysConfig.pmk, 0, sizeof(sysConfig.pmk));  // set to all 0 to indicate that no PMK is set for this sensor. A valid PMK must be generated (e.g. with main_makeSureEncryptionKeysAreSetInSystemConfig()).
}

/**
 * Make sure that for all sensors encryption keys (LMK) and a PMK are set in the system config.
 * If not, generate random keys and save them in the system config.
 * This ensures that encryption is possible for all sensors and also prevents that empty keys are used, which are not valid for encryption.
 * @return true if new keys were generated and saved, false if all keys were already set.
 */
bool main_makeSureEncryptionKeysAreSetInSystemConfig(system_config_t& sysConfig)
{
    bool keysGenerated = false;
    for(uint8_t i = 0; i < NUM_SUPPORTED_SENSORS; i++)
    {
        if(utils_isKeyEmpty(sysConfig.sensors[i].lmk, ESPNOW_KEY_LEN))
        {
            // If the sensor has an empty LMK, generate a random LMK and save it in the system config. This ensures that encryption is possible for this sensor.
            utils_generateRandomKey(sysConfig.sensors[i].lmk, ESPNOW_KEY_LEN);
            keysGenerated = true;
        }
    }

    if(utils_isKeyEmpty(sysConfig.pmk, ESPNOW_KEY_LEN))
    {
        // If the PMK is empty, generate a random PMK and save it in the system config. This ensures that encryption is possible.
        utils_generateRandomKey(sysConfig.pmk, ESPNOW_KEY_LEN);
        keysGenerated = true;
    }

    if(keysGenerated)
    {
        // If new keys were generated, save the system config to persist the new keys in memory.
        memory_saveSystemConfig(sysConfig);
    }
    return keysGenerated;
}

/**********************************************************************/

/**
 * Remove an ESP-NOW peer if it exists.
 * It is identified by the MAC address.
 * @param sensorConfig The sensor config struct containing the MAC address for the peer.
 * @return true if the peer was removed successfully (and existed), false otherwise (e.g. no peer existed).
 */
bool main_removePeer(sensor_config_t& sensorConfig)
{
    if (esp_now_is_peer_exist((uint8_t*)sensorConfig.mac))
    {
        esp_now_del_peer((uint8_t*)sensorConfig.mac);
        return true;
    }
    return false;
}

/**
 * Add an encrypted peer with the given MAC address and LMK.
 * If a peer with the given MAC address already exists, it is deleted before adding the new peer.
 * This is important to ensure that the new LMK is used for the peer, especially if they have changed.
 * It is also important to only add a new peer with encryption in WIFI_STA mode.
 * Also not mix the use of esp_now_add_peer() for encrypted and unencrypted peers (seems to distract the ESP8266 encryption)  
 * @param sensorConfig The sensor config struct containing the MAC address and LMK for the peer.
 * @return true if the peer was added successfully, false otherwise.
 */
bool main_addEncryptedPeer(sensor_config_t& sensorConfig)
{
    if(!sensorConfig.useEncryption)
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("No peer added because encryption is disabled for this sensor!");
        #endif
        return false;
    }

    // If the peer already exists → delete it (important for channel change!)
    main_removePeer(sensorConfig);

    int result = -1;
    result = esp_now_add_peer((uint8_t*)sensorConfig.mac, ESP_NOW_ROLE_SLAVE, 0, (uint8_t*)sensorConfig.lmk, ESPNOW_KEY_LEN);

    #ifdef DEBUG_OUTPUT
        Serial.printf("addEncryptedPeer: %02X:%02X:%02X:%02X:%02X:%02X | ch=%d | res=%d\n", sensorConfig.mac[0], sensorConfig.mac[1], sensorConfig.mac[2], sensorConfig.mac[3], sensorConfig.mac[4], sensorConfig.mac[5], wiFiChannel, result);
    #endif

    if (result != 0)
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("ERROR: Failed to add encrypted peer!");
        #endif
        return false;
    }
    return true;
}

/**********************************************************************/

void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    // For the first chunk: read sensorIndex from the request and open the file
    if (index == 0)
    {
        int sensorIndex = -1;
        if (request->hasParam("sensorIndex", true))
        {
            sensorIndex = request->getParam("sensorIndex", true)->value().toInt();
        }
        if (sensorIndex >= 0 && sensorIndex < NUM_SUPPORTED_SENSORS)
        {
            // open the file with the correct format (ignoring the uploaded filename) on first call and store the file handle in the request object
            char strBuf[32];
            sprintf(strBuf, FILENAME_HISTORY_SENSOR_FORMAT, sensorIndex);
            request->_tempFile = LittleFS.open(strBuf, "w");
        }
        else
        {
            request->_tempFile = File(); // invalid / empty file object
        }
    }

    if (len > 0 && request->_tempFile)    // write only if data exists AND file was opened successfully
    {
        // stream the incoming chunk to the opened file
        request->_tempFile.write(data, len);
    }

    // Upload finished
    if (final)
    {
        if (request->_tempFile)    // file was opened successfully before
        {
            // close the file handle as the upload is now done
            request->_tempFile.close();
            request->redirect("/system_management.html");     // Only redirect if the file was uploaded. If an invalid sensorIndex was given, the upload ends in the <IP>/upload_data page (white page)
            updateLastSensorMessages();
        }
        else                      // return an error message only if file was NOT opened before
        {
            request->send(200, "text/plain", "Upload failed: Invalid sensorIndex or file could not be written!");
        }
    }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void main_initWebserverEndpoints()
{
    server.on("/num_sensors", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(200, "text/plain", String(NUM_SUPPORTED_SENSORS));
    });

    // ----------------------------------

    server.on("/get_sensor_status", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        DynamicJsonDocument doc(1024);
        JsonArray sensors = doc.createNestedArray("sensors");
        for(int i = 0; i < NUM_SUPPORTED_SENSORS; i++)
        {
            JsonObject sensor = sensors.createNestedObject();
            sensor["index"] = i;
            if(sensor_messages_latest[i].timestamp != -1)
            {
                sensor["state"] = sensor_messages_latest[i].msg.pinState;
                sensor["voltage_mV"] = sensor_messages_latest[i].msg.batteryVoltage_mV;
                sensor["percentage"] = battery_voltageToPercent(sensor_messages_latest[i].msg.batteryVoltage_mV);
                time_t time = sensor_messages_latest[i].timestamp;
                tm tm_struct;
                localtime_r(&time, &tm_struct);
                char buffer[20];
                strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M", &tm_struct);
                sensor["timestamp"] = buffer;
            }
            else
            {
                sensor["state"] = false;
                sensor["voltage_mV"] = -1;
                sensor["percentage"] = 0;
                sensor["timestamp"] = "?";
            }
            char macStr[18];
            sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", sysConfig.sensors[i].mac[0], sysConfig.sensors[i].mac[1], sysConfig.sensors[i].mac[2], sysConfig.sensors[i].mac[3], sysConfig.sensors[i].mac[4], sysConfig.sensors[i].mac[5]);
            sensor["mac"] = macStr;
            sensor["name"] = sysConfig.sensors[i].name;
            
            // Add management data
            sensor["mode"] = sysConfig.sensors[i].mode;
            sensor["isPaired"] = sysConfig.sensors[i].isPaired;
            sensor["useEncryption"] = sysConfig.sensors[i].useEncryption;
            sensor["numMessages"] = memory_getNumberSensorMessages(i);
            
            if(sensor_messages_latest[i].timestamp == -1)
            {
                sensor["swVersion"] = "?";
            }
            else
            {
                uint8_t major = (sensor_messages_latest[i].msg.sensor_sw_version & 0xF0) >> 4;
                uint8_t minor = (sensor_messages_latest[i].msg.sensor_sw_version & 0x0F);
                sensor["swVersion"] = "v" + String(major) + "." + String(minor);
            }
        }
        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
    });

    // ----------------------------------

    server.on("/get_indoor_station_info", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        DynamicJsonDocument doc(256);
        //doc["mac"] = WiFi.macAddress();
        doc["swVersion"] = GARAGE_DOOR_INDOOR_STATION_SW_VERSION;
        doc["memoryUsage"] = memory_getMemoryUsageString(true);
        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
    });

    // ----------------------------------

    server.on("/get_system_time", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        time_t now;
        time(&now);
        tm tm_struct;
        localtime_r(&now, &tm_struct);

        char timeBuf[32];
        strftime(timeBuf, sizeof(timeBuf), "%d.%m.%Y %H:%M:%S", &tm_struct);

        request->send(200, "text/plain", String(timeBuf));
    });

    // ----------------------------------

    server.on("/set_sensor_name", HTTP_GET, [] (AsyncWebServerRequest *request)
    {
        int8_t sensorIndex = -1;
        String name;
        if(request->hasParam("sensorIndex"))
        {
            sensorIndex = request->getParam("sensorIndex")->value().toInt();
        }
        if(request->hasParam("name"))
        {
            name = request->getParam("name")->value();
        }

        if(sensorIndex >= 0 && sensorIndex < NUM_SUPPORTED_SENSORS)
        {
            strncpy(sysConfig.sensors[sensorIndex].name, name.c_str(), sizeof(sysConfig.sensors[sensorIndex].name) - 1);
            sysConfig.sensors[sensorIndex].name[sizeof(sysConfig.sensors[sensorIndex].name) - 1] = '\0';
            memory_saveSystemConfig(sysConfig);
        }
        request->redirect("/system_management.html");
    });

    // ----------------------------------

    server.on("/set_mac_sensor", HTTP_GET, [] (AsyncWebServerRequest *request)
    {
        String inputMessage;
        int8_t sensorIndex = -1;
        if(request->hasParam("sensorIndex"))
        {
            sensorIndex = request->getParam("sensorIndex")->value().toInt();
        }
        if(request->hasParam("mac"))
        {
            inputMessage = request->getParam("mac")->value();
        }

        if(sensorIndex >= 0 && sensorIndex < NUM_SUPPORTED_SENSORS && !inputMessage.isEmpty())
        {
            utils_parseMac(inputMessage.c_str(), sysConfig.sensors[sensorIndex].mac);
            sysConfig.sensors[sensorIndex].isPaired = true;         // If a MAC is set, we can assume that the sensor is paired.
            sysConfig.sensors[sensorIndex].useEncryption = false;   // Encryption is based on exchanged keys. This is not possible by simply entering the MAC.
            memory_saveSystemConfig(sysConfig);

            main_removePeer(sysConfig.sensors[sensorIndex]);        // Remove the peer (not needed for unencrypted communication)
        }

        request->redirect("/system_management.html");
    });

    // ----------------------------------

    // Send a GET request to <ESP_IP>/download_data
    server.on("/download_data", HTTP_GET, [] (AsyncWebServerRequest *request)
    {
        int8_t sensorIndex = -1;
        if(request->hasParam("sensorIndex"))
        {
            sensorIndex = request->getParam("sensorIndex")->value().toInt();
        }
        //Download data of the requested sensor
        char strBuf[32];
        sprintf(strBuf, FILENAME_HISTORY_SENSOR_FORMAT, sensorIndex);
        if(LittleFS.exists(strBuf))
        {
            request->send(LittleFS, strBuf, String(), true);
        }
        else
        {
            request->send(200, "text/plain", "No file with this name found: " + String(strBuf));
        }
    });

    // ----------------------------------

    // upload a file to /upload_data
    server.on("/upload_data", HTTP_POST, [](AsyncWebServerRequest *request)
    {
        /* Everything handled in onUpload() */
    }, onUpload);

    // ----------------------------------

    // Send a GET request to <ESP_IP>/remove_data
    server.on("/remove_data", HTTP_POST, [] (AsyncWebServerRequest *request)
    {
        String inputMessage;
        int8_t sensorIndex = -1;
        if(request->hasParam("sensorIndex", true))
        {
            sensorIndex = request->getParam("sensorIndex", true)->value().toInt();
        }
        memory_removeSensorHistory(sensorIndex);
        updateLastSensorMessages();
        request->redirect("/system_management.html");
    });

    // ----------------------------------

    // Send a GET request to <ESP_IP>/get_data
    server.on("/get_data", HTTP_GET, [] (AsyncWebServerRequest *request)
    {
        serverGetDataSensorIndex = -1;
        serverGetDataTimeFrom = 0;
        serverGetDataTimeTo = UINT32_MAX;
        if(request->hasParam("sensorIndex"))
        {
            serverGetDataSensorIndex = request->getParam("sensorIndex")->value().toInt();
        }
        if(request->hasParam("from"))
        {
            serverGetDataTimeFrom = request->getParam("from")->value().toInt();
        }
        if(request->hasParam("to"))
        {
            serverGetDataTimeTo = request->getParam("to")->value().toInt();
        }

        if(serverGetDataSensorIndex < 0 || serverGetDataSensorIndex >= NUM_SUPPORTED_SENSORS)
        {
            request->send(200, "text/plain", "sensorIndex parameter not set or out of range");
            return;
        }

        char strBuf[32];
        sprintf(strBuf, FILENAME_HISTORY_SENSOR_FORMAT, serverGetDataSensorIndex);
        serverGetDataMemoryFile = LittleFS.open(strBuf, "r");
        if(!serverGetDataMemoryFile)
        {
            request->send(200, "text/plain", "file open failed");
            return;
        }
        serverGetDataHasPendingMessage = false;

        AsyncWebServerResponse *response = request->beginChunkedResponse("text/plain", [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t 
        {
            //Write up to "maxLen" bytes into "buffer" and return the amount written.
            //Index equals the amount of bytes that have been already sent.
            //You will be asked for more data until 0 is returned.
            //Keep in mind that you can not delay or yield waiting for more data!

            size_t jsonSize = 0;
            StaticJsonDocument<75> json;
            while(jsonSize < maxLen)
            {
                // Get message (pending or from file)
                message_sensor_timestamped_t sensorMessage;
                if(serverGetDataHasPendingMessage)
                {
                    sensorMessage = serverGetDataPendingMessage;
                    serverGetDataHasPendingMessage = false;
                }
                else
                {
                    size_t numReadBytes = serverGetDataMemoryFile.read((uint8_t*)&sensorMessage, sizeof(message_sensor_timestamped_t));
                    if(numReadBytes == 0)
                    {
                        serverGetDataMemoryFile.close();
                        break;
                    }
                }

                // Skip the message if the timestamp is out of the requested range
                if(sensorMessage.timestamp < serverGetDataTimeFrom || sensorMessage.timestamp > serverGetDataTimeTo)
                {
                    continue;
                }

                // create a JSON document with the data
                json.clear();
                json["time"] = sensorMessage.timestamp;
                json["pin"]  = sensorMessage.msg.pinState;
                json["batP"] = battery_voltageToPercent(sensorMessage.msg.batteryVoltage_mV);

                size_t needed = measureJson(json);

                // Doesn't fit anymore -> remember for next chunk
                if(jsonSize + needed >= maxLen)
                {
                    serverGetDataPendingMessage = sensorMessage;
                    serverGetDataHasPendingMessage = true;
                    break;
                }

                // Write JSON to buffer
                jsonSize += serializeJson(json, buffer + jsonSize, maxLen - jsonSize);
            }
            return jsonSize;
        });
        request->send(response);
    });

    // ----------------------------------

    // Send a GET request to <ESP_IP>/set_sensor_mode
    server.on("/set_sensor_mode", HTTP_GET, [] (AsyncWebServerRequest *request)
    {
        int8_t sensorIndex = -1;
        SensorModes mode;
        if(request->hasParam("sensorIndex"))
        {
            sensorIndex = request->getParam("sensorIndex")->value().toInt();
        }
        if(request->hasParam("mode"))
        {
            mode = (SensorModes)request->getParam("mode")->value().toInt();
            setSensorMode(sensorIndex, mode);
        }
        request->redirect("/system_management.html");
    });

    // ----------------------------------

    server.on("/get_pairing_info", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        DynamicJsonDocument doc(256);
        doc["isPairingApActive"] = pairing_isAPOpen;
        if(pairing_isAPOpen)
        {
            doc["indoor_station_mac"] = WiFi.macAddress();
            doc["wifi_channel"] = WiFi.channel();
            doc["token"] = pairing_currentToken;

            char pmkHex[2 * ESPNOW_KEY_LEN + 1];
            utils_bytesToHex(sysConfig.pmk, ESPNOW_KEY_LEN, pmkHex);
            doc["pmk"] = pmkHex;

            int indexFirstSensorInPairingMode = pairing_findSensorIndexInPairingMode();
            doc["pairingSensorIndex"] = indexFirstSensorInPairingMode;

            char lmkHex[2 * ESPNOW_KEY_LEN + 1];
            utils_bytesToHex(sysConfig.sensors[indexFirstSensorInPairingMode].lmk, ESPNOW_KEY_LEN, lmkHex);
            doc["lmk"] = lmkHex;
        }
        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
    });
}

/**********************************************************************/

void setup()
{
    #ifdef DEBUG_OUTPUT
        Serial.begin(115200);
    #endif
    pinMode(LED_BUILTIN, OUTPUT);

    btn_reset.begin(BTN_RESET_PIN);   //INPUT_PULLUP
    btn_reset.setDebounceTime(100);
    btn_reset.setLongClickTime(1500);
    btn_reset.setLongClickDetectedHandler(btnHandler_reset_longClick);
    btn_pairing.begin(BTN_PAIRING_PIN);   //INPUT_PULLUP
    btn_pairing.setDebounceTime(100);
    btn_pairing.setLongClickTime(500);
    btn_pairing.setDoubleClickTime(400);
    btn_pairing.setLongClickDetectedHandler(btnHandler_pairing_longClick);
    btn_pairing.setClickHandler(btnHandler_pairing_click);
    
    leds.init();
    leds.setBrightness(12);
    leds_allOff();
    leds.start();

    // Begin LittleFS
    if (!LittleFS.begin())
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("An Error has occurred while mounting LittleFS");
        #endif
        return;
    }

    utils_initRandom();

    // Load system config or set defaults if not existing
    if(!memory_loadSystemConfig(sysConfig))
    {
        main_setDefaultSystemConfig(sysConfig);
        memory_saveSystemConfig(sysConfig);
    }
    main_makeSureEncryptionKeysAreSetInSystemConfig(sysConfig);
    main_updateLeds_sensorStatus();

    updateLastSensorMessages();

    #ifdef DEBUG_OUTPUT
        Serial.print("My MAC-Address: ");
        Serial.println(WiFi.macAddress());
    #endif

    timeHandling_init();

    leds_wifiConnecting();
    wifiHandling_init();

    if (esp_now_init() == 0) 
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("ESPNow Init success");
        #endif
    }
    else 
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("ESPNow Init fail");
        #endif
        return;
    }

    digitalWrite(LED_BUILTIN, HIGH);            // Turn off LED
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_recv_cb(messageReceived);
    esp_now_set_kok(sysConfig.pmk, ESPNOW_KEY_LEN);   // set the PMK for ESP-NOW communication. The PMK is used as a common key for all peers and is required for encryption. It must be set before adding any encrypted peers.
    
    for(int i = 0; i < NUM_SUPPORTED_SENSORS; i++)
    {
        sensor_config_t sensorConfig = sysConfig.sensors[i];
        if(sensorConfig.isPaired && sensorConfig.useEncryption)
        {
            main_addEncryptedPeer(sensorConfig);
        }
    }
}

/**********************************************************************/

void loop()
{
    wifiHandling_wifiManagerLoop();
    btn_reset.loop();
    btn_pairing.loop();
    leds.service();
    otaUpdate_loop();

    if(pairing_handlePairingAPTimeout())
    {
        // If the pairing AP timeout occurred, set all sensors that are in pairing mode back to normal mode.
        pairing_stopAllSensorsPairingMode();
        events.send("", SERVER_EVENT_SENSOR_PAIRING_TIMEOUT, millis());
        main_updateLeds_sensorStatus();
    }

    if(sensor_message_received)
    {
        sensor_message_received = false;

        #ifdef DEBUG_OUTPUT
            Serial.printf("Transmitter MAC Address: %02X:%02X:%02X:%02X:%02X:%02X \n\r", received_mac_addr[0], received_mac_addr[1], received_mac_addr[2], received_mac_addr[3], received_mac_addr[4], received_mac_addr[5]);
        #endif
        for(uint i = 0; i < NUM_SUPPORTED_SENSORS; i++)
        {  
            if(received_mac_addr[0] == sysConfig.sensors[i].mac[0] && 
                received_mac_addr[1] == sysConfig.sensors[i].mac[1] &&
                received_mac_addr[2] == sysConfig.sensors[i].mac[2] &&
                received_mac_addr[3] == sysConfig.sensors[i].mac[3] &&
                received_mac_addr[4] == sysConfig.sensors[i].mac[4] &&
                received_mac_addr[5] == sysConfig.sensors[i].mac[5] &&
                (sysConfig.sensors[i].mode == SENSOR_MODE_NORMAL || sysConfig.sensors[i].mode == SENSOR_MODE_ONLY_DISPLAY))
            {
                sensor_messages_latest[i].msg = sensor_message;
                time_t now;
                time(&now);
                sensor_messages_latest[i].timestamp = now;
                #ifdef DEBUG_OUTPUT
                    Serial.printf("Data received from Sensor %d \n\r", i);
                #endif
                timeHandling_printSerial(now);
              
                main_updateLeds_sensorStatus();

                if(sysConfig.sensors[i].mode == SENSOR_MODE_NORMAL)
                {
                    memory_addSensorMessage(i, sensor_messages_latest[i]);
                }
                events.send("", SERVER_EVENT_SENSOR_NEW_MESSAGE, millis());
                break;
            }
        }
    }
    else if(pairing_message_received)
    {
        pairing_message_received = false;

        #ifdef DEBUG_OUTPUT
            Serial.printf("Pairing message from MAC Address: %02X:%02X:%02X:%02X:%02X:%02X \n\r", received_mac_addr[0], received_mac_addr[1], received_mac_addr[2], received_mac_addr[3], received_mac_addr[4], received_mac_addr[5]);
        #endif

        // Check if the received MAC address matches the sensorMac in the pairing message payload. If not, ignore the message.
        // This prevents that random pairing messages from other devices are processed, which could cause issues (e.g. if the sender MAC matches a sensor that is currently in pairing mode, but the sender is not the actual sensor trying to pair).
        if(memcmp(received_mac_addr, pairing_message.sensorMac, sizeof(received_mac_addr)) != 0)
        {
            // The sender MAC does not match the sensorMac in the payload, ignore the message.
            #ifdef DEBUG_OUTPUT
                Serial.println("Pairing message ignored: sender MAC does not match sensorMac in payload.");
            #endif
            return;
        }
        if(pairing_message.pairingToken != pairing_currentToken)
        {
            // The token in the payload does not match the current pairing token, ignore the message. This prevents that old pairing messages are processed, which could cause issues (e.g. if the sender is an old sensor that was in pairing mode before but is now trying to pair again, the old message with the old token could be received after the new token is generated and processed, which would cause that the new sensor is paired with the old message).
            #ifdef DEBUG_OUTPUT
                Serial.println("Pairing message ignored: token in payload does not match current pairing token.");
            #endif
            return;
        }

        for(int sensorIndex = 0; sensorIndex < NUM_SUPPORTED_SENSORS; sensorIndex++)
        {
            if(sysConfig.sensors[sensorIndex].mode == SENSOR_MODE_PAIRING)
            {
                // save the received MAC address for the sensor which is in pairing mode and reset the sensor mode back to normal
                memcpy(sysConfig.sensors[sensorIndex].mac, received_mac_addr, 6 * sizeof(uint8_t));
                sysConfig.sensors[sensorIndex].isPaired = true;
                sysConfig.sensors[sensorIndex].useEncryption = true;
                memory_saveSystemConfig(sysConfig);

                pairing_disablePairingModeForSensor(sensorIndex);

                main_addEncryptedPeer(sysConfig.sensors[sensorIndex]);

                events.send("", SERVER_EVENT_SENSOR_PAIRED, millis());
                break;
            }
        }
    }
}
