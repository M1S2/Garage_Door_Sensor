if (!!window.EventSource)
{
	var source_sensorHistory = new EventSource('/events_sensorHistory');
 
	source_sensorHistory.addEventListener('open', function(e)
	{
		console.log("Events Connected (/events_sensorHistory)");
	}, false);
	
	source_sensorHistory.addEventListener('error', function(e)
	{
		if (e.target.readyState != EventSource.OPEN)
		{
			console.log("error", e.target.readyState);
			console.log("Events Disconnected (/events_sensorHistory)");
		}
	}, false);
 
	// Subscribe to all messages from server
	source_sensorHistory.addEventListener('message', event => console.log(event.data))

	source_sensorHistory.addEventListener('clear_sensorHistory', function(e)
	{
		console.log("clear_sensorHistory");
		
		//If you want to remove all points without remove the serie you can use 
		chart_accu.series[0].setData([]);
		chart_accu.series[1].setData([]);
		chart_pinState.series[0].setData([]);
		chart_pinState.series[1].setData([]);
	}, false);

	source_sensorHistory.addEventListener('new_sensorHistory', function(e)
	{
		console.log("new_sensorHistory", e.data);
		var obj = JSON.parse(e.data);
		
		var x = obj.timestamp * 1000;
		if(obj.sensorId >= 0 && obj.sensorId <= 1)
		{
			chart_accu.series[obj.sensorId].addPoint([x, obj.batteryPercentage], true, false, true);
			chart_pinState.series[obj.sensorId].addPoint([x, obj.pinState ? 1 : 0], true, false, true);
		}
	}, false);
}

var chart_accu = new Highcharts.Chart(
{
	chart:
	{ 
		renderTo : 'chart-accu',
        zoomType: 'xy'
	},
	title: { text: 'Accu Percentage' },
	series: 
	[{
		name: 'Accu Percentage #1',
		showInLegend: false,
		data: [],
		tooltip: 
		{
			pointFormat: '<b>{point.y:.1f}%</b>'
        },
		color: '#FD7E14'
	},
	{
		name: 'Accu Percentage #2',
		showInLegend: false,
		data: [],
		tooltip: 
		{
			pointFormat: '<b>{point.y:.1f}%</b>'
        },
		color: '#1B78E2'
	}],
	plotOptions: 
	{
		line: 
		{ 
			animation: false,
			dataLabels: 
			{ 
				enabled: true, 
				format: '{y:.1f}'
			}
		}
	},
	xAxis: 
	{ 
		type: 'datetime',
		dateTimeLabelFormats: { second: '%d.%m.%y %H:%M:%S' }
	},
	yAxis: 
	{
		title: { text: 'Percentage (%)' }
	},
	credits: { enabled: false }
});


var chart_pinState = new Highcharts.Chart(
{
	chart:
	{ 
		renderTo : 'chart-pinState',
        zoomType: 'xy'
	},
	title: { text: 'Door State' },

	series: 
	[{
		name: 'Door State #1',
		showInLegend: false,
		data: [],
		color: '#FD7E14'
	},
	{
		name: 'Door State #2',
		showInLegend: false,
		data: [],
		color: '#1B78E2'
	}],
	plotOptions: 
	{
		line: 
		{ 
			animation: false,
			dataLabels: 
			{ 
				enabled: true,
				formatter: function () {
					return (this.point.y == 0 ? 'Open' : (this.point.y == 1 ? 'Closed' : ''));
				}
			}
		}
	},
	xAxis: 
	{ 
		type: 'datetime',
		dateTimeLabelFormats: { second: '%d.%m.%y %H:%M:%S' }
	},
	yAxis: 
	{
		title: { text: 'Door State' },
		labels: {
			formatter: function () {
				return (this.value == 0 ? 'Open' : (this.value == 1 ? 'Closed' : ''));
			}
		}
	},
	credits: { enabled: false }
});