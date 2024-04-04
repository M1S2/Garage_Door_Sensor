#include "battery.h"

typedef struct batteryLookupTableEntry
{
    uint8_t batteryLevel_percent;
    uint16_t batteryVoltage_mV;
}batteryLookupTableEntry_t;

// https://www.reddit.com/r/18650masterrace/comments/ibhp9i/what_voltage_is_considered_the_0_of_an_18650/
// CAUTION: This lookup table must contain constantly increasing values in both the percentage and voltage column.
batteryLookupTableEntry_t batteryLookupTable[] = {
    { .batteryLevel_percent = 0, .batteryVoltage_mV = 3000 },
    { .batteryLevel_percent = 10, .batteryVoltage_mV = 3200 },
    { .batteryLevel_percent = 20, .batteryVoltage_mV = 3400 },
    { .batteryLevel_percent = 30, .batteryVoltage_mV = 3560 },
    { .batteryLevel_percent = 40, .batteryVoltage_mV = 3680 },
    { .batteryLevel_percent = 50, .batteryVoltage_mV = 3800 },
    { .batteryLevel_percent = 60, .batteryVoltage_mV = 3880 },
    { .batteryLevel_percent = 70, .batteryVoltage_mV = 3960 },
    { .batteryLevel_percent = 80, .batteryVoltage_mV = 4040 },
    { .batteryLevel_percent = 90, .batteryVoltage_mV = 4120 },
    { .batteryLevel_percent = 100, .batteryVoltage_mV = 4200 }
};

#define NUM_BATTERY_LOOKUP_TABLE_ENTRIES        (sizeof(batteryLookupTable) / sizeof(batteryLookupTable[0]))

float battery_voltageToPercent(uint16_t batteryVoltage_mV, uint8_t numberFractionalDigits)
{
    // if the voltage is smaller or equal than the first lookup table entry voltage, return the first entry percentage
    if(batteryVoltage_mV <= batteryLookupTable[0].batteryVoltage_mV)
    {
        return (float)batteryLookupTable[0].batteryLevel_percent;
    }
    // if the voltage is greater or equal than the last lookup table entry voltage, return the last entry percentage
    if(batteryVoltage_mV >= batteryLookupTable[NUM_BATTERY_LOOKUP_TABLE_ENTRIES - 1].batteryVoltage_mV)
    {
        return (float)batteryLookupTable[NUM_BATTERY_LOOKUP_TABLE_ENTRIES - 1].batteryLevel_percent;
    }

    // Loop over each range in the lookup table and check if the voltage falls in the current range
    for(uint8_t i = 0; i < (NUM_BATTERY_LOOKUP_TABLE_ENTRIES - 1); i++)
    {
        batteryLookupTableEntry_t entry1 = batteryLookupTable[i];
        batteryLookupTableEntry_t entry2 = batteryLookupTable[i + 1];

        if(batteryVoltage_mV > entry1.batteryVoltage_mV && batteryVoltage_mV <= entry2.batteryVoltage_mV)
        {
            // The multiplier determines the number of fractional digits. 1000=10^3 -> 3 digits, 100=10^2 -> 2 digits ...
            uint16_t multiplier = pow(10, numberFractionalDigits);
            // interpolate the battery voltage
            float batteryPercentage = (float)(map(batteryVoltage_mV, entry1.batteryVoltage_mV, entry2.batteryVoltage_mV, entry1.batteryLevel_percent * multiplier, entry2.batteryLevel_percent * multiplier) / (float)multiplier);
            return batteryPercentage;
        }
    }

    return 0;       // If you get to this line, something with the lookup table seems to be wrong.
}

bool battery_isEmpty(uint16_t batteryVoltage_mV)
{
    uint8_t batteryLevel_percent = battery_voltageToPercent(batteryVoltage_mV);
    return (batteryLevel_percent < BATTERY_EMPTY_PERCENT);
}