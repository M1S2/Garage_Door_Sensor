// This is the code that receives all garage door states
// It differentiates between the sensors by their MAC addresses

// Add a file called "addresses.h" beside this "main.cpp" file that contains a line for each sensor that should be used with the following content:
// #define SENSOR1_MAC {0x01, 0x02, 0x03, 0x04, 0x05, 0x06}     // 01:02:03:04:05:06, replace this with the MAC address of the sensor ESP of the first sensor
// #define SENSOR_MACS { SENSOR1_MAC }

// https://wolles-elektronikkiste.de/esp-now#multi_transm_1_recv  (Ein Transmitter, ein Receiver – Advanced (ESP32))
// https://randomnerdtutorials.com/esp-now-auto-pairing-esp32-esp8266/

#include <Arduino.h>
#include <espnow.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ESPAsyncWiFiManager.h>
#include <Button2.h>
#include <Schedule.h>
#include "leds.h"
#include "battery.h"
#include "timeHandling.h"
#include "sensorPairing.h"
#include "addresses.h"

AsyncWebServer server(80);
AsyncEventSource events("/events");
DNSServer dns;
AsyncWiFiManager wifiManager(&server,&dns);

// https://forum.arduino.cc/t/finding-the-size-of-multi-dimensional-array/395465/8
#define ARRAY_ELEMENT_COUNT(array) (sizeof array / sizeof array[0])

uint8_t sensor_macs[][6] = SENSOR_MACS;

#define BTN_SHOW_STATUS_PIN     12                        // The pin which is used for the show status button
#define BTN_WIFI_PIN            13                        // The pin which is used for the Wifi button
#define BTN_RESET_PIN           16                        // The pin which is used for the reset button
Button2 btn_show_status, btn_wifi, btn_reset;             // create button objects

#define SENSOR_PIN_STATE_OPEN   LOW                       // This pin state of the sensor is interpreted as open door
#define NUM_SUPPORTED_SENSORS   2                         // The number of supported sensors

#define CONNECTION_TIMEOUT_MS   10000                     // Timeout in ms for connection to router
#define CONFIGURATION_AP_NAME   "Garage Door Sensor"      // Name for the configuration access point
#define CONFIGURATION_AP_PW     ""                        // Password for the configuration access point

// Packing of this structure reduces the size to 4 bytes
PACK_STRUCT_BEGIN
typedef struct message_sensor
{
  PACK_STRUCT_FIELD(bool pinState);
  PACK_STRUCT_FIELD(uint16_t batteryVoltage_mV);
  PACK_STRUCT_FLD_8(uint8_t numberSendLoops);
}PACK_STRUCT_STRUCT message_sensor_t;
PACK_STRUCT_END

PACK_STRUCT_BEGIN
typedef struct message_sensor_timestamped
{
  PACK_STRUCT_FIELD(message_sensor_t msg);
  PACK_STRUCT_FIELD(time_t timestamp);
}PACK_STRUCT_STRUCT message_sensor_timestamped_t;
PACK_STRUCT_END

message_sensor_t sensor_message;
bool sensor_message_received = false;
uint8_t sensor_message_received_mac_addr[6];

message_sensor_timestamped_t sensor_messages_latest[NUM_SUPPORTED_SENSORS];

// The event handlers are initialized in the setup()
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

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

void updateWebsiteForSensor(uint8_t sensor_id, message_sensor_timestamped_t sensor_message_timestamped)
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
    events.send(payload.c_str(), "new_readings", millis());
  }
}

void updateWebsite()
{
  for(uint8_t i=0; i < NUM_SUPPORTED_SENSORS; i++)
  {
    updateWebsiteForSensor(i, sensor_messages_latest[i]);
  }
}

void messageReceived(uint8_t* mac_addr, uint8_t* data, uint8 len)
{
  memcpy(&sensor_message, data, sizeof(sensor_message));
  memcpy(&sensor_message_received_mac_addr, mac_addr, sizeof(sensor_message_received_mac_addr));
  sensor_message_received = true;
}

/**********************************************************************/

String processor(const String& var)
{
  if(var == "PAIRING_SENSOR_NUMBER")
  {
      switch(sensorPairingMode)
      {
        case PAIRING_MODE_SENSOR1: return "1";
        case PAIRING_MODE_SENSOR2: return "2";
        default: return "NONE";       // This will never be visible (hidden by the PAIRING_STATUS_DISPLAY_STYLE placeholder)
      }
  }
  else if(var == "PAIRING_STATUS_DISPLAY_STYLE")
  {
    return (sensorPairingMode == PAIRING_MODE_NONE) ? "style=\"display: none\"" : "style=\"display: block\"";
  }
  else if(var == "INDOOR_STATION_MAC")
  {
    return WiFi.macAddress();
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

  // Route for root accu_chart.html
  server.on("/accu_chart.html", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/accu_chart.html", "text/html"); 
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

  // Route for root accu_chart.js
  server.on("/accu_chart.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/accu_chart.js", "text/javascript"); 
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

  // Send a GET request to <ESP_IP>/pairing?sensor=<sensorIndex> or pairing?abort
  server.on("/pairing", HTTP_GET, [] (AsyncWebServerRequest *request)
  {
    String inputMessage;
    if (request->hasParam("abort"))
    {
      sensorPairing_Stop(&events);
      updateLeds_sensorStatus();
    }
    else if(request->hasParam("sensor"))
    {
      inputMessage = request->getParam("sensor")->value();
      sensorPairing_Start(inputMessage.toInt() - 1, &events);
    }
    request->send(LittleFS, "/sensor_info.html", "text/html", false, processor); 
  });
}

void wifiEraseCredentials() 
{
  WiFi.disconnect(true);
  ESP.eraseConfig();
  Serial.println("WiFi credentials erased");
}

void wifiManagerSaveCB()
{
  WiFi.softAPdisconnect(true);
  WiFi.persistent(false);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin();

  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  // https://randomnerdtutorials.com/solved-reconnect-esp8266-nodemcu-to-wifi/
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  initWebserverFiles();
  // events 
  events.onConnect([](AsyncEventSourceClient *client)
  {
    updateWebsite();      // Update the values on the website when it is opened (or reloaded in the browser)
  });
  server.addHandler(&events);
  // start webserver
  server.begin();

  leds_wifiConnected();
}

void wifiManagerAPOpenedCB(AsyncWiFiManager* manager)
{
  WiFi.persistent(true);
  leds_wifiAPOpen();
  manager->setConnectTimeout(CONNECTION_TIMEOUT_MS / 1000);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) 
{
  // https://github.com/esp8266/Arduino/issues/5722
  // The wifi callbacks execute in the SYS context, and you can't yield/delay in there.
  // If you need to do that, I suggest to use the wifi callback to schedule another callback with your code that requires yield/delay.
  // Scheduled functions execute as though they were called from the loop, I.e. in CONT context.
  // Same applies to the onWifiDisconnect event
  schedule_function(leds_wifiConnected);
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) 
{
  // This function gets called cyclic as long as the Wifi is disconnected
  schedule_function(leds_wifiFailed);
}

/**********************************************************************/

void btnHandler_reset_longClick(Button2& btn)
{
  leds_wifiFailed();
  wifiEraseCredentials();
  delay(1000);
  ESP.restart();
}

// Move to the next sensor pairing on double click on the show status button
void btnHandler_show_status_doubleClick(Button2& btn)
{
  sensorPairing_MoveToNext(&events);
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

// Stop the sensor pairing on long click on the show status button
void btnHandler_show_status_longClick(Button2& btn)
{
  sensorPairing_Stop(&events);
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
  btn_show_status.setDoubleClickHandler(btnHandler_show_status_doubleClick);
  btn_show_status.setLongClickDetectedHandler(btnHandler_show_status_longClick);
  btn_wifi.begin(BTN_WIFI_PIN);   //INPUT_PULLUP
  btn_wifi.setDebounceTime(100);
  btn_wifi.setLongClickTime(500);
  btn_wifi.setDoubleClickTime(400);
  btn_wifi.setClickHandler(btnHandler_wifi_click);

  leds.init();
  leds.setBrightness(20);
  leds_allOff();
  leds.start();

  // Begin LittleFS
  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // invalidate all last sensor messages
  for(uint8_t i=0; i < NUM_SUPPORTED_SENSORS; i++)
  {
    sensor_messages_latest[i].timestamp = -1;
  }

  Serial.print("My MAC-Address: ");
  Serial.println(WiFi.macAddress());

  timeHandling_init();

  wifiManager.setSaveConfigCallback(wifiManagerSaveCB);
  wifiManager.setAPCallback(wifiManagerAPOpenedCB);

  leds_wifiConnecting();
  
  // https://randomnerdtutorials.com/solved-reconnect-esp8266-nodemcu-to-wifi/ 
  // https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/generic-examples.html
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  WiFi.mode(WIFI_AP_STA);         // Set the device as a Station and Soft Access Point simultaneously
  WiFi.begin();
  boolean keepConnecting = true;
  uint8_t connectionStatus;
  unsigned long start = millis();
  while (keepConnecting)
  {
    connectionStatus = WiFi.status();
    if (millis() > start + CONNECTION_TIMEOUT_MS)
    {
      keepConnecting = false;
      Serial.println("Connection timed out");
    }
    if (connectionStatus == WL_CONNECTED || connectionStatus == WL_CONNECT_FAILED)
    {
      keepConnecting = false;
    }
    leds.service();
  }
  if(connectionStatus != WL_CONNECTED)
  {
    wifiManager.setConnectTimeout(1);
    wifiManager.startConfigPortalModeless(CONFIGURATION_AP_NAME, CONFIGURATION_AP_PW);
  }
  else
  {
    wifiManagerSaveCB();
  }

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

  sensorPairing_Stop(&events);
  updateLeds_sensorStatus();
}

void loop()
{
  wifiManager.loop();
  btn_reset.loop();
  btn_show_status.loop();
  btn_wifi.loop();
  leds.service();

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

        updateWebsiteForSensor(sensor_id, sensor_messages_latest[i]);
        
        bool isOpen = (sensor_message.pinState == SENSOR_PIN_STATE_OPEN);
        leds_sensorStatus(i, isOpen, battery_isEmpty(sensor_message.batteryVoltage_mV));

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