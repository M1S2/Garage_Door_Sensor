if (!!window.EventSource)
{
	var source = new EventSource('/events');
 
	source.addEventListener('open', function(e)
	{
		console.log("Events Connected");
	}, false);
	
	source.addEventListener('error', function(e)
	{
		if (e.target.readyState != EventSource.OPEN)
		{
			console.log("Events Disconnected");
		}
	}, false);
 
	source.addEventListener('message', function(e)
	{
		console.log("message", e.data);
	}, false);
}

function bodyLoaded()
{
	// Disable the download data buttons if the number of messages equals 0

	var isNumMsg1Zero = document.getElementById("num_msg_1").innerText == "0";
	document.getElementById("submit_download_1").disabled = isNumMsg1Zero;

	var isNumMsg1Zero = document.getElementById("num_msg_2").innerText == "0";
	document.getElementById("submit_download_2").disabled = isNumMsg1Zero;
}

function submitMacMessage()
{
    alert("Neue MAC Adresse gespeichert.");
    setTimeout(function(){ document.location.reload(false); }, 500);   
}

document.getElementById("mac_sensor_1").addEventListener('keyup', format_macs, false);
document.getElementById("mac_sensor_2").addEventListener('keyup', format_macs, false);

function format_macs(evt)
{ 
    var mac = evt.currentTarget.value;
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
	evt.currentTarget.value = macs.join('').substring(0,17);
}
