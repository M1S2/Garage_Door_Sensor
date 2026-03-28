let Num_Sensors = 0;	// Will be loaded from /num_sensors endpoint
var chart_accu;
var chart_pinState;

async function bodyLoaded()
{
	// Load the number of supported sensors from the backend
	try
	{
		const response = await fetch('/num_sensors');
		const data = await response.json();
		Num_Sensors = data;
	}
	catch(error)
	{
		console.error("Error loading num_sensors:", error);
		Num_Sensors = 2;	// Fallback to default
	}

	// Initialize charts with the correct number of sensors
	initializeCharts();

	clearCharts();
	chart_accu.showLoading("Neue Daten über den Button oben laden...");
	chart_pinState.showLoading("Neue Daten über den Button oben laden...");

	restoreSettings();
    setupEventHandlers();
}

function toDatetimeLocalValue(date)
{
    const year = date.getFullYear();
    const month = String(date.getMonth() + 1).padStart(2, "0");
    const day = String(date.getDate()).padStart(2, "0");
    const hours = String(date.getHours()).padStart(2, "0");
    const minutes = String(date.getMinutes()).padStart(2, "0");

    return `${year}-${month}-${day}T${hours}:${minutes}`;
}

function restoreSettings()
{
	const range = sessionStorage.getItem("sensorHistory_range");
    const from = sessionStorage.getItem("sensorHistory_from");
    const to = sessionStorage.getItem("sensorHistory_to");

    if(range === null && from === null && to === null)
    {
		// first call to this website -> set default values
		// default: last 6 months
        const defaultRange = "6m";
        const dateRadioInput = document.querySelector(`input[name="date-range"][value="${defaultRange}"]`);
        if(dateRadioInput !== null)
		{
            dateRadioInput.checked = true;
		}

        const now = new Date();
        const fromDate = new Date(now);
        fromDate.setMonth(fromDate.getMonth() - 6);

        document.getElementById("date-from").value = toDatetimeLocalValue(fromDate);
        document.getElementById("date-to").value = toDatetimeLocalValue(now);
    }
    else
    {
		if(range !== null)
        {
            const dateRadioInput = document.querySelector(`input[name="date-range"][value="${range}"]`);
            if(dateRadioInput !== null)
			{
                dateRadioInput.checked = true;
			}
        }
        if(from !== null)
		{
            document.getElementById("date-from").value = from;
		}
        if(to !== null)
        {
            document.getElementById("date-to").value = to;
        }
    }
    updateDateControlsVisibility();
	updateDayCount();
}

function saveSettings()
{
	const selectedRange = document.querySelector('input[name="date-range"]:checked')?.value ?? "all";
    sessionStorage.setItem("sensorHistory_range", selectedRange);
    sessionStorage.setItem("sensorHistory_from", document.getElementById("date-from").value);
    sessionStorage.setItem("sensorHistory_to", document.getElementById("date-to").value);
}

function setupEventHandlers()
{
	document.querySelectorAll('input[name="date-range"]').forEach(radio =>
	{
		radio.addEventListener('change', function()
		{
			updateDateControlsVisibility();
			saveSettings();
		});
	});

    document.getElementById("date-from").addEventListener("change", function()
	{
		updateDayCount();
		saveSettings();
	});
    document.getElementById("date-to").addEventListener("change", function()
	{
		updateDayCount();
		saveSettings();
	});
}

function updateDateControlsVisibility()
{
	const selectedDateRange = document.querySelector('input[name="date-range"]:checked').value;
	const dateControls = document.getElementById("date-controls");
	dateControls.style.display = (selectedDateRange === 'custom') ? 'flex' : 'none';
}

function updateDayCount()
{
    const date_from = document.getElementById("date-from").value;
    const date_to = document.getElementById("date-to").value;
    if(!date_from || !date_to)
    {
        document.getElementById("day-count").textContent = "";
        return;
    }

    const diffMs = new Date(date_to) - new Date(date_from);
    const diffDays = diffMs / (1000 * 60 * 60 * 24);
    document.getElementById("day-count").textContent = diffDays.toFixed(0) + " Tage";
}

function clearCharts()
{
    chart_accu.series.forEach(s => s.setData([]));
	chart_accu.xAxis[0].setExtremes(null, null);
	chart_accu.redraw();
    chart_pinState.series.forEach(s => s.setData([]));
	chart_pinState.xAxis[0].setExtremes(null, null);
	chart_pinState.redraw();
}

function getFromToDates()
{
	const selectedDateRange = document.querySelector('input[name="date-range"]:checked').value;
	let dateFrom = null;
	let dateTo = null;
	const now = new Date();

	if (selectedDateRange === 'custom')
	{
		dateFrom = document.getElementById('date-from').value;
		dateTo = document.getElementById('date-to').value;
	}
	else if (selectedDateRange !== 'all')
	{
		dateTo = now.toISOString();
		const fromDate = new Date(now);

		if (selectedDateRange === '12m') fromDate.setFullYear(fromDate.getFullYear() - 1);
		if (selectedDateRange === '6m') fromDate.setMonth(fromDate.getMonth() - 6);
		if (selectedDateRange === '3m') fromDate.setMonth(fromDate.getMonth() - 3);
		if (selectedDateRange === '1m') fromDate.setMonth(fromDate.getMonth() - 1);

		dateFrom = fromDate.toISOString();
	}

	return { dateFrom, dateTo };
}

var previous_response_length = 0;
var currentSensorIndex = 0;
function loadData()
{
	let urlPartDates = "";
	const dates = getFromToDates();
	const selectedDateRange = document.querySelector('input[name="date-range"]:checked').value;
	if(selectedDateRange !== 'all' && dates.dateFrom !== null && dates.dateTo !== null)
	{
		const date_from = dates.dateFrom;
		const date_to = dates.dateTo;
		const date_from_Ts = Math.floor(new Date(date_from).getTime() / 1000);
		const date_to_Ts = Math.floor(new Date(date_to).getTime() / 1000);
        urlPartDates = "&from=" + date_from_Ts + "&to=" + date_to_Ts;
    }

	clearCharts();
	if(Num_Sensors == 0)
	{
		chart_accu.showLoading("Keine Sensoren gefunden.");
		chart_pinState.showLoading("Keine Sensoren gefunden.");
		return;
	}
	
	chart_accu.showLoading("Lade...");
	chart_pinState.showLoading("Lade...");

	var xhr = new XMLHttpRequest();
	currentSensorIndex = 0;
	previous_response_length = 0;
	xhr.open("GET", "/get_data?sensorIndex=" + currentSensorIndex + urlPartDates, true);
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
				// Check if timestamp is < 01.01.2000 --> skip this point
				if (x < Date.UTC(2000, 0, 1))
				{
    				console.log("invalid timestamp (earlier than 01.01.2000), skip this element:", obj);
					return false; // Return false to "continue" the "forEach" loop (.some(...))
				}

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
		if (currentSensorIndex < Num_Sensors)
		{
			previous_response_length = 0;
			xhr.open("GET", "/get_data?sensorIndex=" + currentSensorIndex + urlPartDates, true);
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
/* ----- CHARTS ----------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------------------ */

Highcharts.setOptions(
{
	chart:
	{
		zoomType: 'xy',
		backgroundColor: style.getPropertyValue('--background-color'),
		height: 700,
		animation: false
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
/* ----- Chart Creation Functions ----------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------------------ */

function createAccuSeries()
{
	const series = [];
	for (let i = 1; i <= Num_Sensors; i++)
	{
		series.push({
			name: `Sensor #${i}`,
			data: [],
			color: style.getPropertyValue(`--sensor${i}-color`)
		});
	}
	return series;
}

function createPinStateSeries()
{
	const series = [];
	for (let i = 1; i <= Num_Sensors; i++)
	{
		series.push({
			name: `Sensor #${i}`,
			data: [],
			color: style.getPropertyValue(`--sensor${i}-color`),
			step: 'left',
			dataGrouping:
			{
				enabled: false
			}
		});
	}
	return series;
}

function initializeCharts()
{
	// Chart Accu
	chart_accu = new Highcharts.stockChart('chart-accu',
	{
		title:
		{
			text: 'Akku Ladezustand'
		},
		legend:
		{
			enabled: true
		},
		series: createAccuSeries(),
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

	// Chart Pin State
	chart_pinState = new Highcharts.stockChart('chart-pinState',
	{
		title:
		{
			text: 'Tor Status'
		},
		legend:
		{
			enabled: true
		},
		series: createPinStateSeries(),
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
}