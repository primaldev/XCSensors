# XCSensors
XCSensors is an Arduino based vario. It provides telemetry data to any vario based program such as XCSoar or XCTrack via bluetooth or wifi.

### Features:

Wifi streaming for e-ink reader.
Bluetooth streaming.
Configuration via Bluetooth
Dual pressure sensor support
Humidity sensor
Gyro compass
Accelerometer
Piezo speaker for vario audio

Not all sensors are needed and can be omitted via de config.h macro definition.

One major feature is the wifi streaming. Most e-readers do not have bluetooth support and so far the best solution was to physically modify the device. XCSensors act as an WiFi access point with which an e-reader can connect to. It will send the data via UDP broadcast packages. Using UDP the e-reader don't have to send ack messages back and will save power. More than one device can connect to XCSensors and receive data.

Also if needed, switching to Bluetooth streaming is possible via a configuration option.
