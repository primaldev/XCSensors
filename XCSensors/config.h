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
  ESP8266 -> SerialESP
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

  Notes:
  Lipo battery can cause voltage drops on the USB supply and make the processor unstable.

*/

// Compiler values
// 

//#define DEBUG
#define CONFIGOPT //enable configuration option (EEPROM required) Teensy LC EEPROM small 128 bytes.
#define HUMANCONFIG //display human readable config menu. Values are entered as 1=on, one at a time. If disabled,
// it can be used by an external app and multiple values can be sent, handy for programs eg: <1=on>

#define SERIALGPS Serial2
#define GPS
#define GPSSERIALEVENT serialEvent2 //use SerialEvent()
#define SERIALGPSBAUD 9600 // serial1 speed

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

#define SERIALESP Serial3
#define ESPWIFI
#define ESPAT  //use AT commands
#define ESPWIFIBAUD 115200 //Softserial of the ESP8266 can't handle high baud rates
#define WIFIEN_PIN 12 //wifi enable pin 

#define WIFISSID "XCSensors" // change this 
#define WIFIPASSWORD "thereisnospoon"

#define DHT
#define DHTTYPE DHT11   //for the library
#define MAG 
#define DHT11_PIN 17
#define ACCL 
#define ACCLREADMS 100 //how often to read the accelerom
#define ACCLSMOOTH 10 //Lowpass filter level

//Enable Serial Bluetooth
//SerialOut can be used to connect to a Kobo serial port or a HC-05, no changes needed
#define SERIAL_MAIN Serial
//#define SERIAL_MAINBAUD 38400  //do not use with Serial (USB) it will hang
/*Program the HC-05/06 via AT commands first (use the ftdi, press the use prog button, program baud is always 38400)
  AT+UART=38400,1,0 //115200 had problems
  AT+NAME=iXsensors0A 
  AT+PSWD=1234
*/


/* 
  As a workaround for abysmally bad standardized method of NMEA sentences, different sentences can be sent
  to different ports. This way you can implement the proprietary drivers for the different sensors 
  (LXNAV for the vario and Vega for the humid sensor). The passthrough option in XCSoar does not always work.
*/
#define CHANNEL1PORT 4353  //udp ports for the designated channels
#define CHANNEL2PORT 10110
#define CHANNEL3PORT 4352
#define CHANNEL4PORT 2000 //depending on esp firmware, the 4rth channel might not be available



//#define VREFCAL 1126400L //Calibration voltage level. see https://github.com/rlogiacco/VoltageReference

#define BUZZER //let's go beep
#define BUZZPIN 16 //board pin 
//#define TESTBUZZER  //simulate the vario sound for testing only




////////////////////set Fixes/////////////////////////


#if !defined(GPS)
#undef GPSSERIALEVENT
#endif

#if !defined(GPSSERIALEVENT)
#undef GPSTIMER
#endif

#if !defined(ESPWIFI)
#undef ESPAT
#endif

