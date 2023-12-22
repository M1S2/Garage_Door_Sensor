// This is the code that receives all garage door states
// It differentiates between the sensors by their MAC addresses

// Add a file called "addresses.h" beside this "main.cpp" file that contains a line for each sensor that should be used with the following content:
// #define SENSOR1_MAC {0x01, 0x02, 0x03, 0x04, 0x05, 0x06}     // 01:02:03:04:05:06, replace this with the MAC address of the sensor ESP of the first sensor

// https://wolles-elektronikkiste.de/esp-now#multi_transm_1_recv  (Ein Transmitter, ein Receiver – Advanced (ESP32))

#include <Arduino.h>
#include <espnow.h>
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include "addresses.h"

// https://forum.arduino.cc/t/finding-the-size-of-multi-dimensional-array/395465/8
#define ARRAY_ELEMENT_COUNT(array) (sizeof array / sizeof array[0])

uint8_t sensor_macs[][6] = SENSOR_MACS;

#define NUM_LEDS  ARRAY_ELEMENT_COUNT(sensor_macs)        // Number of WS2812B leds
#define LEDS_PIN  12                                      // The pin which is used to control the WS2812 leds
#define COLOR_DOOR_OPEN   leds.Color(0, 255, 0)           // LED color used to indicate an open door
#define COLOR_DOOR_CLOSED leds.Color(255, 0, 0)           // LED color used to indicate a closed door
Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, LEDS_PIN, NEO_GRB + NEO_KHZ800);

typedef struct message
{
  bool pinState;
  float voltageVcc;
} message_t;

message_t sensor_messages[ARRAY_ELEMENT_COUNT(sensor_macs)];

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

  WiFi.mode(WIFI_STA);
  Serial.print("My MAC-Address: ");
  Serial.println(WiFi.macAddress());
  WiFi.disconnect();

  if (esp_now_init() == 0) 
  {
    Serial.println("ESPNow Init success");
  }
  else 
  {
    Serial.println("ESPNow Init fail");
    return;
  }

  digitalWrite(LED_BUILTIN, HIGH);
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(messageReceived);
}

void loop()
{
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