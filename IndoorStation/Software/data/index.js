function bodyLoaded()
{
	// Disable the warning if the MAC of the sensor is set

	var isMac1Zero = document.getElementById("mac_sensor_1").innerText == "00:00:00:00:00:00";
	document.getElementById("error_sensor_1").style.display = isMac1Zero ? "inline" : "none";

    var isMac2Zero = document.getElementById("mac_sensor_2").innerText == "00:00:00:00:00:00";
	document.getElementById("error_sensor_2").style.display = isMac2Zero ? "inline" : "none";
}