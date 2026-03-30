// Shared utility functions

function getSensorColor(sensorIndex)
{
	// Read sensor colors from CSS custom properties
	// Fallback through colors if more sensors than defined colors
	const root = document.documentElement;
	const colorVar = `--sensor${sensorIndex % 8 + 1}-color`;
	return getComputedStyle(root).getPropertyValue(colorVar).trim();
}
