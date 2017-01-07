/*
XCsensors by Marco van Zoest

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


typedef struct {
bool hasSavedtoEEPROM;
int  GPSChannel;
int  varioChannel;
int humidChannel;
int AcclChannel;
int magChannel;
char ssid[32];
char password[32];
double qnePressure;
int varioDeadBand;
long realPressureAv;
int magDeclination;
int vario2CalcMethod;
bool wifiMultiPort;
bool bluetoothOnly;
bool ptas1;
bool ptasav;
bool lxnav;
int accloffset;
byte variosmooth;
bool variopaced;
bool buzzer;
int varioAudioDeadBand;
int varioAudioSinkDeadBand;
} conf_t;

extern conf_t conf;
