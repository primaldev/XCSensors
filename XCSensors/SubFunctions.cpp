/*
  XCSensors http://XCSensors.org

  Copyright (c), PrimalCode (http://www.primalcode.org)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/
#include <Arduino.h>
#include "SubFunctions.h"
#include "XCSensors.h"
#include "config.h"
#include "Conf.h"
#include "SendData.h"

#if defined(ADAPTIVEVARIO)
bool vTriggerd = false;
unsigned long vtime = 0;
#endif

//

#define GPSBUFFER 120

void GPSstuff(char c) {                                         // GPSbuffer[] is global
  static int i;                                              //   persistent within function scope
  static char q;
  static bool flag = false;
  static char GPSbuffer[GPSBUFFER];                                   // GPS serial line input buffer
  String nmeaCode;
  q = c;

  if ( q == 0x24 )                                              // '$'
  {
    i = 0;                                                      // brute force sync on '$' to GPSbuffer[0]
    // Serial << "Found $" << endl;
  }

  if ( i < GPSBUFFER) GPSbuffer[i++] = q;

  if (q == 0x0d) {
    flag = true;                                                // is the character a CR for eol
    GPSbuffer[i++] = '\0';
    i = 0;
  }

  if (flag) {                                                   // test for end of line and if the right GPSbuffer
    flag = false;                                               // reset for next time
    sendData(GPSbuffer);

    memset(GPSbuffer, 0, sizeof(GPSbuffer));
  }
}


void checkAdaptiveVario(double vario) {
#if defined(ADAPTIVEVARIO)
  
  double triggerLevel = double(conf.advTriggerLevel)/1000;
  
  if(takeoff) { //compensate for glider sink
    vario += -double(conf.gliderSinkRate)/1000;
  }


  if (fabs(vario) > triggerLevel && !vTriggerd) { //fabs abs but can handle floats
    vtime = millis();
    vTriggerd = true;    
  }

  int diff = millis() - vtime; 

  if (vTriggerd && fabs(vario) < triggerLevel  && diff < int(conf.advTriggerTime))  { //zero
    if (conf.variosmooth <= conf.advMaxSmooth ) {
      conf.variosmooth++;
      vTriggerd = false;
      vtime = millis();
      
    }
  }

  if (diff > conf.advTriggerTime && vTriggerd) { //overflow reset
    vTriggerd = false;
  }

  if (fabs(vario) < float(triggerLevel) && !vTriggerd && diff >  conf.advRelaxTime) {
    if (conf.variosmooth > conf.advMinSmooth ) {
      conf.variosmooth--;
      vtime = millis();      
    }
  }


#endif
}


