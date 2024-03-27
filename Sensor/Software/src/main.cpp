// This is the code that sends the garage door state to the indoor station
// The same code can be used for each sensor (the indoor station differentiates between the sensors by their MAC addresses)

// CAUTION: Copy and adapt the addresses_Template.h file in the include folder!!!

// https://wolles-elektronikkiste.de/esp-now#multi_transm_1_recv  (Ein Transmitter, ein Receiver – Advanced (ESP32))

#include <Arduino.h>
#include <espnow.h>
#include <ESP8266WiFi.h>
#include "addresses.h"
#include "version.h"

#define MCU_LATCH_PIN       12      // GPIO 12
#define DOOR_SWITCH_PIN     14      // GPIO 14
#define MAX_SEND_RETRIES    10      // Number of send retries for each channel before changing to the next channel
#define MAX_WIFI_CHANNELS   13      // 13 in Europe, 11 for North America (adapt the following list accordingly)
uint8_t const wifi_channel_order[] = { 1, 6, 11, 2, 3, 4, 5, 7, 8, 9, 10, 12, 13};      // First try to use the channels 1, 6 and 11 to send (these are the most assigned ones by routers)

#define ADC_RESISTOR_R8     390.0f  // Resistor value in kOhm, from the battery voltage to the ADC input
#define ADC_RESISTOR_R9     100.0f  // Resistor value in kOhm, from the ADC input to GND

//#define DEBUG_OUTPUT                // enable this define to print debugging output on the serial. If this is disabled, no serial output is used at all (to save power, except the version of this SW and the MAC that is always printed at the beginning)

uint8_t indoor_station_mac[] = INDOOR_STATION_MAC;

// Packing of this structure reduces the size to 4 bytes
PACK_STRUCT_BEGIN
typedef struct message_sensor
{
  PACK_STRUCT_FIELD(bool pinState);
  PACK_STRUCT_FIELD(uint16_t batteryVoltage_mV);
  PACK_STRUCT_FLD_8(uint8_t numberSendLoops);
}PACK_STRUCT_STRUCT message_sensor_t;
PACK_STRUCT_END

message_sensor_t sensor_message;

bool messageSentReady;
bool messageSentSuccessful;

void OnDataSent(uint8_t* macAddr, uint8_t status)
{
  messageSentSuccessful = (status == 0);
  messageSentReady = true;
}

/**
 * Reinitialize ESP-NOW to the requested WiFi channel.
 * @param wifiChannel WiFi channel that is used for the ESP-NOW connection. This must match the receiver channel that depends on the channel used by the router.
 */
void initEspNow(uint8_t wifiChannel)
{
  // clean esp now
  esp_now_deinit();
  WiFi.mode(WIFI_STA);
  // set WiFi channel   
  wifi_promiscuous_enable(1);
  wifi_set_channel(wifiChannel);
  wifi_promiscuous_enable(0);

  //#ifdef DEBUG_OUTPUT
  //WiFi.printDiag(Serial);
  //#endif
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) 
  {
    #ifdef DEBUG_OUTPUT
      Serial.println("Error initializing ESP-NOW");
    #endif
  }
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  // set callback routines
  esp_now_register_send_cb(OnDataSent);

  if(esp_now_add_peer(indoor_station_mac, ESP_NOW_ROLE_COMBO, wifiChannel, NULL, 0) != 0)
  {
    #ifdef DEBUG_OUTPUT
      Serial.println("Failed to add peer");
    #endif
  }
}

/**
 * Get the battery voltage by measuring the ADC value and scaling it by the resistor divider.
 * ADC_RESISTOR_R8 is from the battery voltage to the ADC input.
 * ADC_RESISTOR_R9 is from the ADC input to GND.
 */
float getBatteryVoltage()
{
  int adcReadVal = analogRead(A0);
  float adcVoltage_V = adcReadVal / 1023.0f;
  float batteryVoltage_V = adcVoltage_V * ((ADC_RESISTOR_R8 + ADC_RESISTOR_R9) / ADC_RESISTOR_R9);

  #ifdef DEBUG_OUTPUT
    Serial.printf("ADC value=%d, ADC Voltage=%f V, Battery Voltage=%f V\n", adcReadVal, adcVoltage_V, batteryVoltage_V);
  #endif
  return batteryVoltage_V;
}

/**
 * Send the sensor status to the indoor station and repeat MAX_SEND_RETRIES times until the sending was successful. Also loop all channels when not successful.
 * @return return true if send success; otherwise false
 */
bool sendSensorData()
{
  sensor_message.batteryVoltage_mV = getBatteryVoltage() * 1000;
  sensor_message.pinState = (digitalRead(DOOR_SWITCH_PIN) == HIGH);

  for(uint8_t wifiChannelIndex = 0; wifiChannelIndex < MAX_WIFI_CHANNELS; wifiChannelIndex++)
  {
    uint8_t wifiChannel = wifi_channel_order[wifiChannelIndex];

    initEspNow(wifiChannel);
    uint8_t loop_cnt = 0;
    do
    {
      loop_cnt++;
      messageSentReady = false;
      sensor_message.numberSendLoops = loop_cnt + wifiChannelIndex * MAX_SEND_RETRIES;
      esp_now_send(indoor_station_mac, (uint8_t *) &sensor_message, sizeof(sensor_message));
      while(messageSentReady == false) { delay(1); /* wait here. */ }
    }while(messageSentSuccessful == false && loop_cnt < MAX_SEND_RETRIES);

    if(loop_cnt == MAX_SEND_RETRIES)
    {
      #ifdef DEBUG_OUTPUT
        Serial.printf("Maximum number of send retries reached (%d) on channel %d. Move to next channel.\n", MAX_SEND_RETRIES, wifiChannel);
      #endif
    }
    else
    {
      #ifdef DEBUG_OUTPUT
        Serial.printf("Send Success after %d loop(s) over channel %d\n", loop_cnt, wifiChannel);
        Serial.printf("Total number of send tries = %d\n", sensor_message.numberSendLoops);
      #endif
      return true;    // break the wifiChannel for loop and return true to signal send success
    }
  }
  return false;       // if ended here, the data couldn't be send on any channel
}

void setup()
{
  pinMode(MCU_LATCH_PIN, OUTPUT);
  digitalWrite(MCU_LATCH_PIN, HIGH);    // Enable latch pin to keep ESP on

  pinMode(DOOR_SWITCH_PIN, INPUT_PULLDOWN_16);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);      // LED on

  Serial.begin(115200);
  Serial.println();
  Serial.print("Version of Garage Door Sensor SW: ");
  Serial.println(GARAGE_DOOR_SENSOR_SW_VERSION);
  Serial.print("My MAC-Address: ");
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  sendSensorData();

  digitalWrite(MCU_LATCH_PIN, LOW);    // Disable latch pin to power off ESP
}
 
void loop()
{
}