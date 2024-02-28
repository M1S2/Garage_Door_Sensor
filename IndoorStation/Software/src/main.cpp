// This is the code that receives all garage door states
// It differentiates between the sensors by their MAC addresses

// Add a file called "addresses.h" beside this "main.cpp" file that contains a line for each sensor that should be used with the following content:
// #define SENSOR1_MAC {0x01, 0x02, 0x03, 0x04, 0x05, 0x06}     // 01:02:03:04:05:06, replace this with the MAC address of the sensor ESP of the first sensor

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
#include "leds.h"
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

message_sensor_t sensor_messages[ARRAY_ELEMENT_COUNT(sensor_macs)];

enum Sensor_Pairing_Modes { PAIRING_MODE_SENSOR1, PAIRING_MODE_SENSOR2, PAIRING_MODE_NONE };
Sensor_Pairing_Modes sensorPairingMode = PAIRING_MODE_NONE;

/**********************************************************************/

void updateLeds_sensorStatus()
{
  for(uint8_t i = 0; i < ARRAY_ELEMENT_COUNT(sensor_messages); i++)
  {
    bool isOpen = (sensor_messages[i].pinState == SENSOR_PIN_STATE_OPEN);
    leds_sensorStatus(i, isOpen);
  }

  for(int i = ARRAY_ELEMENT_COUNT(sensor_messages); i < NUM_SUPPORTED_SENSORS; i++)
  {
    leds_singleOff(i);
  }
}

/**********************************************************************/

void updateWebsiteForSensor(uint8_t sensor_id, message_sensor_t sensor_message)
{
  // create a JSON document with the data and send it by event to the web page
  StaticJsonDocument<1000> root;
  String payload;
  root["id"] = sensor_id;
  root["batteryVoltage_V"] = (sensor_message.batteryVoltage_mV / 1000.0f);
  root["pinState"] = sensor_message.pinState;
  serializeJson(root, payload);
  Serial.printf("event send: %s\n", payload.c_str());
  events.send(payload.c_str(), "new_readings", millis());
}

void updateWebsite()
{
  for(uint8_t i=0; i < ARRAY_ELEMENT_COUNT(sensor_messages); i++)
  {
    updateWebsiteForSensor(i, sensor_messages[i]);
  }
}

void messageReceived(uint8_t* mac_addr, uint8_t* data, uint8 len)
{
  message_sensor_t sensor_message; 
  memcpy(&sensor_message, data, sizeof(sensor_message));

  Serial.printf("Transmitter MAC Address: %02X:%02X:%02X:%02X:%02X:%02X \n\r", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);    
  int sensor_id = -1;
  for(uint i = 0; i < ARRAY_ELEMENT_COUNT(sensor_macs); i++)
  {  
    if(mac_addr[0] == sensor_macs[i][0] && 
        mac_addr[1] == sensor_macs[i][1] &&
        mac_addr[2] == sensor_macs[i][2] &&
        mac_addr[3] == sensor_macs[i][3] &&
        mac_addr[4] == sensor_macs[i][4] &&
        mac_addr[5] == sensor_macs[i][5])
    {
      sensor_id = i;
      sensor_messages[i] = sensor_message;
      Serial.printf("Data received from Sensor %d \n\r", sensor_id);

      updateWebsiteForSensor(sensor_id, sensor_message);
      
      bool isOpen = (sensor_message.pinState == SENSOR_PIN_STATE_OPEN);
      leds_sensorStatus(i, isOpen);

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
  Serial.printf("Channel=%d", WiFi.channel());
  Serial.println();
  digitalWrite(LED_BUILTIN, LOW);
}

/**********************************************************************/

void updateWebsiteForSensorPairingStatus()
{
  // create a JSON document with the data and send it by event to the web page
  StaticJsonDocument<1000> root;
  String payload;
  root["pairing_id"] = sensorPairingMode;
  root["pairing_active"] = (sensorPairingMode != PAIRING_MODE_NONE);
  serializeJson(root, payload);
  Serial.printf("event send: %s\n", payload.c_str());
  events.send(payload.c_str(), "new_sensor_pairing_status", millis());
}

void sensorPairingStop()
{
  sensorPairingMode = PAIRING_MODE_NONE;
  updateLeds_sensorStatus();
  updateWebsiteForSensorPairingStatus();
}

// sensorIndex = 0..1
void sensorPairingStart(int sensorIndex)
{
  switch(sensorIndex)
  {
    case 0: sensorPairingMode = PAIRING_MODE_SENSOR1; leds_sensorPairing(SENSOR1_LED_INDEX); updateWebsiteForSensorPairingStatus(); break;
    case 1: sensorPairingMode = PAIRING_MODE_SENSOR2; leds_sensorPairing(SENSOR2_LED_INDEX); updateWebsiteForSensorPairingStatus(); break;
    default: sensorPairingStop(); break;
  }
}

void sensorPairingMoveToNext()
{
  switch(sensorPairingMode)
  {
    case PAIRING_MODE_NONE: sensorPairingStart(PAIRING_MODE_SENSOR1); break;
    case PAIRING_MODE_SENSOR1: sensorPairingStart(PAIRING_MODE_SENSOR2); break;
    case PAIRING_MODE_SENSOR2: sensorPairingStop(); break;
  }
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
      sensorPairingStop();
    }
    else if(request->hasParam("sensor"))
    {
      inputMessage = request->getParam("sensor")->value();
      sensorPairingStart(inputMessage.toInt() - 1);
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
  sensorPairingMoveToNext();
}

void btnHandler_show_status_click(Button2& btn)
{
  Serial.println("Show Status Click");
}

// Stop the sensor pairing on long click on the show status button
void btnHandler_show_status_longClick(Button2& btn)
{
  sensorPairingStop();
}

void btnHandler_wifi_click(Button2& btn)
{
  Serial.println("Wifi Click");
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
  leds.setBrightness(30);
  leds_allOff();
  leds.start();

  // Begin LittleFS
  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  Serial.print("My MAC-Address: ");
  Serial.println(WiFi.macAddress());

  wifiManager.setSaveConfigCallback(wifiManagerSaveCB);
  wifiManager.setAPCallback(wifiManagerAPOpenedCB);

  // Set the device as a Station and Soft Access Point simultaneously
  leds_wifiConnecting();
  WiFi.mode(WIFI_AP_STA);
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

  sensorPairingStop();
}

void loop()
{
  wifiManager.loop();
  btn_reset.loop();
  btn_show_status.loop();
  btn_wifi.loop();
  leds.service();
}