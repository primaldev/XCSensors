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
  sendData(nmea.nmeaHumid, conf.humidChannel);
#endif

}

void sendNmeaHEading() {
#if defined(MAG)
  nmea.setMagneticHeading(getCalcHeading());
  sendData(nmea.nmeaMag, conf.magChannel);
#endif

}


void sendAccelerometor() {
#if defined(ACCL)
  sendData(nmea.nmeaGforce, conf.AcclChannel);
#endif
}

void sendPTAS1() { //sends vario data every 155ms on generic GPS channel
#if defined(VARIO)
  sendData(nmea.nmeaPTAS1, conf.GPSChannel);
#endif
}

void sendNmeaAll() {

#if defined(VARIO)
  if (conf.lxnav) {
    sendData(nmea.nmeaVarioLXWP0, conf.varioChannel);
    sendData(nmea.nmeaVario, conf.varioChannel);

  }

#endif
#if defined(MAG)
  sendNmeaHEading();
#endif

#if defined(DHT)
  sendNmeaDHT();
#endif

#if defined(ACCL)
  sendAccelerometor();
#endif

}

float getCalcHeading() {
#if defined(MAG)
  float heading = atan2(my, mx);
  if (heading < 0)
    heading += 2 * M_PI;

  heading = (heading * 180 / M_PI) + conf.magOrientation;

  if (heading > 360) {
    heading = heading - 360;
  }
  return heading;

#else
  return 999;
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
#if defined(SERIALOUT)
  if (conf.SerialMain) {
    SERIALOUT.print(message);
    SERIALOUT.print("\n");
  }
#endif

#if defined(SERIALOUTBT)
  if (conf.SerialMain) {
    SERIALOUTBT.print(message);
    SERIALOUTBT.print("\n");
  }
#endif

#if defined(SERIALOUTUSB)
  if (conf.SerialMain) {
    SERIALOUTUSB.print(message);
    SERIALOUTUSB.print("\n");
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

