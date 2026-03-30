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
#include "config.h"
#include "structures.h"
#include "leds.h"
#include "battery.h"
#include "wifiHandling.h"
#include "timeHandling.h"
#include "otaUpdate.h"
#include "memory.h"
#include "version.h"

// To increase the FS size https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#flash-layout

uint8_t sensor_macs[NUM_SUPPORTED_SENSORS][6];
Button2 btn_pairing, btn_reset;             // create button objects

message_sensor_t sensor_message;
bool sensor_message_received = false;
bool pairing_message_received = false;
uint8_t received_mac_addr[6];

message_sensor_timestamped_t sensor_messages_latest[NUM_SUPPORTED_SENSORS];
SensorModes sensor_modes[NUM_SUPPORTED_SENSORS];

File serverGetDataMemoryFile;
int8_t serverGetDataSensorIndex;
time_t serverGetDataTimeFrom;
time_t serverGetDataTimeTo;
message_sensor_timestamped_t serverGetDataPendingMessage;
bool serverGetDataHasPendingMessage = false;

/**********************************************************************/

void updateLeds_sensorStatus()
{
  for(uint8_t i = 0; i < NUM_SUPPORTED_SENSORS; i++)
  {
    switch(sensor_modes[i])
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

  updateLeds_sensorStatus();
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

  if(magicNumber == PAIRING_MAGIC_NUMBER)
  {
    pairing_message_received = true;
  }
  else
  {
    memcpy(&sensor_message, data, sizeof(sensor_message));
    sensor_message_received = true;
  }
}

/**********************************************************************/

void setSensorMode(uint8_t sensorIndex, SensorModes mode)
{
  if(sensorIndex >= 0 && sensorIndex < NUM_SUPPORTED_SENSORS)
  {
    sensor_modes[sensorIndex] = mode;
    updateLeds_sensorStatus();
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

  uint8_t numSensorInPairingMode = 0;
  for(int sensorIndex = 0; sensorIndex < NUM_SUPPORTED_SENSORS; sensorIndex++)
  {
    if(sensor_modes[sensorIndex] == SENSOR_MODE_PAIRING)
    {
      setSensorMode(sensorIndex, SENSOR_MODE_NORMAL);
      numSensorInPairingMode++;
    }
  }
  if(numSensorInPairingMode == 0)
  {
    setSensorMode(0, SENSOR_MODE_PAIRING);
  }
}

void btnHandler_pairing_click(Button2& btn)
{
  // button is pressed short:
  // - if no sensor is in pairing mode, do nothing
  // - otherwise find the index of the first sensor with pairing mode. Set it to normal mode and set the next sensor to pairing mode. If it was the last sensor begin with the first again.

  int indexFirstSensorInPairingMode = -1;
  for(int sensorIndex = 0; sensorIndex < NUM_SUPPORTED_SENSORS; sensorIndex++)
  {
    if(sensor_modes[sensorIndex] == SENSOR_MODE_PAIRING)
    {
      indexFirstSensorInPairingMode = sensorIndex;
      break;
    }
  }

  if(indexFirstSensorInPairingMode != -1)
  {
    setSensorMode(indexFirstSensorInPairingMode, SENSOR_MODE_NORMAL);
    int indexNextSensor = (indexFirstSensorInPairingMode + 1) % NUM_SUPPORTED_SENSORS;  // increment to the next sensor and roll over at the last sensor.
    setSensorMode(indexNextSensor, SENSOR_MODE_PAIRING);
  }
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

void initWebserverFiles()
{
   // Route for root index.html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/index.html", "text/html", false); 
  });
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/index.html", "text/html", false); 
  });

  // Route for root sensor_history.html
  server.on("/sensor_history.html", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/sensor_history.html", "text/html", false); 
  });

  // Route for root system_management.html
  server.on("/system_management.html", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/system_management.html", "text/html", false); 
  });

  // Route for root style.css
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/style.css", "text/css"); 
  });

  // Route for root utils.js
  server.on("/utils.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/utils.js", "text/javascript"); 
  });

  // Route for root index.js
  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/index.js", "text/javascript"); 
  });

  // Route for root sensor_history.js
  server.on("/sensor_history.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/sensor_history.js", "text/javascript"); 
  });

  // Route for root system_management.js
  server.on("/system_management.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/system_management.js", "text/javascript"); 
  });

  // Route for root version.js
  server.on("/version.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/version.js", "text/javascript"); 
  });

  server.onNotFound([](AsyncWebServerRequest *request)
  {
    request->send(404, "text/plain", "Not found");
  });

  // ----------------------------------

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
      sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", sensor_macs[i][0], sensor_macs[i][1], sensor_macs[i][2], sensor_macs[i][3], sensor_macs[i][4], sensor_macs[i][5]);
      sensor["mac"] = macStr;
      
      // Add management data
      sensor["mode"] = sensor_modes[i];
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
    doc["mac"] = WiFi.macAddress();
    doc["swVersion"] = GARAGE_DOOR_INDOOR_STATION_SW_VERSION;
    doc["memoryUsage"] = memory_getMemoryUsageString();
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

  server.on("/set_mac_sensor", HTTP_GET, [] (AsyncWebServerRequest *request)
  {
    String inputMessage;
    int8_t sensorIndex = -1;
    uint tmp_mac[6];
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
      sscanf(inputMessage.c_str(), "%02X:%02X:%02X:%02X:%02X:%02X", &tmp_mac[0], &tmp_mac[1], &tmp_mac[2], &tmp_mac[3], &tmp_mac[4], &tmp_mac[5]);
      for(uint8_t i = 0; i < 6; i++)
      {
        sensor_macs[sensorIndex][i] = tmp_mac[i];
      }

      // Save new sensor_macs
      memory_saveSensorMacs(sensor_macs);
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

  // Load saved sensor_macs
  memcpy(sensor_macs, memory_getSensorMacs().macs, sizeof(sensor_macs));

  // Reset all sensor modes to Normal
  for(uint8_t i = 0; i < NUM_SUPPORTED_SENSORS; i++)
  {
    setSensorMode(i, SENSOR_MODE_NORMAL);
  }

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
}

/**********************************************************************/

void loop()
{
  wifiHandling_wifiManagerLoop();
  btn_reset.loop();
  btn_pairing.loop();
  leds.service();
  otaUpdate_loop();

  if(sensor_message_received)
  {
    sensor_message_received = false;

    #ifdef DEBUG_OUTPUT
      Serial.printf("Transmitter MAC Address: %02X:%02X:%02X:%02X:%02X:%02X \n\r", received_mac_addr[0], received_mac_addr[1], received_mac_addr[2], received_mac_addr[3], received_mac_addr[4], received_mac_addr[5]);
    #endif
    for(uint i = 0; i < ARRAY_ELEMENT_COUNT(sensor_macs); i++)
    {  
      if(received_mac_addr[0] == sensor_macs[i][0] && 
          received_mac_addr[1] == sensor_macs[i][1] &&
          received_mac_addr[2] == sensor_macs[i][2] &&
          received_mac_addr[3] == sensor_macs[i][3] &&
          received_mac_addr[4] == sensor_macs[i][4] &&
          received_mac_addr[5] == sensor_macs[i][5] &&
          (sensor_modes[i] == SENSOR_MODE_NORMAL || sensor_modes[i] == SENSOR_MODE_ONLY_DISPLAY))
      {
        sensor_messages_latest[i].msg = sensor_message;
        time_t now;
        time(&now);
        sensor_messages_latest[i].timestamp = now;
        #ifdef DEBUG_OUTPUT
          Serial.printf("Data received from Sensor %d \n\r", i);
        #endif
        timeHandling_printSerial(now);
        
        updateLeds_sensorStatus();

        if(sensor_modes[i] == SENSOR_MODE_NORMAL)
        {
          memory_addSensorMessage(i, sensor_messages_latest[i]);
        }
        
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

    for(int sensorIndex = 0; sensorIndex < NUM_SUPPORTED_SENSORS; sensorIndex++)
    {
      if(sensor_modes[sensorIndex] == SENSOR_MODE_PAIRING)
      {
        // save the received MAC address for the sensor which is in pairing mode and reset the sensor mode back to normal
        for(uint8_t i = 0; i < 6; i++)
        {
          sensor_macs[sensorIndex][i] = received_mac_addr[i];
        }
        memory_saveSensorMacs(sensor_macs);
        setSensorMode(sensorIndex, SENSOR_MODE_NORMAL);
        break;
      }
    }
  }
}