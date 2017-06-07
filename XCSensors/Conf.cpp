/*
  XCSensors http://XCSensors.org
  
  Copyright (c), PrimalCode (http://www.primalcode.org)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/


#include "config.h"
#include "Conf.h"
#include "XCSensors.h"
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "SubFunctions.h"


bool cmd = false;
float f = 0.00f;
int eeAddress = 0;

void getConfig() { //load default config values from EEPROM



  getConfigFromEEPROM();
  if (!conf.hasSavedtoEEPROM) {
    conf.hasSavedtoEEPROM = true;
    setDefaultConfig();
    getConfigFromEEPROM();

  }

  getConfigVars();

#if defined(EEPROMDEVMODE)
  setDefaultConfig();
#endif

}

void setDefaultConfig() {

  for (int i = 0 ; i < EEPROM.length() ; i++) { //clear the EEPROM
    EEPROM.write(i, 0);
  }
  getDefaultConfig();
  saveConfigToEEPROM();


}

void getDefaultConfig() {
  conf.qnePressure = 101325; //QNH value to calculate vario Altitude
  conf.varioDeadBand = 100; // X 1000 levels lower than this = 0. (ignored by ptas1)
  conf.SerialMain = false; //only send data to the one serial. reset needed to start WIFI
  conf.ptas1 = true; // send ptas1 nmea, uses the gps channel (once every 100ms)
  conf.lxnav = false; //send vario lxnav sentence
  conf.variosmooth = 15; //low pass filter, the higher the number the slower the raw vario reading changes.
  conf.buzzer = true; // turn vario audio on or off
  conf.varioAudioDeadBand = 100; //X 1000
  conf.varioAudioSinkDeadBand = 3000;  //X 1000 and absolute value
  conf.advTriggerTime = 1000; // if vario level goes lower than advLowTrigger in this time, it will cause a trigger and increase conf.variosmooth.
  conf.advRelaxTime = 20000; // if no trigger occurs in this time frame, conf.variosmooth is reduced by 1,
  conf.advMinSmooth = 6; // lowest level for conf.variosmooth
  conf.advMaxSmooth = 28; // highest level for conf.variosmooth
}

#if defined (SERIAL_CONFIG)
#if defined(HUMANCONFIG)


void printaf(int n) {
  switch (n) {

    case C_SerialMain: SERIAL_CONFIG.print(F("1) Serial Only, Wifi or BT Disable: ")); break;
    case C_qnePressure: SERIAL_CONFIG.print(F("11) QNE Pressure: ")); break;
    case C_varioDeadBand: SERIAL_CONFIG.print(F("12) Vario deadband (x1000): ")); break;
    case C_ptas1: SERIAL_CONFIG.print(F("14) Send PTAS: ")); break;
    case C_lxnav: SERIAL_CONFIG.print(F("16) Send LXNAV: ")); break;
    case C_variosmooth: SERIAL_CONFIG.print(F("18) Vario smoothness level: ")); break;
    case C_buzzer: SERIAL_CONFIG.print(F("20) Enable Vario Audio: ")); break;
    case C_varioAudioDeadBand: SERIAL_CONFIG.print(F("21) Vario Audio asc deadband x1000: ")); break;
    case C_varioAudioSinkDeadBand: SERIAL_CONFIG.print(F("22) Vario audio sink deadband x1000: ")); break;
    case C_advTriggerTime: SERIAL_CONFIG.print(F("24) Adaptive vario trigger time: ")); break;
    case C_advRelaxTime: SERIAL_CONFIG.print(F("25) Adaptive vario relax time: ")); break;
    case C_advMinSmooth: SERIAL_CONFIG.print(F("26) Adaptive vario minimum smooth level: ")); break;
    case C_advMaxSmooth: SERIAL_CONFIG.print(F("27) Adaptive vario maximum smooth level: ")); break;

  }

}

void printtf() {
  SERIAL_CONFIG.println();

}

void printhd() {
  SERIAL_CONFIG.println();
  SERIAL_CONFIG.println(F("XCSensors Config options:"));
  SERIAL_CONFIG.println(F("200 - Stop Feed"));
  SERIAL_CONFIG.println(F("201 - Start Feed"));
  SERIAL_CONFIG.println(F("0 - Default values"));
  SERIAL_CONFIG.println(F("101 - This menu"));
  SERIAL_CONFIG.println(F("100 - Save to EEPROM"));
  SERIAL_CONFIG.println(F("106 - Calibrate Accelerometer"));
  SERIAL_CONFIG.println(F("Use 1 for on and 0 for off"));
  SERIAL_CONFIG.println();
  SERIAL_CONFIG.println(F("XCSensors Configuration:"));
  SERIAL_CONFIG.println();
}

#else
void printaf(int n) {

  SERIAL_CONFIG.print(F("<"));
  SERIAL_CONFIG.print(n);
  SERIAL_CONFIG.print(F("="));
}
void printtf() {
  SERIAL_CONFIG.println(F(">"));
}

void printhd() {

}

#endif
#endif


void getConfigVars() { // order is not that important

#if defined (SERIAL_CONFIG)
  printhd();
  printaf(1);
  SERIAL_CONFIG.print( getStringFromBool(conf.SerialMain)); 
  printtf();
  printaf(11);
  SERIAL_CONFIG.print(String(conf.qnePressure));
  printtf();
  printaf(12);
  SERIAL_CONFIG.print(conf.varioDeadBand);
  printtf();
  printaf(14);
  SERIAL_CONFIG.print(conf.ptas1);
  printtf();
  printaf(16);
  SERIAL_CONFIG.print(conf.lxnav);
  printtf();
  printaf(17);
  SERIAL_CONFIG.print(conf.variosmooth);
  printtf();
  printaf(20);
  SERIAL_CONFIG.print(conf.buzzer);
  printtf();
  printaf(21);
  SERIAL_CONFIG.print(conf.varioAudioDeadBand);
  printtf();
  printaf(22);
  SERIAL_CONFIG.print(conf.varioAudioSinkDeadBand);
  printtf();
  printaf(24);
  SERIAL_CONFIG.print(conf.advTriggerTime);
  printtf();
  printaf(25);
  SERIAL_CONFIG.print( conf.advRelaxTime);
  printtf();
  printaf(26);
  SERIAL_CONFIG.print( conf.advMinSmooth);
  printtf();
  printaf(27);
  SERIAL_CONFIG.print(conf.advMaxSmooth);
  printtf();
#if defined(WIFIEN_PIN)
  if (conf.SerialMain) {
    digitalWrite(WIFIEN_PIN, LOW);
  } else {
    digitalWrite(WIFIEN_PIN, HIGH);
  }
#endif
#endif
  //#if defined(LCDMENU) //here be LCD
  //
}


String getStringFromBool(bool bval) { //TODO: process Boolean values
  if (bval) {
    return "On";
  } else {
    return "Off";
  }
}

void getConfigFromEEPROM() {
  EEPROM.get(eeAddress, conf);

}

void saveConfigToEEPROM() {
  EEPROM.put(eeAddress, conf);

}

bool getBoolFromVal(char *sval) { //TODO: process Boolean values
  if (atoi(sval) == 1) {
    return true;
  } else {
    return false;
  }
}

void setConf(int varname, char *value) {
    
  switch (varname) {
    case 0: setDefaultConfig(); break;//load defaults
    case C_SerialMain: conf.SerialMain = getBoolFromVal(value); break;
    case C_qnePressure: conf.qnePressure = atoi(value); break; 
    case C_varioDeadBand: conf.varioDeadBand = atoi(value); break;
    case C_ptas1: conf.ptas1 = getBoolFromVal(value); break;
    case C_lxnav: conf.lxnav = getBoolFromVal(value); break;
    case C_variosmooth: conf.variosmooth = atoi(value); break;
    case C_buzzer: conf.buzzer = getBoolFromVal(value); break;
    case C_varioAudioDeadBand: conf.varioAudioDeadBand = atoi(value); break;
    case C_varioAudioSinkDeadBand: conf.varioAudioSinkDeadBand = atoi(value); break;
    case C_advTriggerTime: conf.advTriggerTime = atoi(value); break;
    case C_advRelaxTime: conf.advRelaxTime = atoi(value); break;
    case C_advMinSmooth: conf.advMinSmooth = atoi(value); break;
    case C_advMaxSmooth: conf.advMaxSmooth = atoi(value); break;
    case 100: saveConfigToEEPROM(); break; //save to eeprom
    case 106: resetACCLcompVal(); // quick set the ACCL to 0
    case 101: getConfigVars(); break; // get config for app
    case 200: runloop = false; break; //stop
    case 201:
      runloop = true;
#if defined(BTSLEEP)
      if (!conf.SerialMain) {
        digitalWrite(BTENPIN, LOW); //Make BT go ZZ
      }
#endif
      break; //start
    default:

      getConfigVars();
      break;
  }
  getConfigVars();
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

