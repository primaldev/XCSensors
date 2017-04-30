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
#include "XCSensors.h"
#include "SendData.h"
#include "config.h" //All other files must use return values or classes
#include "Conf.h"

#if defined(ESPWIFI)

void setESPPort(int id, int port) {
  SERIALESP.print(F("AT+CIPSTART="));
  SERIALESP.print(id);
  SERIALESP.print(F(",\"UDP\",\"255.255.255.255\","));
  SERIALESP.println(port);
  delay(500);
}

void startESPSoftAP() {
  SERIALESP.println(F("+++AT+RST"));
  delay(1000);
  SERIALESP.println(F("AT+RFPOWER=0"));
  delay(200);
  SERIALESP.println(F("AT+CWMODE=3"));
  delay(500);
  SERIALESP.print(F("AT+CWSAP=\""));
  SERIALESP.print(WIFISSID);
  SERIALESP.print(F("\",\""));
  SERIALESP.print(WIFIPASSWORD);
  SERIALESP.println(F("\",5,3"));
  delay(1000);
}

void setESPMulti() {

  startESPSoftAP();
  SERIALESP.println(F("AT+CIPMUX=1"));
  delay(500);
  setESPPort(1, CHANNEL1PORT);
  setESPPort(2, CHANNEL2PORT);
  setESPPort(3, CHANNEL3PORT);
  setESPPort(4, CHANNEL4PORT);
}

void setESPBroadCast() {
  startESPSoftAP();
  SERIALESP.print(F("AT+CIPSTART=\"UDP\",\"255.255.255.255\","));
  SERIALESP.println(CHANNEL1PORT);
  delay(500);
  SERIALESP.println(F("AT+CIPMODE=1"));
  delay(500);
  SERIALESP.println(F("AT+CIPSEND"));
}

#endif

void setSendData() {
  if (conf.wifiMultiPort) {
    setESPMulti();
  } else {
    setESPBroadCast();
  }
}

int getSize(char* ch) {
  int tmp = 0;
  while (*ch) {
    *ch++;
    tmp++;
  }
  return tmp;
}


void sendData(char *message, int id) {
#if defined(SERIAL_MAIN)
  if (conf.SerialMain) {
    SERIAL_MAIN.print(message);
    SERIAL_MAIN.print("\n");
 } 
#endif
#if defined(SERIALESP)
#if defined(ESPAT)
    if (conf.wifiMultiPort) {
      int len = getSize(message) + 1; //sizeof might be wrong
      SERIALESP.print(F("AT+CIPSEND=")); //shaving memory
      SERIALESP.print( id );
      SERIALESP.print(F(","));
      SERIALESP.println(len);
      delay(10);
      SERIALESP.print(message);
      SERIALESP.print(F("\n\r"));
      delay(10);
    } else {
      SERIALESP.println(message);
      SERIALESP.print(F("\n\r"));
    }
#else
    SERIALESP.print(message);
    SERIALESP.print("\n");
    
#endif
#endif
  


}

