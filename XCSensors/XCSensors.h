/*
XCsensors by Marco van Zoest

www.primaldev.nl
www.primalcode.nl

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version. see <http://www.gnu.org/licenses/>
*/
#include <Arduino.h>
#include "MS5611.h"
#include "NMEA.h"
#include "Average.h"
#include <dht.h>
#include <MPU6050.h>
#include <HMC5883L.h> 
#include <SoftwareSerial.h>


void resetACCLcompVal();
void collectNmea6();
void readVarioPressure();
void readACCLSensor();

extern int16_t mx, my, mz;
extern bool runloop;
extern NMEA nmea;
//#if defined(DHT)
extern dht dhts;
//#endif
extern SoftwareSerial serialEPS;


#if defined(SERIAL1OUTONLY)     
extern HardwareSerial &serialBT;
#else
extern SoftwareSerial serialBT;
#endif 




