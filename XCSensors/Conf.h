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
#include <SoftwareSerial.h>
void getConfig();
void setConf(int varname, char *value);
void getConfigFromEEPROM();
void saveConfigToEEPROM();
void setDefaultConfig();
String getStringFromBool(bool bval);
void getConfigVars() ;
void getDefaultConfig();

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

