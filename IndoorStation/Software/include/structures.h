#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <Arduino.h>

// Packing of this structure reduces the size to 4 bytes
typedef struct __attribute__((packed)) message_sensor
{
  bool pinState;
  uint16_t batteryVoltage_mV;
  uint8_t numberSendLoops;
}message_sensor_t;

typedef struct __attribute__((packed)) message_sensor_timestamped
{
  message_sensor_t msg;
  time_t timestamp;
}message_sensor_timestamped_t;

#endif