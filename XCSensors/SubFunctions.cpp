/*
  XCSensors by Marco van Zoest

  www.primaldev.nl
  www.primalcode.nl

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/
#include <Arduino.h>
#include "SubFunctions.h"
#include "XCSensors.h"
#include "config.h" //All other files must use return values or classes
#include "Conf.h"
#include "SendData.h"

bool cmd = false;
#if defined(ADAPTIVEVARIO)
bool vTriggerd=false;
unsigned long vtime = 0;
#endif


#if defined(BUZZER)

unsigned long ttime = 0;
bool toneon = true;
bool playtone = false;
#endif
#if defined(TESTBUZZER)
unsigned long btime = 0;
float testvario = 0;
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
    sendData(GPSbuffer, conf.GPSChannel); //TODO need testing

    memset(GPSbuffer, 0, sizeof(GPSbuffer));
  }

}

#if defined(HUMANCONFIG)
void getConfVal(char c) {
  static char Confbuffer[5];
  static char Valbuffer[32];
  static int i, y;
  static char q;
  static bool flag = false;
  static bool split = false;
  q = c;

  if (!cmd)                                              // '<'
  {
    i = 0;
    y = 0;
    cmd = true;
  }

  if ( q == 0x3d)                                              // '='
  {
    split = true;
  }

  if (  q == 0x3d || q == 0x0d || q == 0x0a ) {

  } else {
    if (split) {

      if ( y < 32) Valbuffer[y++] = q;

    } else {

      if ( i < 5) Confbuffer[i++] = q;

    }
  }

  if (q == 0x0d) { // enter
    flag = true;
    i = 0;
    y = 0;
    cmd = false;
  }
  //Serial.print(c);
  if (flag) {
    flag = false;
    split = false;
    setConf(atoi(Confbuffer), Valbuffer);
    memset(Confbuffer, 0, sizeof(Confbuffer));
    memset(Valbuffer, 0, sizeof(Valbuffer));

  }


}
#else

void getConfVal(char c) {
  static char Confbuffer[5];
  static char Valbuffer[32];
  static int i, y;
  static char q;
  static bool flag = false;
  static bool split = false;
  q = c;

  if ( q == 0x3c)                                              // '<'
  {
    i = 0;
    y = 0;
  }

  if ( q == 0x3d)                                              // '='
  {
    split = true;
  }

  if ( q == 0x3c || q == 0x3d || q == 0x3e) {

  } else {
    if (split) {

      if ( y < 32) Valbuffer[y++] = q;

    } else {

      if ( i < 5) Confbuffer[i++] = q;

    }
  }

  if (q == 0x3e) { // '>'
    flag = true;
    i = 0;
    y = 0;
  }

  if (flag) {
    flag = false;
    split = false;

    setConf(atoi(Confbuffer), Valbuffer);
    memset(Confbuffer, 0, sizeof(Confbuffer));
    memset(Valbuffer, 0, sizeof(Valbuffer));

  }

}
#endif


void sendNmeaDHT() {
#if defined(DHT)
  int temp = (dhts.temperature + 273.15) * 10;
  nmea.setNmeaHumidSentence(temp, dhts.humidity);
  sendData(nmea.nmeaHumid, conf.humidChannel);
#endif

}

void sendNmeaHEading() {
#if defined(MAG)
  nmea.setMagneticHeading(getCalcHeading());
  sendData(nmea.nmeaMag, conf.magChannel);
#endif

}

float getCalcHeading() {
#if defined(MAG)
  float heading = atan2(my, mx);
  if (heading < 0)
    heading += 2 * M_PI;
  return heading * 180 / M_PI;

#else
  return 999;
#endif
}

void sendAccelerometor() {
#if defined(ACCL)
  sendData(nmea.nmeaGforce, conf.AcclChannel);
#endif
}

void sendPTAS1() { //sends vario data every 155ms on generic GPS channel
#if defined(VARIO)
  sendData(nmea.nmeaPTAS1, conf.GPSChannel);
#endif
}

void sendNmeaAll() {

#if defined(VARIO)
  if (conf.lxnav) {
    sendData(nmea.nmeaVarioLXWP0, conf.varioChannel);
    sendData(nmea.nmeaVario, conf.varioChannel);

  }

#endif
#if defined(MAG)
  sendNmeaHEading();
#endif

#if defined(DHT)
  sendNmeaDHT();
#endif

#if defined(ACCL)
  sendAccelerometor();
#endif


}

#if defined(BUZZER)

void makeSound(float vario) {
  int pulse;

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
  
      pulse = 2000 / (vario * 10) + 100;
    
  } else {
    pulse = 400;
  }

  if (playtone) {
    if (vario > double(conf.varioAudioDeadBand) / 1000 ) {
      
      tone(BUZZPIN, variof);
      playtone = false;
    } else if (vario < -double(conf.varioAudioSinkDeadBand) / 1000 ) {
      pulse = 400;
      tone(BUZZPIN, 200);
      playtone = false;
    } else {
      noTone(BUZZPIN);
      pinMode(BUZZPIN, INPUT);
    }
  }

  int diff = millis() - ttime;

  if (diff > pulse) {
    noTone(BUZZPIN);
    pinMode(BUZZPIN, INPUT);

    if (toneon) {
      toneon = false;
      playtone = false;
      pinMode(BUZZPIN, INPUT);
      ttime = millis() - 80;
    } else  {
      toneon = true;
      playtone = true;
      ttime = millis() + 50;

    }
  }

}

#endif



void checkAdaptiveVario(double vario){
#if defined(ADAPTIVEVARIO)
  if (fabs(vario) > ADVLOWTRIGGER && !vTriggerd) { //fabs abs but can handle floats
     vtime=millis();
     vTriggerd=true;
  }

  int diff = millis() - vtime;
  
  if (vTriggerd && fabs(vario) < ADVLOWTRIGGER  && diff < conf.advTriggerTime)  { //zero   
     if(conf.variosmooth <= conf.advMaxSmooth ) {
       conf.variosmooth++;
       vTriggerd=false;
       vtime=millis();
     }
  }

  if (diff > conf.advTriggerTime && vTriggerd) { //overflow reset
    vTriggerd=false;
  }

  if (fabs(vario) < float(ADVLOWTRIGGER ) && !vTriggerd && diff >  conf.advRelaxTime) {
      if(conf.variosmooth > conf.advMinSmooth ) {
        conf.variosmooth--;
        vtime=millis();       
      }
  }
  

#endif  
}


