// Pin state constants (must match backend config.h)
const SENSOR_PIN_STATE_OPEN = false; 	// LOW
const SENSOR_PIN_STATE_CLOSED = true;	// HIGH

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
			const statusElement = templateClone.querySelector('#sensor_X_status');
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
			
			// Fill content
			titleElement.textContent = `#${sensor.index}`;
			statusElement.textContent = sensor.state == SENSOR_PIN_STATE_OPEN ? "Auf" : "Zu";
			percentageElement.innerHTML = `<span>${sensor.percentage}</span> %%`;
			
			const voltageV = (sensor.voltage_mV / 1000.0).toFixed(2);
			voltageElement.innerHTML = `<span>${voltageV}</span> V`;
			
			timestampElement.textContent = sensor.timestamp;
			macElement.textContent = sensor.mac;
			
			// Hide error when MAC is not empty and not 00:00...
			if(sensor.mac !== "00:00:00:00:00:00" && sensor.mac !== "")
			{
				errorDivElement.style.display = "none";
			}
			
			cardsContainer.appendChild(templateClone);
		});
	})
	.catch(error => console.error('Error loading sensor data:', error));
}