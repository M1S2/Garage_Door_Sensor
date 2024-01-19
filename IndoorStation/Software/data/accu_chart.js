var chart = new Highcharts.Chart(
{
	chart:{ renderTo : 'chart-accu' },
	title: { text: 'Accu Voltage' },
	series: 
	[{
		name: 'Accu Voltage',
		showInLegend: false,
		data: [],
		tooltip: 
		{
			pointFormat: '<b>{point.y:.2f}V</b>'
        }
	}],
	plotOptions: 
	{
		line: 
		{ 
			animation: false,
			dataLabels: 
			{ 
				enabled: true, 
				format: '{y:.2f}'
			}
		},
		series: { color: '#04AA6D' }
	},
	xAxis: 
	{ 
		type: 'datetime',
		dateTimeLabelFormats: { second: '%d.%m.%y %H:%M:%S' }
	},
	yAxis: 
	{
		title: { text: 'Voltage (V)' }
	},
	credits: { enabled: false }
});

setInterval(function ( ) 
{
	var x = (new Date()).getTime(),
	y = parseFloat("3.5") - Math.random();
	if(chart.series[0].data.length > 40)
	{
		chart.series[0].addPoint([x, y], true, true, true);
	} 
	else 
	{
		chart.series[0].addPoint([x, y], true, false, true);
	}
}, 1000 );

/*
setInterval(function ( ) 
{
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function()
	{
		if (this.readyState == 4 && this.status == 200) 
		{
			var x = (new Date()).getTime(),
			y = parseFloat(this.responseText);
			//console.log(this.responseText);
			if(chart.series[0].data.length > 40) 
			{
				chart.series[0].addPoint([x, y], true, true, true);
			} 
			else 
			{
				chart.series[0].addPoint([x, y], true, false, true);
			}
		}
	};
	xhttp.open("GET", "/accu_voltage", true);
	xhttp.send();
}, 1000 );
*/