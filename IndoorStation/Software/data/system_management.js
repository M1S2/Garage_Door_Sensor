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


function bodyLoaded()
{
	fetch('/get_sensor_status')
	.then(response => response.json())
	.then(data =>
	{
		const cardsContainer = document.querySelector('.cards');
		const template = document.getElementById('sensor-management-card-template');
		
		// Find the first static card (System Info) to insert sensor cards before it
		const firstStaticCard = cardsContainer.querySelector('.card.general');
		
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
			const nameElement = templateClone.querySelector('#sensor_X_name');
			const nameEditBtn = templateClone.querySelector('#sensor_X_name_edit_btn');
			const nameSaveBtn = templateClone.querySelector('#sensor_X_name_save_btn');
			const modeSelectElement = templateClone.querySelector('#sensor_X_mode');
			const modeSelectedElement = templateClone.querySelector('#sensor_X_mode_selectedIndex');
			const numMsgElement = templateClone.querySelector('#sensor_X_num_msg');
			const swVersionElement = templateClone.querySelector('#sensor_X_sw_version');
			const isPairedElement = templateClone.querySelector('#sensor_X_isPaired');
			const useEncryptionElement = templateClone.querySelector('#sensor_X_useEncryption');
			const macElement = templateClone.querySelector('#sensor_X_mac_display');
			
			titleElement.id = `sensor_${sensor.index}_title`;
			nameElement.id = `sensor_${sensor.index}_name`;
			nameEditBtn.id = `sensor_${sensor.index}_name_edit_btn`;
			nameSaveBtn.id = `sensor_${sensor.index}_name_save_btn`;
			modeSelectElement.id = `sensor_mode_${sensor.index}`;
			modeSelectedElement.id = `sensor_mode_selectedIndex_${sensor.index}`;
			numMsgElement.id = `num_msg_${sensor.index}`;
			swVersionElement.id = `sensor_${sensor.index}_sw_version`;
			isPairedElement.id = `sensor_${sensor.index}_isPaired`;
			useEncryptionElement.id = `sensor_${sensor.index}_useEncryption`;
			macElement.id = `sensor_${sensor.index}_mac_display`;
			
			// Update form action sensorIndex
			const nameForm = templateClone.querySelector('form[action="/set_sensor_name"]');
			nameForm.querySelector('input[name="sensorIndex"]').value = sensor.index;

			const modeForm = templateClone.querySelector('form[action="/set_sensor_mode"]');
			modeForm.querySelector('input[name="sensorIndex"]').value = sensor.index;
			
			const removeDataForm = templateClone.querySelector('form[action="/remove_data"]');
			removeDataForm.querySelector('input[name="sensorIndex"]').value = sensor.index;

			const removeSensorForm = templateClone.querySelector('form[action="/remove_sensor"]');
			removeSensorForm.querySelector('input[name="sensorIndex"]').value = sensor.index;
			removeSensorForm.style.display = sensor.isPaired ? 'inline-flex' : 'none';
			
			const downloadForm = templateClone.querySelector('form[action="/download_data"]');
			downloadForm.querySelector('input[name="sensorIndex"]').value = sensor.index;
			const downloadButton = downloadForm.querySelector('button');
			
			// Upload form handling
			const uploadForm = templateClone.querySelector('form[action="/upload_data"]');
			uploadForm.querySelector('input[name="sensorIndex"]').value = sensor.index;
			const fileInput = uploadForm.querySelector('input[type="file"]');
			const uploadButton = uploadForm.querySelector('button');
			uploadButton.onclick = () => fileInput.click();
			
			// Handle Name Edit Buttons
			nameEditBtn.onclick = () => enableNameEdit(sensor.index);

			// Fill content
			titleElement.textContent = `#${sensor.index + 1}`;
			nameElement.value = sensor.name || "";
			modeSelectedElement.value = sensor.mode;
			modeSelectElement.value = sensor.mode;
			numMsgElement.textContent = sensor.numMessages;
			swVersionElement.textContent = sensor.swVersion;
			
			// Update Pairing and Encryption status
			isPairedElement.textContent = sensor.isPaired ? 'link' : 'link_off';
			isPairedElement.style.color = sensor.isPaired ? getSensorColor(sensor.index) : 'var(--error-color)';
			isPairedElement.title = sensor.isPaired ? 'Sensor ist verbunden' : 'Sensor nicht verbunden';
			useEncryptionElement.textContent = sensor.useEncryption ? 'lock' : 'lock_open';
			useEncryptionElement.style.color = sensor.useEncryption ? getSensorColor(sensor.index) : 'var(--error-color)';
			useEncryptionElement.title = sensor.useEncryption ? 'Verschlüsselung aktiv' : 'Keine Verschlüsselung';

			macElement.textContent = sensor.mac;

			// Disable download button if no messages
			downloadButton.disabled = Number(sensor.numMessages) === 0;
			
			// Insert before the first static card (System Info)
			cardsContainer.insertBefore(templateClone, firstStaticCard);
		});
	})
	.catch(error => console.error('Error loading sensor management data:', error));

	// Add events for the battery threshold slider
	const batterySlider = document.getElementById('battery_threshold_slider');
	const batteryValDisplay = document.getElementById('battery_threshold_val');
	batterySlider.oninput = function(e)
	{
		batteryValDisplay.innerText = batterySlider.value;
	};
	batterySlider.onchange = function(e)
	{
		fetch('/set_battery_empty_threshold?threshold=' + batterySlider.value)
	};

	// Load indoor station info
	fetch('/get_indoor_station_info')
	.then(response => response.json())
	.then(data =>
	{
		document.getElementById("indoor_station_sw_version").textContent = data.swVersion;
		document.getElementById("indoor_station_memory_usage").textContent = data.memoryUsage;
		if(data.batteryEmptyThreshold !== undefined)
		{
			batterySlider.value = data.batteryEmptyThreshold;
			batteryValDisplay.innerText = data.batteryEmptyThreshold;
		}
	})
	.catch(error => console.error('Error loading indoor station info:', error));

	updateSystemTime();
	setInterval(updateSystemTime, 1000);

	document.getElementById("webpage_version").innerText = window.GARAGE_DOOR_INDOOR_STATION_WEBPAGE_VERSION;
}

// Load and update system time immediately and periodically
function updateSystemTime()
{
	fetch('/get_system_time')
	.then(response => response.text())
	.then(timeString =>
	{
		document.getElementById("indoor_station_time").textContent = timeString;
	})
	.catch(error => console.error('Error loading system time:', error));
}

function enableNameEdit(index)
{
    const input = document.getElementById('sensor_' + index + '_name');
    const editBtn = document.getElementById('sensor_' + index + '_name_edit_btn');
    const saveBtn = document.getElementById('sensor_' + index + '_name_save_btn');
    
    input.readOnly = false;
    input.focus(); 		// Set focus to directly enable typing
    editBtn.style.display = 'none';
    saveBtn.style.display = 'inline-block';
}
