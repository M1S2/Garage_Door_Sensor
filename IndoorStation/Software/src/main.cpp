// This is the code that receives all garage door states
// It differentiates between the sensors by their MAC addresses

// Add a file called "addresses.h" beside this "main.cpp" file that contains a line for each sensor that should be used with the following content:
// #define SENSOR1_MAC {0x01, 0x02, 0x03, 0x04, 0x05, 0x06}     // 01:02:03:04:05:06, replace this with the MAC address of the sensor ESP of the first sensor
// This file should also contain two defines for the Wifi SSID and password:
// #define WIFI_SSID "<Enter SSID here>";
// #define WIFI_PASSWORD "<Enter password here>";

// https://wolles-elektronikkiste.de/esp-now#multi_transm_1_recv  (Ein Transmitter, ein Receiver – Advanced (ESP32))
// https://randomnerdtutorials.com/esp-now-auto-pairing-esp32-esp8266/

#include <Arduino.h>
#include <espnow.h>
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "addresses.h"

AsyncWebServer server(80);
AsyncEventSource events("/events");

// https://forum.arduino.cc/t/finding-the-size-of-multi-dimensional-array/395465/8
#define ARRAY_ELEMENT_COUNT(array) (sizeof array / sizeof array[0])

uint8_t sensor_macs[][6] = SENSOR_MACS;
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

#define NUM_LEDS  ARRAY_ELEMENT_COUNT(sensor_macs)        // Number of WS2812B leds
#define LEDS_PIN  12                                      // The pin which is used to control the WS2812 leds
#define COLOR_DOOR_OPEN   leds.Color(0, 255, 0)           // LED color used to indicate an open door
#define COLOR_DOOR_CLOSED leds.Color(255, 0, 0)           // LED color used to indicate a closed door
Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, LEDS_PIN, NEO_GRB + NEO_KHZ800);

typedef struct message
{
  bool pinState;
  float voltageVcc;
  uint8_t numberSendLoops;
} message_t; 

message_t sensor_messages[ARRAY_ELEMENT_COUNT(sensor_macs)];

void updateWebsiteForSensor(uint8_t sensor_id, message_t sensor_message)
{
  // create a JSON document with the data and send it by event to the web page
  StaticJsonDocument<1000> root;
  String payload;
  root["id"] = sensor_id;
  root["voltageVcc"] = sensor_message.voltageVcc;
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
  message_t sensor_message; 
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
      break;  
    }
  }

  if (sensor_id == -1)
  {
    Serial.println("Data received from unknown Sensor");
  }
  
  Serial.print("PinState: ");
  Serial.println(sensor_message.pinState);
  Serial.print("VCC: ");
  Serial.println(sensor_message.voltageVcc);
  Serial.print("NumberSendLoops: ");
  Serial.println(sensor_message.numberSendLoops);
  Serial.printf("Channel=%d", WiFi.channel());
  Serial.println();
  digitalWrite(LED_BUILTIN, LOW);
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
    request->send(LittleFS, "/sensor_info.html", "text/html"); 
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
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  leds.begin();
  leds.setBrightness(30);
  leds.show(); // Initialize all pixels to 'off'

  // Begin LittleFS
  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // Set the device as a Station and Soft Access Point simultaneously
  WiFi.mode(WIFI_AP_STA);
  Serial.print("My MAC-Address: ");
  Serial.println(WiFi.macAddress());
  
  WiFi.begin(ssid, password);
  Serial.print("Setting as a Wi-Fi Station");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  if (esp_now_init() == 0) 
  {
    Serial.println("ESPNow Init success");
  }
  else 
  {
    Serial.println("ESPNow Init fail");
    return;
  }

  initWebserverFiles();

  // events 
  events.onConnect([](AsyncEventSourceClient *client)
  {
    updateWebsite();      // Update the values on the website when it is opened (or reloaded in the browser)
  });
  server.addHandler(&events);
  // start webserver
  server.begin();

  digitalWrite(LED_BUILTIN, HIGH);            // Turn off LED
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(messageReceived);
}

void loop()
{
  /*static unsigned long lastEventTime = millis();
  static const unsigned long EVENT_INTERVAL_MS = 5000;
  if ((millis() - lastEventTime) > EVENT_INTERVAL_MS) 
  {
    //events.send("ping",NULL,millis());
    lastEventTime = millis();
  }*/

  for(uint8_t i=0; i < ARRAY_ELEMENT_COUNT(sensor_messages); i++)
  {
    if(sensor_messages[i].pinState == LOW) // door is open
    {
      leds.setPixelColor(i, COLOR_DOOR_OPEN);
    }
    else if(sensor_messages[i].pinState == HIGH) // door is closed
    {
      leds.setPixelColor(i, COLOR_DOOR_CLOSED);
    }
  }
  leds.show();
  delay(500);
}