var previous_response_length = 0;
var currentSensorIndex = 0;
function bodyLoaded()
{
	//chart_accu.series[0].setData([]);
	//chart_accu.series[1].setData([]);
	//chart_pinState.series[0].setData([]);
	//chart_pinState.series[1].setData([]);

	chart_accu.showLoading();
	chart_pinState.showLoading();

	var xhr = new XMLHttpRequest();
	currentSensorIndex = 0;
	xhr.open("GET", "/get_data?sensorIndex=" + currentSensorIndex, true);
	xhr.addEventListener("load", function(e)
	{ 
		console.log("completed: sensor #" + currentSensorIndex);
		currentSensorIndex++;
		if(currentSensorIndex <= 1)
		{
			xhr.open("GET", "/get_data?sensorIndex=" + currentSensorIndex, true);
			xhr.send();
		}
		else
		{
			chart_accu.hideLoading();
			chart_pinState.hideLoading();
		}
	});
	xhr.onprogress = function ()
	{
		var chunk = xhr.responseText.slice(previous_response_length);
		previous_response_length = xhr.responseText.length;
		
		chunk.split("}{").forEach((element) => 
		{
			if(!element.startsWith("{")) { element = "{" + element; }
			if(!element.endsWith("}")) { element = element + "}"; }
			console.log("element = " + element);
		
			var obj = JSON.parse(element);
			var x = obj.timestamp * 1000;
			if(obj.sensorId >= 0 && obj.sensorId <= 1)
			{
				chart_accu.series[obj.sensorId].addPoint([x, obj.batteryPercentage], true, false, false);
				chart_pinState.series[obj.sensorId].addPoint([x, obj.pinState ? 1 : 0], true, false, false);
			}
		});
	}
	xhr.send();
}

var style = getComputedStyle(document.body);

Highcharts.setOptions(
{
    time:
	{
        timezone: 'Europe/Berlin'
    }
});

var chart_accu = new Highcharts.Chart(
{
	chart:
	{ 
		renderTo : 'chart-accu',
        zoomType: 'xy',
		backgroundColor: style.getPropertyValue('--background-color'),
	},
	title: 
	{ 
		text: 'Akku Ladezustand',
		style:
		{
            color: style.getPropertyValue('--text-color')
        }
	},
	series: 
	[{
		name: 'Akku Ladezustand #1',
		showInLegend: false,
		data: [],
		tooltip: 
		{
			pointFormat: '<b>{point.y:.1f}%</b>'
        },
		color: style.getPropertyValue('--sensor1-color')
	},
	{
		name: 'Akku Ladezustand #2',
		showInLegend: false,
		data: [],
		tooltip: 
		{
			pointFormat: '<b>{point.y:.1f}%</b>'
        },
		color: style.getPropertyValue('--sensor2-color')
	}],
	plotOptions: 
	{
		series: 
		{
			animation: false
		},
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
		labels:
		{
			style:
			{
				color: style.getPropertyValue('--text-color')
			}
		},
		type: 'datetime',
		dateTimeLabelFormats: { second: '%d.%m.%y %H:%M:%S' },
		lineColor: style.getPropertyValue('--text-color'),
		tickColor: style.getPropertyValue('--text-color')
	},
	yAxis: 
	{
		labels:
		{
			style:
			{
				color: style.getPropertyValue('--text-color')
			}
		},
		title: 
		{ 
			text: 'Ladezustand (%)',
			style:
			{
				color: style.getPropertyValue('--text-color')
			}
		},
		gridLineColor: style.getPropertyValue('--chart-grid-line-color')
	},
	credits: { enabled: false }
});


var chart_pinState = new Highcharts.Chart(
{
	chart:
	{ 
		renderTo : 'chart-pinState',
        zoomType: 'xy',
		backgroundColor: style.getPropertyValue('--background-color')
	},
	title:
	{ 
		text: 'Tor Status',
		style:
		{
            color: style.getPropertyValue('--text-color')
        }
	},
	series: 
	[{
		name: 'Tor Status #1',
		showInLegend: false,
		data: [],
		color: style.getPropertyValue('--sensor1-color')
	},
	{
		name: 'Tor Status #2',
		showInLegend: false,
		data: [],
		color: style.getPropertyValue('--sensor2-color')
	}],
	plotOptions: 
	{
		series: 
		{
			animation: false
		},
		line: 
		{ 
			animation: false,
			dataLabels: 
			{ 
				enabled: true,
				formatter: function ()
				{
					return (this.point.y == 0 ? 'Auf' : (this.point.y == 1 ? 'Zu' : ''));
				}
			}
		}
	},
	xAxis: 
	{ 
		labels:
		{
			style:
			{
				color: style.getPropertyValue('--text-color')
			}
		},
		type: 'datetime',
		dateTimeLabelFormats: { second: '%d.%m.%y %H:%M:%S' },
		lineColor: style.getPropertyValue('--text-color'),
		tickColor: style.getPropertyValue('--text-color')
	},
	yAxis: 
	{
		title: 
		{ 
			text: 'Tor Status',
			style:
			{
				color: style.getPropertyValue('--text-color')
			}
		},
		labels: 
		{
			style:
			{
				color: style.getPropertyValue('--text-color')
			},
			formatter: function () 
			{
				return (this.value == 0 ? 'Auf' : (this.value == 1 ? 'Zu' : ''));
			}
		},
		gridLineColor: style.getPropertyValue('--chart-grid-line-color')
	},
	credits: { enabled: false }
});