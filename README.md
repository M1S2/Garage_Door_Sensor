# Garage Door Sensor

This is a project to send the status of multiple garage doors (or equal) to one indoor station. This is useful if the door can't be seen from inside.
Each device (sensor and indoor station) uses an ESP12-F (ESP8266) controller that runs the software and is the WiFi transmitter. The data between the sensor and the indoor station is transmitted using ESP-Now. The indoor station acts as a gateway between ESP-Now and WiFi.

<<<<< Image of sensor and indoor station >>>>>

## Building the Sensor
You need to build one sensor per door to monitor. The current version of the indoor station is capable of displaying 2 sensors at the same time. If you need more sensors, the indoor station must be adapted. No changes to the sensor are necessary.
### PCBs
The PCBs are designed using Autodesk Fusion360 to be manufactured with toner transfer method. All design files are located in the `Sensor\Electrical` subfolder.
### Housing
TBC...
### Software
TBC... (configuration, flashing, development environment)

## Building the Indoor Station
TBC...

## Usage
TBC...

## Developer Hints
- The sensor is designed to consume as less power as possible. Therefore the ESP8266 on the sensor is only switched on when the pin state changed and only as long until the data is sent to the indoor station.
- The sensor trys to send the data on all available WiFi channels until success. The most common used channels are tried first.

TBC...