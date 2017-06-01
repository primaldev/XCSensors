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

typedef struct {
  bool hasSavedtoEEPROM;
  int  GPSChannel;
  int  varioChannel;
  int humidChannel;
  int AcclChannel;
  int magChannel;
  //char ssid[32];
  //char password[32];
  double qnePressure;
  int varioDeadBand;
  int magOrientation;
  int magDeclination;
  bool wifiMultiPort;
  bool SerialMain;
  bool ptas1;

  bool lxnav;
  int accloffset;
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

