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
 
	source.addEventListener('new_readings', function(e)
	{
		console.log("new_readings", e.data);
		var obj = JSON.parse(e.data);
		document.getElementById("s"+obj.id).innerHTML = ((obj.pinState == true) ? 'Closed' : 'Open');
		document.getElementById("v"+obj.id).innerHTML = obj.voltageVcc.toFixed(2);
	}, false);
}