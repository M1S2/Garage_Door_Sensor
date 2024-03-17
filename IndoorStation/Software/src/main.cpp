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

uint8_t sensor_macs[NUM_SUPPORTED_SENSORS][6];
Button2 btn_show_status, btn_wifi, btn_reset;             // create button objects

message_sensor_t sensor_message;
bool sensor_message_received = false;
uint8_t sensor_message_received_mac_addr[6];

message_sensor_timestamped_t sensor_messages_latest[NUM_SUPPORTED_SENSORS];

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
  wifiHandling_eraseCredentials();
  delay(1000);
  ESP.restart();
}

void btnHandler_show_status_click(Button2& btn)
{
  Serial.println("Show Status Click (used to emulate sensor message for the first sensor for the moment)");

  message_sensor_t emulated_message;
  emulated_message.pinState = random(0,2) == 0 ? false : true;
  emulated_message.batteryVoltage_mV = 3141 + random(0, 1000);
  emulated_message.numberSendLoops = 42;
  messageReceived((uint8_t*)&sensor_macs[0], (uint8_t*)&emulated_message, 0 /* len not used */);
}

void btnHandler_show_status_longClick(Button2& btn)
{  
  Serial.println("Clear the memory (for testing purposes!)");
  memory_reset();
  // invalidate all last sensor messages
  for(uint8_t i=0; i < NUM_SUPPORTED_SENSORS; i++)
  {
    sensor_messages_latest[i].timestamp = -1;
  }
  
  updateLeds_sensorStatus();
}

void btnHandler_wifi_click(Button2& btn)
{
  Serial.println("Wifi Click (used to emulate sensor message for the second sensor for the moment)");

  message_sensor_t emulated_message;
  emulated_message.pinState = random(0,2) == 0 ? false : true;
  emulated_message.batteryVoltage_mV = 2700 + random(0, 500);
  emulated_message.numberSendLoops = 12;
  messageReceived((uint8_t*)&sensor_macs[1], (uint8_t*)&emulated_message, 0 /* len not used */);
}

void btnHandler_wifi_longClick(Button2& btn)
{
  Serial.println("Wifi long Click, show all memory data (only for testing!)");
  memory_showMemoryContent();
}

/**********************************************************************/

String processor(const String& var)
{
    if(var == "INDOOR_STATION_MAC")
    {
        return WiFi.macAddress();
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

  // Route for root sensor_info.html
  server.on("/sensor_info.html", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/sensor_info.html", "text/html", false, processor); 
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

  // Route for root sensor_info.js
  server.on("/sensor_info.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/sensor_info.js", "text/javascript"); 
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
    if(request->hasParam("mac_sensor_1"))
    {
      sensorIndex = 0;
      inputMessage = request->getParam("mac_sensor_1")->value();
    }
    else if(request->hasParam("mac_sensor_2"))
    {
      sensorIndex = 1;
      inputMessage = request->getParam("mac_sensor_2")->value();
    }

    if(sensorIndex >= 0 && sensorIndex < NUM_SUPPORTED_SENSORS)
    {
      sscanf(inputMessage.c_str(), "%02X:%02X:%02X:%02X:%02X:%02X", &tmp_mac[0], &tmp_mac[1], &tmp_mac[2], &tmp_mac[3], &tmp_mac[4], &tmp_mac[5]);
      for(uint8_t i = 0; i < 6; i++)
      {
        sensor_macs[sensorIndex][i] = tmp_mac[i];
      }

      // Save new sensor_macs
      memory_saveSensorMacs(sensor_macs);
    }

    request->send(LittleFS, "/sensor_info.html", "text/html", false, processor); 
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

void updateWebsiteSensorHistory()
{     
  events_sensorHistory.send("", "clear_sensorHistory", millis());

  for(int sensorIdx = 0; sensorIdx < NUM_SUPPORTED_SENSORS; sensorIdx++)
  {
    uint16_t numberMessages = memory_getNumberSensorMessages(sensorIdx);
    message_sensor_timestamped_t sensorMessages[numberMessages];
    memory_getSensorMessagesForSensor(sensorIdx, sensorMessages);
    for(int msgIdx = 0; msgIdx < numberMessages; msgIdx++)
    {
      // create a JSON document with the data and send it by event to the web page
      StaticJsonDocument<1000> root;
      String payload;
      root["sensorId"] = sensorIdx;
      root["timestamp"] = sensorMessages[msgIdx].timestamp;
      root["pinState"] = sensorMessages[msgIdx].msg.pinState;
      root["batteryVoltage_mV"] = sensorMessages[msgIdx].msg.batteryVoltage_mV;
      root["batteryPercentage"] = battery_voltageToPercent(sensorMessages[msgIdx].msg.batteryVoltage_mV);
      serializeJson(root, payload);
      Serial.printf("sensor history event send: %s\n", payload.c_str());
      events_sensorHistory.send(payload.c_str(), "new_sensorHistory", millis());
    }
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
  btn_show_status.setClickHandler(btnHandler_show_status_click);
  btn_show_status.setLongClickDetectedHandler(btnHandler_show_status_longClick);
  btn_wifi.begin(BTN_WIFI_PIN);   //INPUT_PULLUP
  btn_wifi.setDebounceTime(100);
  btn_wifi.setLongClickTime(500);
  btn_wifi.setDoubleClickTime(400);
  btn_wifi.setClickHandler(btnHandler_wifi_click);
  btn_wifi.setLongClickDetectedHandler(btnHandler_wifi_longClick);

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

  // invalidate all last sensor messages and get the saved ones
  for(uint8_t i=0; i < NUM_SUPPORTED_SENSORS; i++)
  {
    sensor_messages_latest[i].timestamp = -1;

    if(memory_getNumberSensorMessages(i) > 0)
    {
      sensor_messages_latest[i] = memory_getLatestSensorMessagesForSensor(i);
    }
  }

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

  updateLeds_sensorStatus();
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