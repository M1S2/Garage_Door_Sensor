if (!!window.EventSource)
{
	var source = new EventSource('/events_dashboard');
 
	source.addEventListener('open', function(e)
	{
		console.log("Events Connected (/events_dashboard)");
	}, false);
	
	source.addEventListener('error', function(e)
	{
		if (e.target.readyState != EventSource.OPEN)
		{
			console.log("Events Disconnected (/events_dashboard)");
		}
	}, false);
 
	// Subscribe to all messages from server
	source.addEventListener('message', event => console.log(event.data))

	source.addEventListener('new_readings', function(e)
	{
		console.log("new_readings", e.data);
		var obj = JSON.parse(e.data);
		document.getElementById("state"+obj.id).innerHTML = ((obj.pinState == true) ? 'Zu' : 'Auf');
		document.getElementById("voltage"+obj.id).innerHTML = obj.batteryVoltage_V.toFixed(2);
		document.getElementById("percentage"+obj.id).innerHTML = obj.batteryPercentage.toFixed(0);
		document.getElementById("time"+obj.id).innerHTML = new Date(obj.timestamp * 1000).toLocaleString();
	}, false);
}