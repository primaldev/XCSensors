/*
  XCSensors http://XCSensors.org
  
  Copyright (c), PrimalCode (http://www.primalcode.org)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#include <Arduino.h>
#include "XCSensors.h"
#include "SendData.h"
#include "config.h"
#include "Conf.h"


void sendNmeaDHT() {
#if defined(DHT)
  int temp = (dhttemperature + 273.15) * 10;
  nmea.setNmeaHumidSentence(temp, dhthumidity);
  sendData(nmea.nmeaHumid);
#endif

}

void sendPcProbe() {
#if defined(ACCL) && defined(DHT)
  sendData(nmea.nmeaPcProbe);
#endif
}

void sendAccelerometor() {
#if defined(ACCL)
  sendData(nmea.nmeaGforce);
#endif
}

void sendPTAS1() { //sends vario data every 155ms on generic GPS channel
#if defined(VARIO)
  sendData(nmea.nmeaPTAS1);
#endif
}


//Send all data
//Here is a good spot to use configuration switches to turn stuff on and off
void sendNmeaAll() {

#if defined(VARIO)
  if (conf.lxnav) {
    sendData(nmea.nmeaVarioLXWP0);
    sendData(nmea.nmeaVario);

  }

#endif

#if defined(DHT)
  sendNmeaDHT();
#endif

#if defined(ACCL)
  sendAccelerometor();
#endif

}


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


void setSendDataMultiCast() {
  startESPSoftAP();
  SERIALESP.print(F("AT+CIPSTART=\"UDP\",\"255.255.255.255\","));
  SERIALESP.println(CHANNEL1PORT);
  delay(500);
  SERIALESP.println(F("AT+CIPMODE=1"));
  delay(500);
  SERIALESP.println(F("AT+CIPSEND"));
}
#endif

int getSize(char* ch) {
  int tmp = 0;
  while (*ch) {
    *ch++;
    tmp++;
  }
  return tmp;
}


void sendData(char *message) {
char newl[2] = "\n";
  
#if defined(SERIALOUT)
  if (conf.SerialOut) {
    SERIALOUT.print(message);
    SERIALOUT.print(newl);
  }
#endif

#if defined(SERIALOUTBT)
  if (conf.SerialOutBT) {
    SERIALOUTBT.print(message);
    SERIALOUTBT.print(newl);
  }
#endif

#if defined(SERIALOUTUSB)
  if (conf.SerialOutUSB) {
    SERIALOUTUSB.print(message);
    SERIALOUTUSB.print(newl); 
  }
#endif

#if defined(SERIALESP)
  if (conf.SerialOutESP) {
#if defined(ESPAT)  
    SERIALESP.print(message); 
    SERIALESP.print(newl);   
#else
    SERIALESP.print(message);
    SERIALESP.print(newl);
  }
#endif
#endif



}

