#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <Arduino.h>

// Packing of this structure reduces the size to 4 bytes
typedef struct __attribute__((packed)) message_sensor
{
  bool pinState;                  // This field contains the state of the sensor (door open / closed)
  uint16_t batteryVoltage_mV;     // This field contains the measured battery voltage in mV
  uint8_t numberSendLoops;        // This field contains the total number of loops needed to send the message to the indoor station
  uint8_t sensor_sw_version;      // This field contains the software version of the sensor in the following format: Upper 4 bits contain the Major number, lower 4 bits contain the Minor number (each number can be in the range from 0..15)
}message_sensor_t;

typedef struct __attribute__((packed)) message_sensor_timestamped
{
  message_sensor_t msg;
  time_t timestamp;
}message_sensor_timestamped_t;

#endif