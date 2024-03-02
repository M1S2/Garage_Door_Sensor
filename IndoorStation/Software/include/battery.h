#include <Arduino.h>

#define BATTERY_EMPTY_PERCENT   15              // If the battery level in percent falls under this level, the battery is regarded as empty

/**
 * Convert the battery voltage in mV to an level in percent.
 * @param batteryVoltage_mV Measured voltage of the battery in mV.
 * @return Value between 0 and 100 indicating the battery level in percent.
 */
uint8_t battery_voltageToPercent(uint16_t batteryVoltage_mV);

/**
 * Check if the battery is empty. It is regarded as empty, when the battery level in percent falls under BATTERY_EMPTY_PERCENT
 * @param batteryVoltage_V Measured voltage of the battery in mV.
 * @return True, if the battery level is below BATTERY_EMPTY_PERCENT; otherwise false.
 */
bool battery_isEmpty(uint16_t batteryVoltage_mV);