/*
  XCSensors http://XCSensors.org
  
  Copyright (c), PrimalCode (http://www.primalcode.org)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/
#include <Arduino.h>
#include "NMEA.h"


void resetACCLcompVal();
void collectNmea10();
void readVarioPressure();
void readACCLSensor();
void getSlowSensorData();
extern int16_t mx, my, mz;
extern bool runloop;
extern NMEA nmea;
extern bool takeoff;

extern byte dhttemperature;
extern byte dhthumidity;


