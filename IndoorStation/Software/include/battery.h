#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

#define BATTERY_EMPTY_PERCENT   15              // If the battery level in percent falls under this level, the battery is regarded as empty

/**
 * Convert the battery voltage in mV to an level in percent.
 * @param batteryVoltage_mV Measured voltage of the battery in mV.
 * @param numberFractionalDigits Number of fractional digits returned. E.g. numberFractionalDigits=2 --> return e.g. 3.14
 * @return Value between 0 and 100 indicating the battery level in percent. The return value uses the number of fractional digits given.
 */
float battery_voltageToPercent(uint16_t batteryVoltage_mV, uint8_t numberFractionalDigits = 2);

/**
 * Check if the battery is empty. It is regarded as empty, when the battery level in percent falls under BATTERY_EMPTY_PERCENT
 * @param batteryVoltage_V Measured voltage of the battery in mV.
 * @return True, if the battery level is below BATTERY_EMPTY_PERCENT; otherwise false.
 */
bool battery_isEmpty(uint16_t batteryVoltage_mV);

#endif