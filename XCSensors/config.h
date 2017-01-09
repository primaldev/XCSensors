/*
  XCsensors by Marco van Zoest

  www.primaldev.nl
  www.primalcode.nl

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

/*

  Config 1:
  GPS -> HW Serial
  BT -> SerialBT
  ESP8266 -> SerialEPS
  Config options always via BT and with bluetoothOnly disabled in the config options, data will go via Wifi

  Config 2:
  GPS -> HW Serial
  BT -> SerialBT
  No wifi, data and config options via Bluetooth

  Config 3:
  GPS -> HW Serial
  Kobo serial -> SerialBT
  No wifi, data and config options via SerialBT  direct connection to Kobo serial port

  Config 4:
  GPS -> Kobo Serial port 1
  Kobo Serial port 2 -> HW Serial
  No wifi, SerialBT disabled. Config and data via Pro mini HW Serial port. Kobo with more than one serial port needed (Kobo glo)

*/

// Compiler values
// Trinket pro: Pins #2 and #7 are not available (they are exclusively for USB)

//#define DEBUG
#define CONFIGOPT //enable configuration option (EEPROM required)
#define HUMANCONFIG //display human readable config menu. Values are entered as 1=on, one at a time. If disabled,
// it can be used by an external app and multiple values can be sent, handy for programs eg: <1=on>

#define GPS
#define GPSSERIALEVENT //use Arduino SerialEvent()
#define GPSTIMER //Use GPS pulse to send data. 
#define SERIAL1BAUD 9600 // serial1 speed

#define VARIO
#define VARIO2 //if 2nd vario
#define VARIO2LEASTDEV //base dual vario on least deviation
#define PTASAVERAGE //include vario avarage in ptas sentence (ignored by XCSoar)
#define VARIOREADMS 40//40 //read vario every n ms. this is handy for the faster processors. Value must be lower than timedNmea6
#define BAROADDR 0x77
#define BAROADDR2 0x76

/*
 * Adative vario will automatacally adjust the lowpass filter by changing setting conf.variosmooth.
 * It does it by detecting "triggers". 
 */
#define ADAPTIVEVARIO //Adapts the vario low pass filter aka. 
#define ADVLOWTRIGGER 0.08 // level at witch low trigger are checked

#define EPSWIFI
#define EPSAT  //use AT commands
#define EPSWIFIBAUD 115200 //Softserial of the ESP8266 can't handle high baud rates

#define WIFIRX_PIN 10//2 //ESP8266 PIn
#define WIFITX_PIN 9//3
#define WIFIEN_PIN 4//3 //wifi channel enable pin

#define DHT
#define DHTTYPE DHT11   //for the library
#define MAG 
#define DHT11_PIN 3
#define ACCL 
#define ACCLREADMS 100 //how often to read the accelerom
#define ACCLSMOOTH 10 //Lowpass filter level

//Enable Serial Bluetooth
//SerialBT can also be used to connect to a Kobo serial port instead of a HC-05, no changes needed
#define SERIALBT  //output will revert to Serial if disabled (no gps support possible)
#define SERIALBTBAUD 38400
/*Program the HC-05/06 via AT commands first (use the ftdi, press the use prog button, program baud is always 38400)
  AT+UART=38400,1,0 //115200 had problems
  AT+NAME=iXsensors0A 
  AT+PSWD=1234
*/

#define BTRX_PIN 12 //7  // The serial pins for Bluetooth
#define BTTX_PIN 11 //8

/* 
  As a workaround for abysmally bad standardized method of NMEA sentences, different sentences can be sent
  to different ports. This way you can implement the proprietary drivers for the different sensors 
  (LXNAV for the vario and Vega for the humid sensor). The passthrough option in XCSoar does not always work.
*/
#define CHANNEL1PORT 4353  //udp ports for the designated channels
#define CHANNEL2PORT 10110
#define CHANNEL3PORT 4352
#define CHANNEL4PORT 2000 //depending on esp firmware, the 4rth channel might not be available



#define VREFCAL 1126400L //Calibration voltage level. see https://github.com/rlogiacco/VoltageReference

#define BUZZER //let's go beep
#define BUZZPIN 6 //board pin 
//#define TESTBUZZER  //simulate the vario sound for testing only




////////////////////set Fixes/////////////////////////

#if !defined(SERIALBT) && !defined(GPS)
#define SERIAL1OUTONLY
#endif

#if !defined(GPS)
#undef GPSSERIALEVENT
#endif

#if !defined(GPSSERIALEVENT)
#undef GPSTIMER
#endif

#if !defined(EPSWIFI)
#undef EPSAT
#endif

