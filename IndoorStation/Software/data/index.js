// Pin state constants (must match backend config.h)
const SENSOR_PIN_STATE_OPEN = false; 	// LOW
const SENSOR_PIN_STATE_CLOSED = true;	// HIGH

// SensorModes enum from structures.h
const SENSOR_MODE_NORMAL = 0;           // All messages are received, everything is saved, LED on
const SENSOR_MODE_DISABLED = 1;         // All messages are ignored, nothing is saved, LED off
const SENSOR_MODE_CHARGING = 2;         // All messages are ignored, nothing is saved, LED in different color
const SENSOR_MODE_ONLY_DISPLAY = 3;     // Messages are only displayed via the LED, nothing is saved
const SENSOR_MODE_PAIRING = 4;          // Message from Indoor Station to corresponding sensor to configure the MAC address in the sensor, LED flashes red and blue, nothing is saved

if(window.evtSource)
{
    window.evtSource.close();
}
window.evtSource = new EventSource(SERVER_EVENT_SOURCE);
window.evtSource.addEventListener(SERVER_EVENT_SENSOR_PAIRED, function(event)
{
    console.log("Sensor paired");
    location.reload();
});
window.evtSource.addEventListener(SERVER_EVENT_SENSOR_PAIRING_TIMEOUT, function(event)
{
    console.log("Sensor pairing timeout");
    location.reload();
});
window.evtSource.addEventListener(SERVER_EVENT_SENSOR_NEW_MESSAGE, function(event)
{
    console.log("Sensor new message");
    location.reload();
});
window.evtSource.addEventListener(SERVER_EVENT_SENSOR_MODE_CHANGED, function(event)
{
    console.log("Sensor mode changed");
    location.reload();
});


function getSensorModeContent(sensorMode)
{
	switch(sensorMode)
	{
		case SENSOR_MODE_DISABLED:
			return '<div class="sensor-mode-indicator"><i class="material-symbols-outlined">block</i><br>Deaktiviert</div>';
		case SENSOR_MODE_CHARGING:
			return '<div class="sensor-mode-indicator"><i class="material-symbols-outlined">battery_charging_full</i><br>Laden</div>';
		case SENSOR_MODE_PAIRING:
			return '<div class="sensor-mode-indicator"><i class="material-symbols-outlined">link</i><br>Sensor verbinden</div>';
		default:
			return null; // Show normal content
	}
}

function bodyLoaded()
{
	fetch('/get_sensor_status')
	.then(response => response.json())
	.then(data =>
	{
		const cardsContainer = document.querySelector('.cards');
		const template = document.getElementById('sensor-card-template');
		data.sensors.forEach(sensor =>
		{
			const templateClone = template.content.cloneNode(true);
			
			// Replace X with sensor.index
			const card = templateClone.querySelector('.card');
			card.classList.remove('sensorX');
			card.classList.add(`sensor${sensor.index}`);
			
			// Set the sensor color via CSS custom property
			card.style.setProperty('--sensor-color', getSensorColor(sensor.index));
			
			// Update all element IDs from sensorX_* to sensorN_*
			const titleElement = templateClone.querySelector('#sensor_X_title');
			const statusSubTitleElement = templateClone.querySelector('#sensor_X_status_subTitle');
			const statusElement = templateClone.querySelector('#sensor_X_status');
			const batterySubTitleElement = templateClone.querySelector('#sensor_X_battery_subTitle');
			const percentageElement = templateClone.querySelector('#sensor_X_percentage');
			const voltageElement = templateClone.querySelector('#sensor_X_voltage');
			const timestampElement = templateClone.querySelector('#sensor_X_timestamp');
			const errorDivElement = templateClone.querySelector('#sensor_X_error');
			const macElement = templateClone.querySelector('#sensor_X_mac');
			titleElement.id = `sensor_${sensor.index}_title`;
			statusElement.id = `sensor_${sensor.index}_status`;
			percentageElement.id = `sensor_${sensor.index}_percentage`;
			voltageElement.id = `sensor_${sensor.index}_voltage`;
			timestampElement.id = `sensor_${sensor.index}_timestamp`;
			errorDivElement.id = `sensor_${sensor.index}_error`;
			macElement.id = `sensor_${sensor.index}_mac`;
			
			titleElement.textContent = `#${sensor.index + 1}`;

			// Check if sensor is in special mode
			let modeContent = getSensorModeContent(sensor.mode);
			if(!modeContent && sensor.mode !== SENSOR_MODE_ONLY_DISPLAY && sensor.numMessages === 0)
			{
				modeContent = '<div class="sensor-mode-indicator"><i class="material-symbols-outlined">mail_off</i><br>Keine Sensor Daten</div>';
			}
			if(modeContent)
			{
				// Hide all content elements and show mode indicator
				statusSubTitleElement.style.display = 'none';
				statusElement.style.display = 'none';
				batterySubTitleElement.style.display = 'none';
				percentageElement.style.display = 'none';
				voltageElement.style.display = 'none';
				timestampElement.style.display = 'none';
				errorDivElement.style.display = 'none';
				macElement.style.display = 'none';
				
				// Add mode indicator after title
				const modeDiv = document.createElement('div');
				modeDiv.innerHTML = modeContent;
				modeDiv.className = 'sensor-mode-content';
				titleElement.parentNode.insertBefore(modeDiv, titleElement.nextSibling);
			}
			else
			{
				// Normal mode: show all content
				statusElement.textContent = sensor.state == SENSOR_PIN_STATE_OPEN ? "Auf" : "Zu";
				percentageElement.innerHTML = `<span>${Number(sensor.percentage).toFixed(2)}</span> %`;
				
				const voltageV = (sensor.voltage_mV / 1000.0).toFixed(2);
				voltageElement.innerHTML = `<span>${voltageV}</span> V`;
				
				timestampElement.textContent = sensor.timestamp;
				macElement.textContent = sensor.mac;
				
				// Hide error when MAC is not empty and not 00:00...
				if(sensor.mac !== "00:00:00:00:00:00" && sensor.mac !== "")
				{
					errorDivElement.style.display = "none";
				}
			}
			
			cardsContainer.appendChild(templateClone);
		});
	})
	.catch(error => console.error('Error loading sensor data:', error));
}