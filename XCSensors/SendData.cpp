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
#include "XCSensors.h"
#include "SendData.h"
#include "config.h" //All other files must use return values or classes
#include "Conf.h"

#if defined(EPSWIFI)

void setEPSPort(int id, int port) {
   serialEPS.print(F("AT+CIPSTART="));
   serialEPS.print(id);
   serialEPS.print(F(",\"UDP\",\"255.255.255.255\","));
   serialEPS.println(port);
   delay(500); 
}

void startESPSoftAP(){
  serialEPS.println(F("+++AT+RST"));
  delay(1000);
  serialEPS.println(F("AT+RFPOWER=0"));
  delay(200);
  serialEPS.println(F("AT+CWMODE=3"));
  delay(500);
  serialEPS.print(F("AT+CWSAP=\""));
  serialEPS.print(conf.ssid);
  serialEPS.print(F("\",\""));
  serialEPS.print(conf.password);
  serialEPS.println(F("\",5,3"));  
  delay(500);
}

void setEPSMulti(){ 

  startESPSoftAP();
  serialEPS.println(F("AT+CIPMUX=1"));
  delay(500);
  setEPSPort(1, CHANNEL1PORT);
  setEPSPort(2, CHANNEL2PORT);
  setEPSPort(3, CHANNEL3PORT);
  setEPSPort(4, CHANNEL4PORT); 
}

void setESPBroadCast(){  
  startESPSoftAP();
  serialEPS.print(F("AT+CIPSTART=\"UDP\",\"255.255.255.255\","));
  serialEPS.println(CHANNEL1PORT);  
  delay(500); 
  serialEPS.println(F("AT+CIPMODE=1"));
  delay(500); 
  serialEPS.println(F("AT+CIPSEND"));  
}

#endif

void setSendData(){ 
  if (conf.wifiMultiPort) {
    setEPSMulti();
  }else {
    setESPBroadCast();
  }
}

int getSize(char* ch){
      int tmp=0;
      while (*ch) {
        *ch++;
        tmp++;
      }
      return tmp;
}


void sendData(char *message, int id) {
  #if defined(SERIAL1OUTONLY)
    Serial.println(message);
  #else
  if(conf.bluetoothOnly) {
    serialBT.println(message); 
  }else{
    #if defined(EPSAT)
    #if defined(EPSAT)
    if (conf.wifiMultiPort) {      
      int len = getSize(message) + 1; //sizeof might be wrong
       serialEPS.print(F("AT+CIPSEND=")); //shaving memory
       serialEPS.print( id );
       serialEPS.print(F(","));
       serialEPS.println(len); 
       delay(10);
       serialEPS.print(message);  
       serialEPS.print(F("\n\r"));
       delay(10);
    }else{
      serialEPS.println(message); 
      serialEPS.print(F("\n\r")); 
    }
    #else
      serialEPS.print(message);  
      serialEPS.print(F("\n\r"));
    #endif
    #endif
  }
  
  #endif
}

