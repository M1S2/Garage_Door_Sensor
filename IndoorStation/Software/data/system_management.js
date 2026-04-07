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
			const modeSelectElement = templateClone.querySelector('#sensor_X_mode');
			const modeSelectedElement = templateClone.querySelector('#sensor_X_mode_selectedIndex');
			const numMsgElement = templateClone.querySelector('#sensor_X_num_msg');
			const swVersionElement = templateClone.querySelector('#sensor_X_sw_version');
			const macInputElement = templateClone.querySelector('#sensor_X_mac');
			
			titleElement.id = `sensor_${sensor.index}_title`;
			modeSelectElement.id = `sensor_mode_${sensor.index}`;
			modeSelectedElement.id = `sensor_mode_selectedIndex_${sensor.index}`;
			numMsgElement.id = `num_msg_${sensor.index}`;
			swVersionElement.id = `sensor_${sensor.index}_sw_version`;
			macInputElement.id = `mac_sensor_${sensor.index}`;
			
			// Update form action sensorIndex
			const modeForm = templateClone.querySelector('form[action="/set_sensor_mode"]');
			modeForm.querySelector('input[name="sensorIndex"]').value = sensor.index;
			
			const macForm = templateClone.querySelector('form[action="/set_mac_sensor"]');
			macForm.querySelector('input[name="sensorIndex"]').value = sensor.index;
			
			const removeForm = templateClone.querySelector('form[action="/remove_data"]');
			removeForm.querySelector('input[name="sensorIndex"]').value = sensor.index;
			
			const downloadForm = templateClone.querySelector('form[action="/download_data"]');
			downloadForm.querySelector('input[name="sensorIndex"]').value = sensor.index;
			const downloadButton = downloadForm.querySelector('button');
			
			// Upload form handling
			const uploadForm = templateClone.querySelector('form[action="/upload_data"]');
			uploadForm.querySelector('input[name="sensorIndex"]').value = sensor.index;
			const fileInput = uploadForm.querySelector('input[type="file"]');
			const uploadButton = uploadForm.querySelector('button');
			uploadButton.onclick = () => fileInput.click();
			
			// Fill content
			titleElement.innerHTML = `<i class="material-symbols-outlined">settings</i> #${sensor.index + 1}`;
			modeSelectedElement.value = sensor.mode;
			modeSelectElement.value = sensor.mode;
			numMsgElement.textContent = sensor.numMessages;
			swVersionElement.textContent = sensor.swVersion;
			macInputElement.value = sensor.mac;
			
			// Store original MAC value for comparison
			macInputElement.setAttribute('data-original-mac', sensor.mac);
			// Get the MAC confirm button and set it up
			const macConfirmButton = macForm.querySelector('.mac-confirm-button');
			// Initial state
			updateMacButtonState(macInputElement, macConfirmButton);
			
			// Add MAC formatting and button state update on keyup
			macInputElement.addEventListener('keyup', (e) =>
			{
				format_macs(e);
				updateMacButtonState(macInputElement, macConfirmButton);
			}, false);

			// Update original MAC value after successful save
			macForm.onsubmit = function(e)
			{
				macInputElement.setAttribute('data-original-mac', macInputElement.value);
				updateMacButtonState(macInputElement, macConfirmButton);
				submitMacMessage();
				return true;
			};

			// Disable download button if no messages
			downloadButton.disabled = Number(sensor.numMessages) === 0;
			
			// Insert before the first static card (System Info)
			cardsContainer.insertBefore(templateClone, firstStaticCard);
		});
	})
	.catch(error => console.error('Error loading sensor management data:', error));

	// Load indoor station info
	fetch('/get_indoor_station_info')
	.then(response => response.json())
	.then(data =>
	{
		document.getElementById("indoor_station_mac").textContent = data.mac;
		document.getElementById("indoor_station_sw_version").textContent = data.swVersion;
		document.getElementById("indoor_station_memory_usage").textContent = data.memoryUsage;
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

// Function to update button state based on MAC match
function updateMacButtonState(macInputElement, macConfirmButton)
{
	const currentMac = macInputElement.value;
	const originalMac = macInputElement.getAttribute('data-original-mac');
	const icon = macConfirmButton.querySelector('i');
	
	if (currentMac.toUpperCase() === originalMac.toUpperCase())
	{
		// MAC matches original - show check icon, hide button functionality
		icon.textContent = 'check';
		macConfirmButton.disabled = true;
		macConfirmButton.style.opacity = '0.5';
		macConfirmButton.title = 'MAC Adresse gespeichert';
	}
	else
	{
		// MAC differs - show save icon, enable button
		icon.textContent = 'save';
		macConfirmButton.disabled = false;
		macConfirmButton.style.opacity = '1';
		macConfirmButton.title = 'MAC Adresse speichern';
	}
}

function submitMacMessage()
{
    alert("Neue MAC Adresse gespeichert.");
    setTimeout(function(){ document.location.reload(false); }, 500);   
}

function format_macs(e)
{ 
	if(e.keyCode != 8)		// don't format on backspace (8 == backspace)
	{
		var mac = e.currentTarget.value;
		var macs = mac.split('');
		var colons = [2, 5, 8, 11, 14];
		for (var col in colons)
		{
			if (colons[col] <= macs.length)
			{
				if (macs[colons[col]] !== ":")
				{
					macs.splice(colons[col], 0, ":");
				}
			}
		}
		e.currentTarget.value = macs.join('').substring(0,17);
	}
}
