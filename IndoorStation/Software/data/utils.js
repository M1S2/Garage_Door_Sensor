// Shared constants

const SERVER_EVENT_SOURCE =                   "/events"
const SERVER_EVENT_SENSOR_PAIRED =            "sensorPaired"
const SERVER_EVENT_SENSOR_PAIRING_TIMEOUT =   "sensorPairingTimeout"
const SERVER_EVENT_SENSOR_NEW_MESSAGE =       "newSensorMessage"
const SERVER_EVENT_SENSOR_MODE_CHANGED =      "sensorModeChanged"

// Shared utility functions

function getSensorColor(sensorIndex)
{
	// Read sensor colors from CSS custom properties
	// Fallback through colors if more sensors than defined colors
	const root = document.documentElement;
	const colorVar = `--sensor${sensorIndex % 8 + 1}-color`;
	return getComputedStyle(root).getPropertyValue(colorVar).trim();
}
