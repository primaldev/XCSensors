# XCSensors
XCSensors is an Arduino based vario. It provides telemetry data to any vario based program such as XCSoar or XCTrack via bluetooth or wifi.

### Features:

Wifi streaming for e-ink readers <br>
Bluetooth, Serial or USB OTG streaming<br>
Configuration via Bluetooth, USB or Serial <br>
Dual pressure sensor support <br>
Adaptive Vario* <br>
Humidity sensor <br>
Accelerometer(under development)<br>
Piezo speaker for vario audio <br>
Serial GPS (neo6) <br>

Not all sensors are needed and can be omitted via de config.h macro definition.

*Adaptive Vario: The sensitivity filter of the barometric sensors are adapted according to the environment. Eg. On a quiet day your flight will be more stable, causing the filter to become more sensitive to changes, this way you will be able to detect the lighter thermals.

One feature to single out, is the wifi streaming. Most e-readers do not have bluetooth support and so far the best solution was to physically modify the device. XCSensors act as an WiFi access point with which an e-reader can connect to. It will send the data via UDP broadcast packages. Using UDP the e-reader don't have to send ack messages back and will save power. More than one device can connect to XCSensors and receive data.

Also if needed, switching to Bluetooth streaming is possible via a configuration option.

