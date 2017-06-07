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

const int C_SerialMain=1;
const int C_qnePressure=5;
const int C_varioDeadBand=6;
const int C_ptas1=7;
const int C_lxnav=8;
const int C_variosmooth=9;
const int C_buzzer=10;
const int C_varioAudioDeadBand=11;
const int C_varioAudioSinkDeadBand=12;
const int C_advTriggerTime=13;
const int C_advRelaxTime=14;
const int C_advMinSmooth=15;
const int C_advMaxSmooth=16;


typedef struct {
  bool hasSavedtoEEPROM;
  double qnePressure;
  int varioDeadBand;
  int magOrientation;
  int magDeclination;
  bool SerialMain;
  bool ptas1;
  bool lxnav;
  byte variosmooth;
  bool buzzer;
  int varioAudioDeadBand;
  int varioAudioSinkDeadBand;
  int advTriggerTime;
  int advRelaxTime;
  byte advMinSmooth;
  byte advMaxSmooth;
   
} conf_t;

extern conf_t conf;

