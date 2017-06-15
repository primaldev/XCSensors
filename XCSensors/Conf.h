/*
  XCSensors http://XCSensors.org
  
  Copyright (c), PrimalCode (http://www.primalcode.org)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#include <Arduino.h>
#include <SoftwareSerial.h>
void getConfig();
void setConf(int varname, char *value);
void getConfigFromEEPROM();
void saveConfigToEEPROM();
void setDefaultConfig();
String getStringFromBool(bool bval);
void getConfigVars() ;
void getDefaultConfig();
void getConfVal(char c);

const int C_SerialOut=1;
const int C_SerialOutBT=2;
const int C_SerialOutESP=3;
const int C_SerialOutUSB=4;
const int C_qnePressure=5;
const int C_varioDeadBand=6;
const int C_ptas1=7;
const int C_lxnav=8;
const int C_pcprobe=9;
const int C_xcs=10;
const int C_variosmooth=11;
const int C_buzzer=12;
const int C_varioAudioDeadBand=13;
const int C_varioAudioSinkDeadBand=14;
const int C_advTriggerTime=15;
const int C_advRelaxTime=16;
const int C_advMinSmooth=17;
const int C_advMaxSmooth=18;


typedef struct {
  byte hasSavedtoEEPROM;
  double qnePressure;
  int varioDeadBand;
  byte SerialOut; 
  byte SerialOutBT; 
  byte SerialOutESP; 
  byte SerialOutUSB;
  byte ptas1;
  byte lxnav;
  byte pcprobe; //
  byte xcs; //
  byte variosmooth;
  byte buzzer;
  int varioAudioDeadBand;
  int varioAudioSinkDeadBand;
  int advTriggerTime;
  int advRelaxTime;
  byte advMinSmooth;
  byte advMaxSmooth;
 
} conf_t;



extern conf_t conf;

