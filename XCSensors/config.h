/*
  XCSensors http://XCSensors.org
  
  Copyright (c), PrimalCode (http://www.primalcode.org)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/


/////////////////////////////
// MCU type
////////////////////////////

//There can be only one
//Don't forget to change Arduino Settings when changing boards

//#define STM32F
#define TEENSY

/////////////////////////////
// Config Type
////////////////////////////
//Switch between different configurations

//#define WIFIBOX_BT //Wifi box configuration
#define KOBO_BT //Kobo intergrated with extra Bluetooth module

///////////////////////////
// Developer Options
///////////////////////////

//#define DEBUG
//#define DEBUGSERIAL Serial
//#define EEPROMDEVMODE //For developement. will reset the eeprom on every startup
#define EEPROMPVERSION 2  //If the eeprom needs to be resetted for a new release version, increment this value

///////////////////////////
// General Options
///////////////////////////

#define STARTDELAY 8000 //the time delay before the process starts

#define HUMANCONFIG //display human readable config menu. Values are entered as 1=on, one at a time. If disabled,
// it can be used by an external app and multiple values can be sent, handy for programs eg: <1=on>

//#define GPSSERIALEVENT serialEvent2 //use SerialEvent(). Carefull as this might interupt other nmea sentences

#define TAKEOFFVARIO 0.8 //0.4 //abs vario level to detect takeoff
#define BUZZERZEROCLIMB -0.3 // Normal sink rate for glider is -0.9 m/s. At this value up to 0 m/s a "blip" sound is made
#define BUZZERVARIOSTOP 30000 //time vario STOP making noise when climbrate 0 m/s 
//#define TESTBUZZER  //simulate the vario sound for testing only
#define BUZZERCYCLE 100 //buzzer causes delays so it is only triggerd every n'th cycle. Value depens on cpu clock. 
#define OUTOFTHERMALBUZZT 3000 //time buzzer goes buuhhhhh

#define VARIO2LEASTDEV //base dual vario on least deviation
#define PTASAVERAGE //include vario avarage in ptas sentence (ignored by XCSoar)
#define VARIOREADMS 20//40 //read vario every n ms. this is handy for the faster processors. Value must be lower than timedNmea10
#define SOARDETECTION 30000 // if climbrate is constant for set milliseconds at 0 m/s the buzzer is muted
/*
   Adative vario will automatacally adjust the lowpass filter by changing setting conf.variosmooth.
   It does it by detecting "triggers".
*/
#define ADAPTIVEVARIO //Adapts the vario low pass filter 

#define ACCLREADMS 100 //how often to read the accelerom
#define ACCLSMOOTH 10 //Lowpass filter level
#define ACCLOFFSET -0.93 //finetune Accl offset (includes -1G for gravity)

//#define ALLFASTDATA //send all data at 10Hz. DMA channels reccomended

/*
  As a workaround for the limited method of NMEA sentences, different sentences can be sent
  to different ports. This way you can implement the proprietary drivers for the different sensors
  (LXNAV for the vario and Vega for the humid sensor). The passthrough option in XCSoar does not always work.
*/
#define CHANNEL1PORT 4353  //udp ports for the designated channels
#define CHANNEL2PORT 10110
#define CHANNEL3PORT 4352
#define CHANNEL4PORT 2000 //depending on esp firmware, the 4rth channel might not be available



/////////////////////////////
// MCU Board STM32F
/////////////////////////////

#if defined(STM32F)

/////////////////////////////////////////////////////////////////////
//STM32F103R Kobo Integrated
/////////////////////////////////////////////////////////////////////

#if defined(KOBO_BT)
#define CONFIGOPT //enable configuration option (EEPROM required)
#define I2CEEPROM 0x50 ////External I2C EEPROM, required if using  CONFIGOPT on STM32F
#define I2CEEPROMPAGE 64 // page size of EEPROM 128 for 512k. 64 for 256k

#define LEDPIN PC13
#define SERIAL_CONFIG Serial3 //the serial port for remote config options
#define SERIAL_CONFIG_BAUD  115200 //only define if SERIAL_CONFIG is not sharing with a SERIALOUT* type port 

//SerialOut means data will be sent from that port. It needs to be set to a harware serial port.
//Best practice is only to send out to ports you will actually use.

#define SERIALOUT Serial1 //Serial1=kobo
#define SERIALOUT_BAUD 115200 //38400  //do not use with Serial (USB) it will hang

//#define SERIALOUTBT Serial3 //Bluetooth without AT commands Serial out
//#define SERIALOUTBT_BAUD 115200 //38400  //do not use with Serial (USB) it will hang

//#define SERIALOUTUSB  Serial //USB output

#define SERIALGPS Serial2
#define GPS
#define SERIALGPSBAUD 115200

#define VARIO
#define VARIO2 //if 2nd vario

#define BAROADDR 0x77
#define BAROADDR2 0x76


//#define ESPWIFI
//#define SERIALESP Serial3
//#define ESPAT  //use AT commands
//#define SERIALESPBAUD 115200 
#define WIFIEN_PIN 12 //wifi enable pin 

//#define WIFISSID "XCSensors01" // change this
//#define WIFIPASSWORD "thereisnospoon"

#define DHTH
#define DHT_PIN 17
#define DHTOFFSET 50 //calibrate sensor


#define ACCL

/* set HC-05 in sleep mode (via BTPINENABLE) after startup
  if the stop command is sent during startup, it will delay the sleep mode
  until start command.
*/
#define BTENPIN 2 //pin to enable HC-05
#define BTSLEEP  //enable sleep mode
/*Program the HC-05/06 via AT commands first (use the ftdi, press the use prog button, program baud is always 38400)
  AT+UART=38400,1,0 //115200 had problems
  AT+NAME=iXsensors0A
  AT+PSWD=1234
*/

#define BUZZER //let's go beep
#define BUZZPIN 16 //board pin 


#endif

/////////////////////////////////////////////////////////////////////
//STM32F Wifi Box
/////////////////////////////////////////////////////////////////////
#if defined(WIFIBOX_BT)


#define CONFIGOPT //enable configuration option (EEPROM required)
#define I2CEEPROM 0x50 ////External I2C EEPROM, required if using CONFIGOPT on STM32F 
#define I2CEEPROMPAGE 64 // page size of EEPROM 128 for 512

#define LEDPIN PB11 // PC13 - blue pill

#define SERIAL_CONFIG Serial //the serial port for remote config options
#define SERIAL_CONFIG_BAUD  115200 //only define if SERIAL_CONFIG uses it's own port


//SerialOut means data will be sent from that port. It needs to be set to a harware serial port.
//Best practice is only to send out to ports you will actually use.

//#define SERIALOUT Serial1
//#define SERI ALOUT_BAUD 115200  //do not use with Serial (USB) it will hang

#define SERIALOUTBT Serial1 //Bluetooth without AT commands Serial out
#define SERIALOUTBT_BAUD 115000 //38400  //do not use with Serial (USB) it will hang

#define SERIALOUTUSB  Serial //USB output. Usually no baud rate needed

#define SERIALGPS Serial2
#define GPS
#define SERIALGPSBAUD 9600 // serial1 speed

#define VARIO
//#define VARIO2 //if 2nd vario
#define BAROADDR 0x77
//#define BAROADDR2 0x76


//#define ESPWIFI 
//#define SERIALESP Serial3
//#define ESPAT  //use AT commands
//#define SERIALESPBAUD 115200 
//#define WIFIEN_PIN 12 //wifi enable pin 

#define WIFISSID "XCSensors" // change this 
#define WIFIPASSWORD "thereisnospoon"

#define DHTH //Curren lib autodetect sensor
#define DHT_PIN PB1
#define DHTOFFSET 0 //calibrate sensor


//#define ACCL


/* set HC-05 in sleep mode (via BTPINENABLE) after startup
  if the stop command is sent during startup, it will delay the sleep mode
  until start command.
*/
#define BTENPIN 2 //pin to enable HC-05
#define BTSLEEP  //enable sleep mode
/*Program the HC-05/06 via AT commands first (use the ftdi, press the use prog button, program baud is always 38400)
  AT+UART=38400,1,0 //115200 had problems
  AT+NAME=iXsensors0A
  AT+PSWD=1234
*/


#define BUZZER //let's go beep
#define BUZZPIN PB0 //board pin 



#endif //WIFIBOX



/////////////////////////////
// MCU Board TEENSY
////////////////////////////


#elif defined(TEENSY)

/////////////////////////////////////////////////////////////////////
//TEENSY Kobo Integrated
/////////////////////////////////////////////////////////////////////

#if defined(KOBO_BT)
#define CONFIGOPT //enable configuration option (EEPROM required)
//#define I2CEEPROM 0x50 //External I2C EEPROM
//#define I2CEEPROMPAGE 64 // page size of EEPROM 128 for 512

#define LEDPIN 13

#define SERIAL_CONFIG Serial3 //the serial port for remote config options
#define SERIAL_CONFIG_BAUD  115200 //only define if SERIAL_CONFIG is not sharing with a SERIALOUT* type port 

//SerialOut means data will be sent from that port. It needs to be set to a harware serial port.
//Best practice is only to send out to ports you will actually use.

#define SERIALOUT Serial1 //Serial1=kobo
#define SERIALOUT_BAUD 115200 //38400  //do not use with Serial (USB) it will hang

#define SERIALOUTBT Serial3 //Bluetooth without AT commands Serial out
#define SERIALOUTBT_BAUD 115200 //38400  //do not use with Serial (USB) it will hang

//#define SERIALUSB Serial //USB output

#define SERIALGPS Serial2
#define GPS
#define SERIALGPSBAUD 115200

#define VARIO
#define VARIO2 //if 2nd vario

#define BAROADDR 0x77
#define BAROADDR2 0x76


//#define ESPWIFI
//#define SERIALESP Serial3
//#define ESPAT  //use AT commands
//#define SERIALESPBAUD 115200 
///#define WIFIEN_PIN 12 //wifi enable pin 

//#define WIFISSID "XCSensors01" // change this
//#define WIFIPASSWORD "thereisnospoon"

#define DHTH;
#define DHT_PIN 17
#define DHTOFFSET 50 //calibrate sensor


#define ACCL



/* set HC-05 in sleep mode (via BTPINENABLE) after startup
  if the stop command is sent during startup, it will delay the sleep mode
  until start command.
*/
#define BTENPIN 2 //pin to enable HC-05
#define BTSLEEP  //enable sleep mode
/*Program the HC-05/06 via AT commands first (use the ftdi, press the use prog button, program baud is always 38400)
  AT+UART=38400,1,0 //115200 had problems
  AT+NAME=iXsensors0A
  AT+PSWD=1234
*/

#define BUZZER //let's go beep
#define BUZZPIN 16 //board pin 


#endif

/////////////////////////////////////////////////////////////////////
//TEENSY Wifi Box
/////////////////////////////////////////////////////////////////////
#if defined(WIFIBOX_BT)
#define CONFIGOPT //enable configuration option (EEPROM required)
//#define I2CEEPROM 0x50 //External I2C EEPROM
//#define I2CEEPROMPAGE 64 // page size of EEPROM 128 for 512

#define SERIAL_CONFIG Serial //the serial port for remote config options
//#define SERIAL_CONFIG_BAUD  115200 //only define if SERIAL_CONFIG uses it's own port
#define LEDPIN 13

//SerialOut means data will be sent from that port. It needs to be set to a harware serial port.
//Best practice is only to send out to ports you will actually use.

//#define SERIALOUT Serial1
//#define SERIALOUT_BAUD 115200  //do not use with Serial (USB) it will hang

#define SERIALOUTBT Serial1 //Bluetooth without AT commands Serial out
#define SERIALOUTBT_BAUD 38400 //38400  //do not use with Serial (USB) it will hang

#define SERIALOUTUSB Serial //USB output. Usually no baud rate needed

#define SERIALGPS Serial2
#define GPS
#define SERIALGPSBAUD 9600 // serial1 speed

#define VARIO
#define VARIO2 //if 2nd vario
#define BAROADDR 0x77
#define BAROADDR2 0x76


#define ESPWIFI 
#define SERIALESP Serial3
#define ESPAT  //use AT commands
#define SERIALESPBAUD 115200 
#define WIFIEN_PIN 12 //wifi enable pin 

#define WIFISSID "XCSensors" // change this 
#define WIFIPASSWORD "thereisnospoon"

#define DHTH 
#define DHT_PIN 17
#define DHTOFFSET 30 //calibrate sensor


#define ACCL


/* set HC-05 in sleep mode (via BTPINENABLE) after startup
  if the stop command is sent during startup, it will delay the sleep mode
  until start command.
*/
#define BTENPIN 2 //pin to enable HC-05
#define BTSLEEP  //enable sleep mode
/*Program the HC-05/06 via AT commands first (use the ftdi, press the use prog button, program baud is always 38400)
  AT+UART=38400,1,0 //115200 had problems
  AT+NAME=iXsensors0A
  AT+PSWD=1234
*/


#define BUZZER //let's go beep
#define BUZZPIN 16 //board pin 



#endif

#endif //TEENSY
////////////////////Set Configuration logic/////////////////////////


#if !defined(GPS)

#undef GPSSERIALEVENT
#endif

#if !defined(GPSSERIALEVENT)
#undef GPSTIMER
#endif

#if !defined(ESPWIFI)
#undef ESPAT
#endif

#if !defined(BTENPIN)
#undef BTSLEEP
#endif

#if defined(CONFIGOPT) && defined(STM32F) && !defined(I2CEEPROM)
#undef CONFIGOPT
#endif

/////////////////////////////////////////////////////////////////////




