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
unsigned long stime = 0;
unsigned long ttime = 0;
unsigned long sinktime = 0;
bool toneon = true;
bool playtone = false;
bool climbing = false;
#endif

#if defined(TESTBUZZER)
unsigned long btime = 0;
float testvario = 0;
#endif


#if defined(BUZZER)

#define BASEPULSE 200
#define TOPPULSE  1000

void makeSound(float vario) {
  int pulse;
  float varioorg = vario;
  if (takeoff) {
    vario += BUZZERVARIOGRPAD;
  }
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

  int variof = (fabs(vario) * 200 ) + 800;

  if (vario >= double(conf.varioAudioDeadBand) / 1000) {

    pulse = TOPPULSE / (vario * 10) + 100;

  } else {
    pulse = BASEPULSE;
  }

#if defined(SOARDETECTION) && !defined(TESTBUZZER)

  if (varioorg > -0.2 && varioorg < 0.2) {
    int diffe = millis() - stime;
    if (diffe >  SOARDETECTION) {
      playtone = false;
    }
  } else {
    stime = millis();
  }

#endif


  if (playtone) {

    if (vario > double(conf.varioAudioDeadBand) / 1000 ) {
      tone(BUZZPIN, variof);
      climbing = true;
      playtone = false;
    } else if (vario < -double(conf.varioAudioSinkDeadBand) / 1000 ) {
      pulse = BASEPULSE;
      tone(BUZZPIN, 200);
      playtone = false;
    } else {

      if (climbing ) { //dropped out of the thermal
        pulse = 2000;
        tone(BUZZPIN, 100);
        int diffsk = millis() - stime;
        if (diffsk > OUTOFTHERMALBUZZT) {
          climbing = false;
        }
      } else {
        sinktime = millis();
      }

    }
  }

  int diff = millis() - ttime;

  if (diff > pulse) {
    noTone(BUZZPIN);
    //pinMode(BUZZPIN, INPUT);

    if (toneon) {
      toneon = false;
      playtone = false;
      // pinMode(BUZZPIN, INPUT);
      ttime = millis() - 40;
    } else  {
      toneon = true;
      playtone = true;
      ttime = millis() + 20;

    }
  }

}

#endif


