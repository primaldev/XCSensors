/*
  XCSensors http://XCSensors.org

  Copyright (c), PrimalCode (http://www.primalcode.org)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

//Audio related stuff

#include <Arduino.h>
#include "Audio.h"
#include "config.h"
#include "Conf.h"
#include "XCSensors.h"


#if defined(BUZZER)
bool climbing = false;
int tm;
int rm;
int stime;
byte toneOn = false;
byte muted = false;
float variof;
#endif

#if defined(TESTBUZZER)
unsigned long btime = 0;
float testvario = 0;
#endif


#if defined(BUZZER)

#define BASEPULSE 200
#define TOPPULSE  1000



// Non-Blocking beep blob beep
void playTwoToneInterval(int freq,int freq2, int period, int intervald) {
 
  
  if (toneOn) {
    int wait = period + tm;
    
    
    if ( wait < millis()) {
      toneOn = false;
      noTone(BUZZPIN);
      tone(BUZZPIN, freq2, intervald);
      rm = millis();
    }

  } else {
    int ndwait = intervald + rm;
     
    if(ndwait < millis()) {
    
    tone(BUZZPIN, freq, period);
    toneOn = true;
    tm = millis();
    }
  }

}


// Non-Blocking beep beep beep
void playToneInterval(int freq, int period, int interval) {

  if (toneOn) {
    int wait = period + interval + tm;

    if ( wait < millis()) {
      toneOn = false;
      noTone(BUZZPIN);
    }

  } else {
    tone(BUZZPIN, freq, period);
    toneOn = true;
    tm = millis();
  }

}


void makeVarioAudio(float vario) {
  int pulse;
  float varioorg = vario;

#if defined(TESTBUZZER)
  vario = testvario;
  int tpassed = millis() - btime;
  if (tpassed > 2000) {
    testvario = testvario + 0.1;
    btime = millis();
  }

  if (testvario > 9 ) {
    testvario = 0;
  }

  Serial.println(vario);
#endif

  if (vario > 9) {
    vario = 9;

  }

#if defined(SOARDETECTION) && !defined(TESTBUZZER)

  if (varioorg > -0.2 && varioorg < 0.2) {
    int diffe = millis() - stime;
    if (diffe >  int(conf.SoarDeadBandTime)) {
      muted = true;
    }
  } else {
    stime = millis();
    muted = false;
  }

#endif

    if (vario <= 0 && vario >= BUZZERZEROCLIMB) {
      if (!muted) {
         playToneInterval(variof, 50, 400);
      }

    }
    
   if (vario <= double(conf.sinkAlarmLevel)/1000 ) { //sink alarm
      if (!muted) {
         playTwoToneInterval(1400, 1800, 100, 100);
      }

   }

 // float variofa = (float(fabs(vario)) * 200 ) + 800;
 float variofa = (float(fabs(vario)) * 200 ) + 900;
  variof = (10 * variof + variofa) / 11 ;

  if (vario > 0) {
    pulse = TOPPULSE / (vario * 10) + 100;
    if (!muted) {
      playToneInterval(variof, pulse, pulse / 2);
    }
    climbing = true;
  } else {
    if (climbing ) { //dropped out of the thermal
      tone(BUZZPIN, 100, OUTOFTHERMALBUZZT);
      climbing = false;
    }
  }

}


#endif


