// This is the code that sends the garage door state to the indoor station
// The same code can be used for each sensor (the indoor station differentiates between the sensors by their MAC addresses)

// CAUTION: Copy and adapt the addresses_Template.h file in the include folder!!!

// https://wolles-elektronikkiste.de/esp-now#multi_transm_1_recv  (Ein Transmitter, ein Receiver – Advanced (ESP32))

#include <Arduino.h>
#include <espnow.h>
#include <ESP8266WiFi.h>
#include "pairing.h"
#include "version.h"
#include "config.h"

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

PACK_STRUCT_BEGIN
typedef struct message_pairing
{
    PACK_STRUCT_FIELD(uint32_t pairingMagicNumber);         // This field contains a fixed number (0xCAFEBEEF). Only if this number is found, the MAC of the received message is saved by the Indoor Station.
    PACK_STRUCT_FIELD(uint32_t pairingToken);               // This field contains a token for pairing validation. The indoor station generates a random token for each pairing session. The sensor must include this token in the pairing message. The indoor station only accepts the pairing message if the token is correct. This prevents that an attacker can send a pairing message with the correct magic number and a random MAC address to pair with the indoor station without knowing the token.
    PACK_STRUCT_FIELD(uint8_t sensorMac[6] = {0});          // This field contains the MAC address of the sensor which wants to pair. The indoor station saves this MAC address in the system config to identify the sensor in the future and to display the correct MAC address in the web interface.
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
 * @param forceUnencrypted If true, the peer will be added without encryption. This is used for pairing messages (the indoor station will be in AP mode then and isn't able to receive encrypted messages).
 */
void initEspNow(uint8_t wifiChannel, bool forceUnencrypted)
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(1);

    // set WiFi channel
    wifi_promiscuous_enable(1);
    wifi_set_channel(wifiChannel);
    wifi_promiscuous_enable(0);

    // If the peer already exists → delete it (important for channel change!)
    if (esp_now_is_peer_exist((uint8_t*)PairingInfo.indoor_station_mac))
    {
        esp_now_del_peer((uint8_t*)PairingInfo.indoor_station_mac);
    }

    int result = -1;
    if(!utils_isKeyEmpty(PairingInfo.lmk, ESPNOW_KEY_LEN) && !utils_isKeyEmpty(PairingInfo.pmk, ESPNOW_KEY_LEN) && !forceUnencrypted)
    {
        result = esp_now_add_peer(PairingInfo.indoor_station_mac, ESP_NOW_ROLE_SLAVE, 0, PairingInfo.lmk, ESPNOW_KEY_LEN);

        #ifdef DEBUG_OUTPUT
            char pmkHex[2 * ESPNOW_KEY_LEN + 1];
            utils_bytesToHex(PairingInfo.pmk, ESPNOW_KEY_LEN, pmkHex);
            char lmkHex[2 * ESPNOW_KEY_LEN + 1];
            utils_bytesToHex(PairingInfo.lmk, ESPNOW_KEY_LEN, lmkHex);
            Serial.printf("[Main] Add peer with encryption: pmk=%s, lmk=%s\n", pmkHex, lmkHex);
        #endif
    }
    else
    {
        result = esp_now_add_peer(PairingInfo.indoor_station_mac, ESP_NOW_ROLE_SLAVE, 0, NULL, 0);
        
        #ifdef DEBUG_OUTPUT
            Serial.println("[Main] Add peer without encryption.");
        #endif
    }

    if(result != 0)
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[Main] Failed to add peer");
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
        Serial.printf("[Main] ADC value=%d, ADC Voltage=%f V, Battery Voltage=%f V\n", adcReadVal, adcVoltage_V, batteryVoltage_V);
    #endif
    return batteryVoltage_V;
}

/**
 * Send data from the sensor to the indoor station and repeat MAX_SEND_RETRIES times until the sending was successful. Also loop all channels when not successful.
 * The data is sent encrypted with the PMK and LMK that are stored in the pairing info. If no valid PMK and LMK are available, the data is sent unencrypted.
 * @return return true if send success; otherwise false
 */
bool sendSensorData()
{
    if(!pairing_isPairingInfoValid())
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[Main] No valid pairing info available. Can't send data.");
        #endif
        return false;
    }

    WiFiOff();		// turn WiFi off to get more accurate readings
    message_sensor_t sensor_message;
    sensor_message.batteryVoltage_mV = getBatteryVoltage() * 1000;
    sensor_message.pinState = (digitalRead(DOOR_SWITCH_PIN) == HIGH);
    sensor_message.sensor_sw_version = ((SENSOR_SW_VERSION_MAJOR & 0xF) << 4) + (SENSOR_SW_VERSION_MINOR & 0xF);
    WiFiOn();
	
    for(uint8_t wifiChannelIndex = 0; wifiChannelIndex < MAX_WIFI_CHANNELS; wifiChannelIndex++)
    {
        uint8_t wifiChannel = wifi_channel_order[wifiChannelIndex];
        initEspNow(wifiChannel, false);

        uint8_t loop_cnt = 0;
        do
        {
            messageSentReady = false;
            loop_cnt++;
            sensor_message.numberSendLoops = loop_cnt + wifiChannelIndex * MAX_SEND_RETRIES;
            esp_now_send(PairingInfo.indoor_station_mac, (uint8_t*) &sensor_message, sizeof(sensor_message));
            while(messageSentReady == false) { delay(1); /* wait here. */ }
        }while(messageSentSuccessful == false && loop_cnt < MAX_SEND_RETRIES);

        if(messageSentSuccessful)
        {
            #ifdef DEBUG_OUTPUT
                Serial.printf("[Main] Send message successful on channel %d.\n", wifiChannel);
            #endif
            return true;    // break the wifiChannel for loop and return true to signal send success
        }
        else
        {
            #ifdef DEBUG_OUTPUT
                Serial.printf("[Main] Send message not successful on channel %d. Move to next channel.\n", wifiChannel);
            #endif
        }
    }
    return false;       // if ended here, the data couldn't be send on any channel
}

/**********************************************************************/

/**
 * Send pairing data from the sensor to the indoor station and repeat MAX_SEND_RETRIES times until the sending was successful.
 * The pairing data is sent on the channel that is stored in the pairing info (this should be the channel of the indoor station's AP that is used for pairing).
 * The pairing message is sent without encryption, because the indoor station is in AP mode during pairing and isn't able to receive encrypted messages.
 * @return return true if send success; otherwise false
 */
bool sendPairingData()
{
    message_pairing_t pairing_message;
    pairing_message.pairingMagicNumber = PAIRING_MAGIC_NUMBER;
    pairing_message.pairingToken = pairing_token;
    WiFi.macAddress(pairing_message.sensorMac);     // save the own MAC address in the message to validate against the received MAC on the indoor station.
    
    uint8_t wifiChannel = pairing_last_wifi_channel;
    initEspNow(wifiChannel, true);   // for pairing messages, encryption is not possible because the indoor station is in AP mode and can't receive encrypted messages

    uint8_t loop_cnt = 0;
    do
    {
        messageSentReady = false;
        loop_cnt++;
        esp_now_send(PairingInfo.indoor_station_mac, (uint8_t*) &pairing_message, sizeof(pairing_message));
        while(messageSentReady == false) { delay(1); /* wait here. */ }
    }while(messageSentSuccessful == false && loop_cnt < MAX_SEND_RETRIES);

    if(messageSentSuccessful)
    {
        #ifdef DEBUG_OUTPUT
            Serial.printf("[Main] Send message successful on channel %d.\n", wifiChannel);
        #endif
        return true;    // break the wifiChannel for loop and return true to signal send success
    }
    else
    {
        #ifdef DEBUG_OUTPUT
            Serial.printf("[Main] Send message not successful on channel %d. Move to next channel.\n", wifiChannel);
        #endif
    }
    return false;       // if ended here, the data couldn't be send
}

/**********************************************************************/

bool pairing_fetchInfoSuccess = false;

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
        Serial.print("[Main] Version of Garage Door Sensor SW: v");
        Serial.printf("%d.%d\n", SENSOR_SW_VERSION_MAJOR, SENSOR_SW_VERSION_MINOR);
        Serial.print("[Main] My MAC-Address: ");
        Serial.println(WiFi.macAddress());
    #endif

    if (pairing_loadPairingInfo())
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[Main] Pairing info loaded.");
        #endif
    }
    else
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[Main] No valid pairing info available.");
        #endif
    }

    // Init ESP-NOW
    if (esp_now_init() != 0) 
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[Main] Error initializing ESP-NOW");
        #endif
    }
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_set_kok(PairingInfo.pmk, ESPNOW_KEY_LEN);
    esp_now_register_send_cb(OnDataSent);

    bool sendSuccess = sendSensorData();

    digitalWrite(MCU_LATCH_PIN, LOW);    // Disable latch pin to power off ESP

    #ifdef DEBUG_OUTPUT
        Serial.println("[Main] Sending sensor data " + String(sendSuccess ? "successful" : "failed") + ". Wait 5 seconds before starting pairing.");
    #endif

    // delay 5 seconds after disabling the latch pin. If the ESP continues here and in the loop(), the sensor is held on via the pairing switch.
    for(int i = 0; i < 5; i++)
    {
        toggleLedState();
        delay(1000);
    }
    setLedState(true);    // LED on

    #ifdef DEBUG_OUTPUT
        Serial.println("[Main] Fetch pairing info from indoor station...");
    #endif

    pairing_fetchInfoSuccess = pairing_runPairing();
    if (pairing_fetchInfoSuccess)
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[Main] Finding pairing info successful!");
        #endif
      
        // save the pairing info (including the received MAC) in the EEPROM to be able to load it after a power cycle without the need of pairing again
        pairing_savePairingInfo();
    }
    else
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[Main] Finding pairing info failed.");
        #endif
    }
  
    // Disable WiFi to save power.
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    if(pairing_fetchInfoSuccess)
    {
        #ifdef DEBUG_OUTPUT
            Serial.println("[Main] Start to send pairing messages.");
        #endif

        // Send the pairing message as long as it is received by the indoor station.
        sendSuccess = false;
        do
        {
            toggleLedState();
            sendSuccess = sendPairingData();
            #ifdef DEBUG_OUTPUT
                if(sendSuccess)
                {
                    Serial.println("[Main] Send pairing message successful.");
                }
                else
                {
                    Serial.println("[Main] Send pairing message failed. Retry...");
                }
            #endif
        } while (!sendSuccess);
        setLedState(true);    // LED on
    }
}
 
void loop()
{
    // Nothing to do here.
}