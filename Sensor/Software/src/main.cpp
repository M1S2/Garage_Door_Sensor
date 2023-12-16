// This is the code that sends the garage door state to the indoor station
// The same code can be used for each sensor (the indoor station differentiates between the sensors by their MAC addresses)

// Add a file called "addresses.h" beside this "main.cpp" file that contains a line with the following content:
// #define INDOOR_STATION_MAC {0x01, 0x02, 0x03, 0x04, 0x05, 0x06}     // 01:02:03:04:05:06, replace this with the MAC address of the indoor stations ESP

// https://wolles-elektronikkiste.de/esp-now#multi_transm_1_recv  (Ein Transmitter, ein Receiver – Advanced (ESP32))

#include <Arduino.h>
#include <espnow.h>
#include <ESP8266WiFi.h>
#include "addresses.h"

#define DOOR_SWITCH_PIN    14    // GPIO 14 is D5 (https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/)

ADC_MODE(ADC_VCC);

uint8_t indoor_station_mac[] = INDOOR_STATION_MAC;

typedef struct message
{
  bool pinState;
  float voltageVcc;
} message_t;
message_t sensor_message; 

// This method can be omited after some testing
void messageSent(uint8_t *macAddr, uint8_t status)
{
  Serial.print("Send status: ");
  if(status == 0)
  {
    Serial.println("Success");
  }
  else
  {
    Serial.println("Error");
  }
}

void setup()
{
  pinMode(DOOR_SWITCH_PIN, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.print("My MAC-Address: ");
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() == 0)
  {
    Serial.println("ESPNow Init success");
  }
  else 
  {
    Serial.println("ESPNow Init fail");
    return;
  }
    
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  if(esp_now_add_peer(indoor_station_mac, ESP_NOW_ROLE_COMBO, 0, NULL, 0) != 0)
  {
    Serial.println("Failed to add peer");
  }
  esp_now_register_send_cb(messageSent);
}
 
void loop()
{
  sensor_message.voltageVcc = ESP.getVcc() / 1000.00;
  sensor_message.pinState = (digitalRead(DOOR_SWITCH_PIN) == HIGH);

  // send the sensor status to the indoor station
  esp_now_send(indoor_station_mac, (uint8_t *) &sensor_message, sizeof(sensor_message));
  delay(5000);
}