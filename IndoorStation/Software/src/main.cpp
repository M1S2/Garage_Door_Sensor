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
Button2 btn_show_status, btn_wifi, btn_reset;             // create button objects

message_sensor_t sensor_message;
bool sensor_message_received = false;
uint8_t sensor_message_received_mac_addr[6];

message_sensor_timestamped_t sensor_messages_latest[NUM_SUPPORTED_SENSORS];

File serverGetChartDataMemoryFile;
int8_t serverGetCharDataSensorIndex;

/**********************************************************************/

void updateLeds_sensorStatus()
{
  for(uint8_t i = 0; i < NUM_SUPPORTED_SENSORS; i++)
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
  memcpy(&sensor_message, data, sizeof(sensor_message));
  memcpy(&sensor_message_received_mac_addr, mac_addr, sizeof(sensor_message_received_mac_addr));
  sensor_message_received = true;
}

/**********************************************************************/

void btnHandler_reset_longClick(Button2& btn)
{
  memory_removeAllData();
  wifiHandling_eraseCredentials();
  delay(1000);
  ESP.restart();
}

void btnHandler_show_status_longClick(Button2& btn)
{
  Serial.println("Show all memory data (only for testing!)");
  memory_showMemoryContent();
}

/**********************************************************************/

String processor(const String& var)
{
    if(var == "INDOOR_STATION_MAC")
    {
        return WiFi.macAddress();
    }
    else if(var == "MEMORY_USAGE")
    {
      return memory_getMemoryUsageString();
    }
    else if(var == "SW_VERSION")
    {
      return GARAGE_DOOR_INDOOR_STATION_SW_VERSION;
    }
    else if(var.startsWith("MAC_SENSOR"))
    {
        String sensorIndexStr = var;
        sensorIndexStr.replace("MAC_SENSOR","");
        long sensorIndex = sensorIndexStr.toInt() - 1;
        if(sensorIndex >= NUM_SUPPORTED_SENSORS)
        {
            return "MAC_SENSOR index to large!";
        }
        else
        {
            char macAddrStr[18];
            sprintf(macAddrStr, "%02X:%02X:%02X:%02X:%02X:%02X", sensor_macs[sensorIndex][0], sensor_macs[sensorIndex][1], sensor_macs[sensorIndex][2], sensor_macs[sensorIndex][3], sensor_macs[sensorIndex][4], sensor_macs[sensorIndex][5]);
            return macAddrStr;
        }
    }
    else if(var.startsWith("NUM_MESSAGES_"))
    {
        String sensorIndexStr = var;
        sensorIndexStr.replace("NUM_MESSAGES_","");
        long sensorIndex = sensorIndexStr.toInt() - 1;
        if(sensorIndex >= NUM_SUPPORTED_SENSORS)
        {
            return "NUM_MESSAGES_ index to large!";
        }
        else
        {
            return String(memory_getNumberSensorMessages(sensorIndex));
        }
    }
    return String();
}

void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if (!index)
  {
    // Set this variable to true, if the given filename is ok to be uploaded.
    // At the moment only the sensor history data files are allowed for the number of supported sensors.
    bool fileAllowedToUpload = false;
    for(int i = 0; i < NUM_SUPPORTED_SENSORS; i++)
    {
      char strBuf[32];
      sprintf(strBuf, FILENAME_HISTORY_SENSOR_FORMAT, i);
      if("/" + filename == strBuf)
      {
        fileAllowedToUpload = true;
      }
    }

    if(fileAllowedToUpload)
    {
      // open the file (by the requested filename) on first call and store the file handle in the request object
      request->_tempFile = LittleFS.open("/" + filename, "w");
    }
  }

  if (len && request->_tempFile)    // last part is to check that file was opened
  {
    // stream the incoming chunk to the opened file
    request->_tempFile.write(data, len);
  }

  if (final && request->_tempFile)    // last part is to check that file was opened
  {
    // close the file handle as the upload is now done
    request->_tempFile.close();
    request->redirect("/system_management.html");     // Only redirect if the file was uploaded. If a not allowed file was given, the upload ends in the <IP>/upload_data page (white page)

    updateLastSensorMessages();
  }
}

void initWebserverFiles()
{
   // Route for root index.html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/index.html", "text/html"); 
  });
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/index.html", "text/html"); 
  });

  // Route for root sensor_history.html
  server.on("/sensor_history.html", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/sensor_history.html", "text/html"); 
  });

  // Route for root system_management.html
  server.on("/system_management.html", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/system_management.html", "text/html", false, processor); 
  });

  // Route for root style.css
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/style.css", "text/css"); 
  });

  // Route for root common.js
  server.on("/common.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/common.js", "text/javascript"); 
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

  server.onNotFound([](AsyncWebServerRequest *request)
  {
    request->send(404, "text/plain", "Not found");
  });

  // Send a GET request to <ESP_IP>/mac_sensor
  server.on("/mac_sensor", HTTP_GET, [] (AsyncWebServerRequest *request)
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

  // Send a GET request to <ESP_IP>/download_data_sensor1
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
    request->send(LittleFS, strBuf, String(), true);
  });

  // upload a file to /upload_data
  server.on("/upload_data", HTTP_POST, [](AsyncWebServerRequest *request)
  {
    request->send(200);
  }, onUpload);

  // Send a GET request to <ESP_IP>/remove_data
  server.on("/remove_data", HTTP_GET, [] (AsyncWebServerRequest *request)
  {
    String inputMessage;
    int8_t sensorIndex = -1;
    if(request->hasParam("sensorIndex"))
    {
      sensorIndex = request->getParam("sensorIndex")->value().toInt();
    }
    memory_removeSensorHistory(sensorIndex);
    updateLastSensorMessages();
    request->redirect("/system_management.html");
  });

  server.on("/get_data", HTTP_GET, [] (AsyncWebServerRequest *request)
  {
    serverGetCharDataSensorIndex = -1;
    if(request->hasParam("sensorIndex"))
    {
      serverGetCharDataSensorIndex = request->getParam("sensorIndex")->value().toInt();
    }

    AsyncWebServerResponse *response = request->beginChunkedResponse("text/plain", [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t 
    {
      //Write up to "maxLen" bytes into "buffer" and return the amount written.
      //index equals the amount of bytes that have been already sent
      //You will be asked for more data until 0 is returned
      //Keep in mind that you can not delay or yield waiting for more data!

      if(index == 0)
      {
        char strBuf[32];
	      sprintf(strBuf, FILENAME_HISTORY_SENSOR_FORMAT, serverGetCharDataSensorIndex);
	      serverGetChartDataMemoryFile = LittleFS.open(strBuf, "r");
      }

      StaticJsonDocument<75> jsonRoot;
      uint8_t maxLenPerJsonMsg = jsonRoot.capacity();
      size_t jsonSize = 0;
      for(uint i = 0; i < (maxLen / maxLenPerJsonMsg) - 1; i++)
      {
        message_sensor_timestamped_t sensorMessage;
        size_t numReadBytes = serverGetChartDataMemoryFile.read((uint8_t*)&sensorMessage, sizeof(message_sensor_timestamped_t));
        if(numReadBytes == 0 && i == 0)
        {
          serverGetChartDataMemoryFile.close();
          return 0;
        }
        else if(numReadBytes == 0)
        {
          break;
        }

        // create a JSON document with the data and send it to the web page
        jsonRoot.clear();
        jsonRoot["time"] = sensorMessage.timestamp;
        jsonRoot["pin"] = sensorMessage.msg.pinState;
        jsonRoot["batP"] = battery_voltageToPercent(sensorMessage.msg.batteryVoltage_mV);
        uint8_t buf[maxLenPerJsonMsg];
        size_t jsonSizeMsg = serializeJson(jsonRoot, buf, maxLenPerJsonMsg);
        memcpy(buffer + jsonSize, buf, jsonSizeMsg);
        jsonSize += jsonSizeMsg;
      }
      return jsonSize;
    });
    request->send(response);
  });
}

void updateWebsiteMainForSensor(uint8_t sensor_id, message_sensor_timestamped_t sensor_message_timestamped)
{
  if(sensor_message_timestamped.timestamp != -1)
  {
    // create a JSON document with the data and send it by event to the web page
    StaticJsonDocument<1000> root;
    String payload;
    root["id"] = sensor_id;
    root["batteryVoltage_V"] = (sensor_message_timestamped.msg.batteryVoltage_mV / 1000.0f);
    root["batteryPercentage"] = battery_voltageToPercent(sensor_message_timestamped.msg.batteryVoltage_mV);
    root["pinState"] = sensor_message_timestamped.msg.pinState;
    root["timestamp"] = sensor_message_timestamped.timestamp;
    serializeJson(root, payload);
    Serial.printf("event send: %s\n", payload.c_str());
    events_dashboard.send(payload.c_str(), "new_readings", millis());
  }
}

void updateWebsiteMain()
{
  for(uint8_t i=0; i < NUM_SUPPORTED_SENSORS; i++)
  {
    updateWebsiteMainForSensor(i, sensor_messages_latest[i]);
  }
}

/**********************************************************************/

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  btn_reset.begin(BTN_RESET_PIN);   //INPUT_PULLUP
  btn_reset.setDebounceTime(100);
  btn_reset.setLongClickTime(1500);
  btn_reset.setLongClickDetectedHandler(btnHandler_reset_longClick);
  btn_show_status.begin(BTN_SHOW_STATUS_PIN);   //INPUT_PULLUP
  btn_show_status.setDebounceTime(100);
  btn_show_status.setLongClickTime(500);
  btn_show_status.setDoubleClickTime(400);
  btn_show_status.setLongClickDetectedHandler(btnHandler_show_status_longClick);
  //btn_wifi.begin(BTN_WIFI_PIN);   //INPUT_PULLUP
  //btn_wifi.setDebounceTime(100);
  //btn_wifi.setLongClickTime(500);
  //btn_wifi.setDoubleClickTime(400);
  //btn_wifi.setClickHandler(btnHandler_wifi_click);
  
  leds.init();
  leds.setBrightness(12);
  leds_allOff();
  leds.start();

  // Begin LittleFS
  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // Load saved sensor_macs
  memcpy(sensor_macs, memory_getSensorMacs().macs, sizeof(sensor_macs));

  updateLastSensorMessages();

  Serial.print("My MAC-Address: ");
  Serial.println(WiFi.macAddress());

  timeHandling_init();

  leds_wifiConnecting();
  wifiHandling_init();

  if (esp_now_init() == 0) 
  {
    Serial.println("ESPNow Init success");
  }
  else 
  {
    Serial.println("ESPNow Init fail");
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
  btn_show_status.loop();
  btn_wifi.loop();
  leds.service();
  otaUpdate_loop();

  if(sensor_message_received)
  {
    sensor_message_received = false;

    Serial.printf("Transmitter MAC Address: %02X:%02X:%02X:%02X:%02X:%02X \n\r", sensor_message_received_mac_addr[0], sensor_message_received_mac_addr[1], sensor_message_received_mac_addr[2], sensor_message_received_mac_addr[3], sensor_message_received_mac_addr[4], sensor_message_received_mac_addr[5]);    
    int sensor_id = -1;
    for(uint i = 0; i < ARRAY_ELEMENT_COUNT(sensor_macs); i++)
    {  
      if(sensor_message_received_mac_addr[0] == sensor_macs[i][0] && 
          sensor_message_received_mac_addr[1] == sensor_macs[i][1] &&
          sensor_message_received_mac_addr[2] == sensor_macs[i][2] &&
          sensor_message_received_mac_addr[3] == sensor_macs[i][3] &&
          sensor_message_received_mac_addr[4] == sensor_macs[i][4] &&
          sensor_message_received_mac_addr[5] == sensor_macs[i][5])
      {
        sensor_id = i;
        sensor_messages_latest[i].msg = sensor_message;
        time_t now;
        time(&now);
        sensor_messages_latest[i].timestamp = now;
        Serial.printf("Data received from Sensor %d \n\r", sensor_id);
        timeHandling_printSerial(now);

        updateWebsiteMainForSensor(sensor_id, sensor_messages_latest[i]);
        
        bool isOpen = (sensor_message.pinState == SENSOR_PIN_STATE_OPEN);
        leds_sensorStatus(i, isOpen, battery_isEmpty(sensor_message.batteryVoltage_mV));

        memory_addSensorMessage(i, sensor_messages_latest[i]);

        break;  
      }
    }

    if (sensor_id == -1)
    {
      Serial.println("Data received from unknown Sensor");
    }
    
    Serial.print("PinState: ");
    Serial.println(sensor_message.pinState);
    Serial.print("Battery Voltage mV: ");
    Serial.println(sensor_message.batteryVoltage_mV);
    Serial.print("NumberSendLoops: ");
    Serial.println(sensor_message.numberSendLoops);
  }
}