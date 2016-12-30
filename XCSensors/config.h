/*
XCsensors by Marco van Zoest

www.primaldev.nl
www.primalcode.nl

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version. see <http://www.gnu.org/licenses/>
*/
// Compiler values
// Trinket pro: Pins #2 and #7 are not available (they are exclusively for USB)

//#define DEBUG
#define GPS
#define GPSTIMER //Use GPS pulse to send data. 
#define GPSSERIAL 9600 //GPS serial speed

#define VARIO 
#define VARIO2 //if 2nd vario
#define VARIOREADMS 40 //read vario every n ms. this is handy for the faster processors. Value must be lower than timedNmea6
#define BAROADDR 0x77
#define BAROADDR2 0x76

#define EPSWIFI
#define EPSAT  //use AT commands
#define EPSWIFIBAUD 115200 //Softserial of the ESP8266 can't handle high baud rates

#define WIFIRX_PIN 10//2 //ESP8266 PIn
#define WIFITX_PIN 9//3
#define WIFIEN_PIN 4//3 //wifi channel enable pin

#define DHT11
#define DHTTYPE DHT11
#define MAG
#define DHT11_PIN 3
#define ACCL 
#define ACCLREADMS 100 //how often to read the accelerom

//Enable Serial Bluetooth
#define SERIALBT
#define SERIALBTBAUD 38400 
//Program the HC-05/06 via AT commands first (use the ftdi, press the use prog button, program baud is always 38400) 
                            //AT+UART=38400,1,0 //115200 had problems
                            //AT+NAME=iXsensors0A
                            //AT+PSWD=1234

//define SERIALOUT                           
               
#define BTRX_PIN 12 //7  // The serial pins for Bluetooth
#define BTTX_PIN 11 //8

// Due to the abysmally bad standardized method of NMEA sentences, everyone ended up doing their own thing
// resulsting in a complete an utter disorganized mess. 
// As a workaround different sentences can be sent to different ports. This way you can implement the
// proprietary drivers for the different sensors (LXNAV for the vario and Vega for the humid sensor)
// The passthrough option in XCSoar does not work so well.
#define CHANNEL1PORT 4353  //udp ports for the designated channels
#define CHANNEL2PORT 10110
#define CHANNEL3PORT 4352
#define CHANNEL4PORT 2000 //depending on esp firmware, the 4rth channel might not be available

#define ACCLSMOOTH 7 //TODO: change to lowpass filter

#define VREFCAL 1126400L //Calibration voltage level. see https://github.com/rlogiacco/VoltageReference

#define BUZZER //let's go beep
#define BUZZPIN 6 //board pin 
//#define TESTBUZZER //simulate the vario sound


