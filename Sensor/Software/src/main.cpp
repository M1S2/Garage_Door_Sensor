// This is the code that sends the garage door state to the indoor station
// The same code can be used for each sensor (the indoor station differentiates between the sensors by their MAC addresses)

// Add a file called "addresses.h" beside this "main.cpp" file that contains a line with the following content:
// #define INDOOR_STATION_MAC {0x01, 0x02, 0x03, 0x04, 0x05, 0x06}     // 01:02:03:04:05:06, replace this with the MAC address of the indoor stations ESP

// https://wolles-elektronikkiste.de/esp-now#multi_transm_1_recv  (Ein Transmitter, ein Receiver – Advanced (ESP32))

#include <Arduino.h>
#include <espnow.h>
#include <ESP8266WiFi.h>
#include "addresses.h"

#define MCU_LATCH_PIN       12    // GPIO 12
#define DOOR_SWITCH_PIN     14    // GPIO 14
#define MAX_SEND_RETRIES    10

ADC_MODE(ADC_VCC);

uint8_t indoor_station_mac[] = INDOOR_STATION_MAC;

typedef struct message
{
  bool pinState;
  float voltageVcc;
  uint8_t numberSendLoops;
} message_t;
message_t sensor_message; 

bool messageSentReady;
bool messageSentSuccessful;

void messageSent(uint8_t *macAddr, uint8_t status)
{
  messageSentSuccessful = (status == 0);
  messageSentReady = true;
}

// send the sensor status to the indoor station and repeat MAX_SEND_RETRIES times until the sending was successful
void sendSensorData()
{
  uint8_t loop_cnt = 0;
  do
  {
    loop_cnt++;
    //Serial.printf("Sender loop %d: ", loop_cnt);
    messageSentReady = false;
    sensor_message.voltageVcc = ESP.getVcc() / 1000.00;
    sensor_message.pinState = (digitalRead(DOOR_SWITCH_PIN) == HIGH);
    sensor_message.numberSendLoops = loop_cnt;
    esp_now_send(indoor_station_mac, (uint8_t *) &sensor_message, sizeof(sensor_message));
    while(messageSentReady == false) { delay(1); /* wait here. */ }
  }while(messageSentSuccessful == false && loop_cnt < MAX_SEND_RETRIES);

  if(loop_cnt == MAX_SEND_RETRIES)
  {
    Serial.printf("Maximum number of send retries reached (%d)\n", MAX_SEND_RETRIES);
    digitalWrite(LED_BUILTIN, HIGH);      // LED off
  }
  else
  {
    Serial.printf("Send Success after %d loop(s)\n", loop_cnt);
    digitalWrite(LED_BUILTIN, LOW);       // LED on
  }
}

void setup()
{
  pinMode(MCU_LATCH_PIN, OUTPUT);
  digitalWrite(MCU_LATCH_PIN, HIGH);    // Enable latch pin to keep ESP on

  pinMode(DOOR_SWITCH_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);      // LED off

  Serial.begin(115200);
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
    
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  if(esp_now_add_peer(indoor_station_mac, ESP_NOW_ROLE_COMBO, 0, NULL, 0) != 0)
  {
    Serial.println("Failed to add peer");
  }
  esp_now_register_send_cb(messageSent);

  sendSensorData();
  delay(3000);

  digitalWrite(MCU_LATCH_PIN, LOW);    // Disable latch pin to power off ESP
}
 
void loop()
{
  //sendSensorData();
  //delay(3000);
}