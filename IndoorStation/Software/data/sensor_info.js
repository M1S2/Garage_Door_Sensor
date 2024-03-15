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
 
    // Update the pairing status when the buttons of the indoor station are pressed
	source.addEventListener('new_sensor_pairing_status', function(e)
	{
		console.log("new_sensor_pairing_status", e.data);
		var obj = JSON.parse(e.data);
		document.getElementById("pairing_status").style.display = ((obj.pairing_active == true) ? 'block' : 'none');
        document.getElementById("pairing_sensor_number").innerHTML = (obj.pairing_id + 1);
	}, false);
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
