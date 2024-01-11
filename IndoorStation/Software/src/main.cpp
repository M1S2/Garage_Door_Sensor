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
#include "ESPAsyncWebServer.h"
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

// icons: https://fontawesome.com/search
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
<title>ESP-NOW DASHBOARD</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #2f4468; color: white; font-size: 1.7rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .reading { font-size: 2.8rem; }
    .packet { color: #bebebe; }
    .card.pinstate { color: #fd7e14; }
    .card.voltage { color: #1b78e2; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>ESP-NOW DASHBOARD</h3>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card pinstate">
        <h4><i class="fas fa-door-open"></i> #1 - STATE</h4><p><span class="reading" id="s0">?</span></p>
      </div>
      <div class="card voltage">
        <h4><i class="fas fa-battery-half"></i> #1 - VOLTAGE</h4><p><span class="reading"><span id="v0">?</span> V</span></p>
      </div>
      <div class="card pinstate">
        <h4><i class="fas fa-door-open"></i> #2 - STATE</h4><p><span class="reading" id="s1">?</span></p>
      </div>
      <div class="card voltage">
        <h4><i class="fas fa-battery-half"></i> #2 - VOLTAGE</h4><p><span class="reading"><span id="v1"></span>? V</span></p>
      </div>
    </div>
  </div>
<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 
 source.addEventListener('new_readings', function(e) {
  console.log("new_readings", e.data);
  var obj = JSON.parse(e.data);
  document.getElementById("s"+obj.id).innerHTML = ((obj.pinState == true) ? 'Closed' : 'Open');
  document.getElementById("v"+obj.id).innerHTML = obj.voltageVcc.toFixed(2);
 }, false);
}
</script>
</body>
</html>)rawliteral";

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

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  leds.begin();
  leds.setBrightness(30);
  leds.show(); // Initialize all pixels to 'off'

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

  // configure webserver
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send_P(200, "text/html", index_html);
  });

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