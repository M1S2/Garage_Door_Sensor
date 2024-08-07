function bodyLoaded()
{
	// Disable the download data buttons if the number of messages equals 0

	var isNumMsg1Zero = document.getElementById("num_msg_1").innerText == "0";
	document.getElementById("submit_download_1").disabled = isNumMsg1Zero;

	var isNumMsg2Zero = document.getElementById("num_msg_2").innerText == "0";
	document.getElementById("submit_download_2").disabled = isNumMsg2Zero;

	document.getElementById("sensor_mode_1").value = document.getElementById("sensor_mode_selectedIndex_1").value;
	document.getElementById("sensor_mode_2").value = document.getElementById("sensor_mode_selectedIndex_2").value;

	document.getElementById("webpage_version").innerText = window.GARAGE_DOOR_INDOOR_STATION_WEBPAGE_VERSION;
}

function submitMacMessage()
{
    alert("Neue MAC Adresse gespeichert.");
    setTimeout(function(){ document.location.reload(false); }, 500);   
}

document.getElementById("mac_sensor_1").addEventListener('keyup', format_macs, false);
document.getElementById("mac_sensor_2").addEventListener('keyup', format_macs, false);

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
