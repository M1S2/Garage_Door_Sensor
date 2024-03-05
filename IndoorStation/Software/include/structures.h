#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <Arduino.h>

// Packing of this structure reduces the size to 4 bytes
PACK_STRUCT_BEGIN
typedef struct message_sensor
{
  PACK_STRUCT_FIELD(bool pinState);
  PACK_STRUCT_FIELD(uint16_t batteryVoltage_mV);
  PACK_STRUCT_FLD_8(uint8_t numberSendLoops);
}PACK_STRUCT_STRUCT message_sensor_t;
PACK_STRUCT_END

PACK_STRUCT_BEGIN
typedef struct message_sensor_timestamped
{
  PACK_STRUCT_FIELD(message_sensor_t msg);
  PACK_STRUCT_FIELD(time_t timestamp);
}PACK_STRUCT_STRUCT message_sensor_timestamped_t;
PACK_STRUCT_END

#endif