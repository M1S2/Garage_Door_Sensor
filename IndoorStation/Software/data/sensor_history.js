const NUM_SUPPORTED_SENSORS = 2		// The number of supported sensors. This must match the #define NUM_SUPPORTED_SENSORS

var previous_response_length = 0;
var currentSensorIndex = 0;
function bodyLoaded()
{
	chart_accu.showLoading("Lade...");
	chart_pinState.showLoading("Lade...");

	var xhr = new XMLHttpRequest();
	currentSensorIndex = 0;
	xhr.open("GET", "/get_data?sensorIndex=" + currentSensorIndex, true);
	xhr.onprogress = function ()
	{
		var chunk = xhr.responseText.slice(previous_response_length);
		previous_response_length = xhr.responseText.length;

		// Use .some() instead of .forEach() to be able to break the loop
		chunk.split("}{").some(function(element)
		{
			var originalElementLength = element.length;
			if(!element.startsWith("{")) { element = "{" + element; }
			if(!element.endsWith("}")) { element = element + "}"; }
			console.log("element = " + element);

			try
			{
				var obj = JSON.parse(element);
				if(!("time" in obj) || !("batP" in obj) || !("pin" in obj))
				{
					// property in object is missing. Wait for the rest of the message
					console.log("not all properties found in object.");
					throw new Error("Not all properties found");
				}
				var x = obj.time * 1000;
				chart_accu.series[currentSensorIndex].addPoint([x, obj.batP], false, false, false);
				chart_pinState.series[currentSensorIndex].addPoint([x, obj.pin ? 1 : 0], false, false, false);
				// Return false to "continue" the "forEach" loop (.some(...))
				return false;
			}
			catch(error)
			{
				console.log("uncomplete element received. Rewind position and wait for the rest.")
				// Rewind the chunk position to the beginning of this element (uncomplete parts received)
				previous_response_length -= (originalElementLength + 1);
				// Return true to break the "forEach" loop (.some(...))
				return true;
			}
		});
		// redraw the charts after each chunk of data
		chart_accu.redraw();
		chart_pinState.redraw();
	}
	xhr.addEventListener("load", function(e)
	{
		console.log("completed: sensor #" + currentSensorIndex);
		currentSensorIndex++;
		if (currentSensorIndex < NUM_SUPPORTED_SENSORS)
		{
			previous_response_length = 0;
			xhr.open("GET", "/get_data?sensorIndex=" + currentSensorIndex, true);
			xhr.send();
		}
		else
		{
			chart_accu.hideLoading();
			chart_pinState.hideLoading();

			// Click the "All" button (index 4 in the rangeSelector) after loading to make sure, everything is shown
			// https://www.highcharts.com/forum/viewtopic.php?t=27631
			chart_accu.rangeSelector.clickButton(4, { type:'all' }, true);
			chart_pinState.rangeSelector.clickButton(4, { type:'all' }, true);
		}
	});
	xhr.send();
}

var style = getComputedStyle(document.body);

/* ------------------------------------------------------------------------------------------------------ */

Highcharts.setOptions(
{
	chart:
	{
		zoomType: 'xy',
		backgroundColor: style.getPropertyValue('--background-color'),
		height: 700
	},
	title:
	{
		style:
		{
			color: style.getPropertyValue('--text-color')
		}
	},
	legend:
	{
		itemStyle:
		{
			color: style.getPropertyValue('--text-color')
		}
	},
	time:
	{
		useUTC: false
	},
	plotOptions:
	{
		series:
		{
			animation: false,
			tooltip:
			{
				xDateFormat: '%d.%m.%y %H:%M:%S'
			},
			marker:
			{
				enabled: true,
				radius: 3
			}
		},
		line:
		{
			animation: false,
			dataLabels:
			{
				enabled: false
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
		opposite: false,
		labels:
		{
			style:
			{
				color: style.getPropertyValue('--text-color')
			}
		},
		title:
		{
			style:
			{
				color: style.getPropertyValue('--text-color')
			}
		},
		gridLineColor: style.getPropertyValue('--chart-grid-line-color')
	},
	loading:
	{
		labelStyle:
		{
			color: style.getPropertyValue('--chart-loading-text-color'),
			fontSize: '30px'
		},
		style:
		{
			backgroundColor: style.getPropertyValue('--chart-loading-background')
		}
	},
	credits: { enabled: false },
	rangeSelector:
	{
		buttonTheme: 
		{
			fill: 'none',
			stroke: 'none',
			'stroke-width': 0,
			r: 8,
			width : 60,
			style:
			{
				// Default text color
				color: style.getPropertyValue('--text-color')
			},
			states:
			{
				hover: 
				{
					// Mouse hovertext color (style.color) and background color (fill)
					fill: style.getPropertyValue('--mouse-over-background'),
					style: 
					{
						color: style.getPropertyValue('--mouse-over-text-color')
					}
				},
				select: 
				{
					// Selected button text color (style.color) and background color (fill)
					fill: style.getPropertyValue('--accent1-color'),
					style: 
					{
						color: style.getPropertyValue('--text-color')
					}
				},
				disabled:
				{ 
					style:
					{
						// Disabled text color
						color: style.getPropertyValue('--chart-grid-line-color')
					}
				}
			}
		},
		buttons:
		[{
			type: 'week',
			count: 1,
			text: '1 Woche',
			title: '1 Woche anzeigen'
		},{
			type: 'month',
			count: 1,
			text: '1 Monat',
			title: '1 Monat anzeigen'
		},
		{
			type: 'month',
			count: 6,
			text: '6 Monate',
			title: '6 Monate anzeigen'
		},
		{
			type: 'year',
			count: 1,
			text: '1 Jahr',
			title: '1 Jahr anzeigen'
		}, 
		{
			type: 'all',
			text: 'Alles',
			title: 'Alles anzeigen'
		}]
	}
});

/* ------------------------------------------------------------------------------------------------------ */

var chart_accu = new Highcharts.stockChart('chart-accu',
{
	title:
	{
		text: 'Akku Ladezustand'
	},
	legend:
	{
		enabled: true
	},
	series:
	[{
		name: 'Sensor #1',
		data: [],
		color: style.getPropertyValue('--sensor1-color')
	},
	{
		name: 'Sensor #2',
		data: [],
		color: style.getPropertyValue('--sensor2-color')
	}],
	plotOptions:
	{
		series:
		{
			tooltip:
			{
				pointFormat: '<b>{point.y:.1f}%</b>'
			}
		}
	},
	xAxis:
	{
		ordinal: false
	},
	yAxis:
	{
		title:
		{
			text: 'Ladezustand (%)'
		}
	}
});

/* ------------------------------------------------------------------------------------------------------ */

var chart_pinState = new Highcharts.stockChart('chart-pinState',
{
	title:
	{
		text: 'Tor Status'
	},
	legend:
	{
		enabled: true
	},
	series:
	[{
		name: 'Sensor #1',
		data: [],
		color: style.getPropertyValue('--sensor1-color')
	},
	{
		name: 'Sensor #2',
		data: [],
		color: style.getPropertyValue('--sensor2-color')
	}],
	xAxis:
	{
		ordinal: false
	},
	yAxis:
	{
		title:
		{
			text: 'Tor Status'
		},
		labels:
		{
			formatter: function () 
			{
				return (this.value == 0 ? 'Auf' : (this.value == 1 ? 'Zu' : ''));
			}
		}
	}
});