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