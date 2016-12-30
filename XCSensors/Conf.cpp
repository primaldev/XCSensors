/*
XCsensors by Marco van Zoest

www.primaldev.nl
www.primalcode.nl

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

  float f = 0.00f;   
  int eeAddress = 0; 

void getConfig(){ //load default config values from EEPROM
  
   getConfigFromEEPROM();
   if (!conf.hasSavedtoEEPROM) {
      conf.hasSavedtoEEPROM=true;
      setDefaultConfig();
      getConfigFromEEPROM();
     Serial.println("No config from EE");
   }

   getConfigVars(serialBT);
}

void setDefaultConfig(){
  
    for (int i = 0 ; i < EEPROM.length() ; i++) { //clear the EEPROM
      EEPROM.write(i, 0);
    }
     conf.GPSChannel = 1;
     conf.varioChannel = 2;
     conf.humidChannel = 3;
     conf.magChannel = 1; 
     conf.AcclChannel = 4;
     strcpy(conf.ssid,"XcSensors");
     strcpy(conf.password,"thereisnospoon");
     conf.magDeclination = 0; //in Radians. Find your declination here: http://www.magnetic-declination.com/   
     conf.qnePressure = 101325; //QNH value to calculate vario Altitude
     conf.varioDeadBand= 100; // X 1000 levels lower than this = 0. (ignored by ptas1)
     conf.vario2CalcMethod=1; //0 = average, 1=least deviation
     conf.wifiMultiPort=true; //use different ports for NMEA sentences, or only the gps port
     conf.bluetoothOnly=true; //only send data to BT. reset needed to start WIFI
     conf.ptas1=true;// send ptas1 nmea, uses the gps channel (once every 155ms)  
     conf.ptasav=true; //inlclude vario avarage in ptas sentence. ( value is ignored by XCsoar)
     conf.lxnav=true; //send vario lxnav sentence
     conf.accloffset=0; //manual offset for accl x 1000
     conf.variosmooth=20; //low pass filter, the higher the number the slower the raw vario reading changes.
     conf.variopaced=false; //take a reading every 155ms instead of every n'th cycle of the loop(). Much slower reaction but stable vario smooth
     conf.buzzer=true; // turn vario audio on or off
     conf.varioAudioDeadBand = 200; //X 1000
     conf.varioAudioSinkDeadBand = 300;  //X 1000 and absolute value
     saveConfigToEEPROM();
     
}


void getConfigVars(SoftwareSerial outSerial) { // order is not that important
    outSerial.print(F("<1="));
    outSerial.print( getStringFromBool(conf.bluetoothOnly)); 
    outSerial.println(F(">")); 
    outSerial.print(F("<2="));
    outSerial.print(getStringFromBool(conf.wifiMultiPort));
    outSerial.println(F(">"));
    outSerial.print(F("<3="));
    outSerial.print(conf.GPSChannel);
    outSerial.println(F(">"));
    outSerial.print(F("<4="));
    outSerial.print(conf.varioChannel);
    outSerial.println(F(">"));
    outSerial.print(F("<5="));
    outSerial.print(conf.humidChannel);
    outSerial.println(F(">"));
    outSerial.print(F("<6="));
    outSerial.print(conf.magChannel);
    outSerial.println(F(">"));
    outSerial.print(F("<7="));
    outSerial.print(conf.AcclChannel);
    outSerial.println(F(">"));
    outSerial.print(F("<8="));
    outSerial.print(conf.ssid);
    outSerial.println(F(">"));
    outSerial.print(F("<9="));
    outSerial.print(conf.password);
    outSerial.println(F(">"));
    outSerial.print(F("<10="));
    outSerial.print(conf.magDeclination);
    outSerial.println(F(">"));
    outSerial.print(F("<11="));
    outSerial.print(String(conf.qnePressure));
    outSerial.println(F(">"));
    outSerial.print(F("<12="));
    outSerial.print(conf.varioDeadBand);
    outSerial.println(F(">"));
    outSerial.print(F("<13="));
    outSerial.print(conf.vario2CalcMethod);
    outSerial.println(F(">"));
    outSerial.print(F("<14="));
    outSerial.print(conf.ptas1);
    outSerial.println(F(">"));
    outSerial.print(F("<15="));
    outSerial.print(conf.ptasav); 
    outSerial.println(F(">"));
    outSerial.print(F("<16="));
    outSerial.print(conf.lxnav); 
    outSerial.println(F(">"));
    outSerial.print(F("<17="));
    outSerial.print(conf.accloffset); 
    outSerial.println(F(">"));
    outSerial.print(F("<18="));
    outSerial.print(conf.variosmooth); 
    outSerial.println(F(">"));
    outSerial.print(F("<19="));
    outSerial.print(conf.variopaced); 
    outSerial.println(F(">"));
    outSerial.print(F("<20="));
    outSerial.print(conf.buzzer); 
    outSerial.println(F(">"));
    outSerial.print(F("<21="));
    outSerial.print(conf.varioAudioDeadBand); 
    outSerial.println(F(">"));
    outSerial.print(F("<22="));
    outSerial.print(conf.varioAudioSinkDeadBand); 
    outSerial.println(F(">"));
    
}

String getStringFromBool(bool bval){
   if (bval) {
    return "On";
   }else{
     return "Off";
   }
}

void getConfigFromEEPROM() {
   EEPROM.get(eeAddress, conf);
}

void saveConfigToEEPROM(){
  EEPROM.put(eeAddress, conf);
  
}

bool getBoolFromVal(char *sval) {
  if (sval[0]=="1") {
    return true;
  } else {
    return false;
  }
}


void setConf(int varname, char *value) {

  switch (varname) {
    case 0: setDefaultConfig(); break;//load defaults
    case 1: conf.bluetoothOnly=getBoolFromVal(value); break;
    case 2: conf.wifiMultiPort= getBoolFromVal(value); break;
    case 3: conf.GPSChannel= atoi(value); break;
    case 4: conf.varioChannel= atoi(value); break;
    case 5: conf.humidChannel= atoi(value); break;
    case 6: conf.magChannel= atoi(value); break;
    case 7: conf.AcclChannel= atoi(value); break;
    case 8: strcpy(conf.ssid,value); break;
    case 9: strcpy(conf.password,value); break;
    case 10: conf.magDeclination= atoi(value); break;
    case 11: conf.qnePressure= atoi(value); break;
    case 12: conf.varioDeadBand= atoi(value); break;
    case 13: conf.vario2CalcMethod= atoi(value); break;
    case 14: conf.ptas1= getBoolFromVal(value); break;
    case 15: conf.ptasav= getBoolFromVal(value); break;
    case 16: conf.lxnav= getBoolFromVal(value); break;
    case 17: conf.accloffset=atoi(value); break;
    case 18: conf.variosmooth=atoi(value); break;
    case 19: conf.variopaced=getBoolFromVal(value); break;
    case 20: conf.buzzer=getBoolFromVal(value); break;
    case 21: conf.varioAudioDeadBand=atoi(value); break;
    case 22: conf.varioAudioSinkDeadBand=atoi(value); break;
    case 100: saveConfigToEEPROM(); break; //save to eeprom
    case 106: resetACCLcompVal(); // quick set the ACCL to 0
    case 101: getConfigVars(serialBT); break; // get config for app
    case 200: runloop=false; break; //stop 
    case 201: runloop=true; break; //start
    default:
     
     getConfigVars(serialBT);
    break;
  }
  
}



