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

//#define DEBUG_OUTPUT                // enable this define to print debugging output on the serial. If this is disabled, no serial output is used at all (to save battery power)

uint8_t indoor_station_mac[] = INDOOR_STATION_MAC;

// Packing of this structure reduces the size to 5 bytes
PACK_STRUCT_BEGIN
typedef struct message_sensor
{
  PACK_STRUCT_FIELD(bool pinState);                 // This field contains the state of the sensor (door open / closed)
  PACK_STRUCT_FIELD(uint16_t batteryVoltage_mV);    // This field contains the measured battery voltage in mV
  PACK_STRUCT_FLD_8(uint8_t numberSendLoops);       // This field contains the total number of loops needed to send the message to the indoor station
  PACK_STRUCT_FLD_8(uint8_t sensor_sw_version);     // This field contains the software version of the sensor in the following format: Upper 4 bits contain the Major number, lower 4 bits contain the Minor number (each number can be in the range from 0..15)
}PACK_STRUCT_STRUCT message_sensor_t;
PACK_STRUCT_END

#define PAIRING_MAGIC_NUMBER    0xCAFEBEEF

PACK_STRUCT_BEGIN
typedef struct message_pairing
{
  PACK_STRUCT_FIELD(uint32_t pairingMagicNumber);         // This field contains a fixed number (0xCAFEBEEF). Only if this number is found, the MAC of the received message is saved by the Indoor Station.
}PACK_STRUCT_STRUCT message_pairing_t;
PACK_STRUCT_END

bool messageSentReady;
bool messageSentSuccessful;

bool isLedOn;
void setLedState(bool on)
{
  isLedOn = on;
  digitalWrite(LED_BUILTIN, isLedOn ? LOW : HIGH);
}
void toggleLedState()
{
  isLedOn = !isLedOn;
  setLedState(isLedOn);
}

void OnDataSent(uint8_t* macAddr, uint8_t status)
{
  messageSentSuccessful = (status == 0);
  messageSentReady = true;
}

/**
 * Turn the WiFi completely off.
 * https://www.instructables.com/ESP8266-Pro-Tips/ (Step 7)
 */
void WiFiOff()
{
	// "From the experiments, it was found that both WiFi.mode() and WiFi.forceSleepBegin() were required in order to switch off the radio. 
	// The forceSleepBegin() call will set the flags and modes necessary, but the radio will not actually switch off until control returns to the ESP ROM. 
	// To do that we’re adding a delay(1), but I suppose a yield() would work as well."
	WiFi.mode(WIFI_OFF);
	WiFi.forceSleepBegin();
	delay(1);
}

/**
 * Turn the WiFi on.
 * https://www.instructables.com/ESP8266-Pro-Tips/ (Step 7)
 */
void WiFiOn()
{
	WiFi.forceSleepWake();
	delay(1);
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
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
 * Read the ADC value and average it 16 times
 * https://www.esp8266.com/viewtopic.php?p=80628
 */
int readADC()
{
  int a;
  int i;
   
  a = analogRead(A0);
  a = 0;
  for(i=0; i<16; i++) 
  {
    a += analogRead(A0);
  }
  return a>>4;
}

/**
 * Get the battery voltage by measuring the ADC value and scaling it by the resistor divider.
 * ADC_RESISTOR_R8 is from the battery voltage to the ADC input.
 * ADC_RESISTOR_R9 is from the ADC input to GND.
 */
float getBatteryVoltage()
{
  int adcReadVal = readADC();
  float adcVoltage_V = adcReadVal / 1023.0f;
  float batteryVoltage_V = adcVoltage_V * ((ADC_RESISTOR_R8 + ADC_RESISTOR_R9) / ADC_RESISTOR_R9);

  #ifdef DEBUG_OUTPUT
    Serial.printf("ADC value=%d, ADC Voltage=%f V, Battery Voltage=%f V\n", adcReadVal, adcVoltage_V, batteryVoltage_V);
  #endif
  return batteryVoltage_V;
}

/**
 * Send data from the sensor to the indoor station and repeat MAX_SEND_RETRIES times until the sending was successful. Also loop all channels when not successful.
 * The isPairingMessage parameter determines if it is a pairing message (true) or a normal message (false).
 * A pairing message only contains a magic number to verify the correctness of the data. The received MAC is then saved by the indoor station.
 * @param isPairingMessage If true, a pairing message is created and send to the indoor station; if false, a sensor status message is generated and send.
 * @return return true if send success; otherwise false
 */
bool sendSensorData(bool isPairingMessage = false)
{
	message_sensor_t sensor_message;
	message_pairing_t pairing_message;
  
	if(isPairingMessage)
	{
		pairing_message.pairingMagicNumber = PAIRING_MAGIC_NUMBER;
	}
	else
	{
    WiFiOff();		// turn WiFi off to get more accurate readings
		sensor_message.batteryVoltage_mV = getBatteryVoltage() * 1000;
		sensor_message.pinState = (digitalRead(DOOR_SWITCH_PIN) == HIGH);
		sensor_message.sensor_sw_version = ((SENSOR_SW_VERSION_MAJOR & 0xF) << 4) + (SENSOR_SW_VERSION_MINOR & 0xF);
    WiFiOn();
	}
	
	for(uint8_t wifiChannelIndex = 0; wifiChannelIndex < MAX_WIFI_CHANNELS; wifiChannelIndex++)
	{
		uint8_t wifiChannel = wifi_channel_order[wifiChannelIndex];
		initEspNow(wifiChannel);

		uint8_t loop_cnt = 0;
		do
		{
			messageSentReady = false;
			loop_cnt++;
			if(isPairingMessage)
			{
				esp_now_send(indoor_station_mac, (uint8_t*) &pairing_message, sizeof(pairing_message));
			}
			else
			{
				sensor_message.numberSendLoops = loop_cnt + wifiChannelIndex * MAX_SEND_RETRIES;
				esp_now_send(indoor_station_mac, (uint8_t*) &sensor_message, sizeof(sensor_message));
			}
			while(messageSentReady == false) { delay(1); /* wait here. */ }
		}while(messageSentSuccessful == false && loop_cnt < MAX_SEND_RETRIES);

		if(messageSentSuccessful)
		{
			#ifdef DEBUG_OUTPUT
				Serial.printf("Send message successful on channel %d.\n", wifiChannel);
			#endif
			return true;    // break the wifiChannel for loop and return true to signal send success
		}
		else
		{
			#ifdef DEBUG_OUTPUT
				Serial.printf("Send message not successful on channel %d. Move to next channel.\n", wifiChannel);
			#endif
		}
	}
	return false;       // if ended here, the data couldn't be send on any channel
}

void setup()
{
  pinMode(MCU_LATCH_PIN, OUTPUT);
  digitalWrite(MCU_LATCH_PIN, HIGH);    // Enable latch pin to keep ESP on

  WiFiOff();

  pinMode(DOOR_SWITCH_PIN, INPUT_PULLDOWN_16);
  pinMode(LED_BUILTIN, OUTPUT);
  setLedState(true);    // LED on

  #ifdef DEBUG_OUTPUT
    Serial.begin(115200);
    Serial.println();
    Serial.print("Version of Garage Door Sensor SW: v");
    Serial.printf("%d.%d\n", SENSOR_SW_VERSION_MAJOR, SENSOR_SW_VERSION_MINOR);
    Serial.print("My MAC-Address: ");
    Serial.println(WiFi.macAddress());
  #endif

  sendSensorData(false);

  digitalWrite(MCU_LATCH_PIN, LOW);    // Disable latch pin to power off ESP

  #ifdef DEBUG_OUTPUT
    Serial.println("Sending sensor data successful. Wait 5 seconds before starting pairing.");
  #endif

  // delay 5 seconds after disabling the latch pin. If the ESP continues here and in the loop(), the sensor is held on via the pairing switch.
  for(int i = 0; i < 5; i++)
  {
    toggleLedState();
    delay(1000);
  }
  setLedState(true);    // LED on

  #ifdef DEBUG_OUTPUT
    Serial.println("Start to send pairing messages.");
  #endif
}
 
void loop()
{
  // The pairing data is send until the pairing switch is released. Even if the indoor station received the pairing data, it is send by the sensor but ignored by the indoor station.
  // No advanced acknowledge mechanism is used here.
  sendSensorData(true);
  toggleLedState();
}