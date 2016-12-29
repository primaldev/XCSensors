/*
XCsensors by Marco van Zoest

www.primaldev.nl
www.primalcode.nl

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version. see <http://www.gnu.org/licenses/>
*/
class NMEA {
public:
  NMEA(); 

  void setnmeaVarioLXWP0(double varioAlt,float a, float b, float c, float d, float e, float f,float magneticHeading);
  void setVarioAlt(float tvarioAlt);
  void setMagneticHeading(float magneticHeading);
  void setGforce(float gforce);
  void setNmeaVarioSentence(long rawPressure,double varioAlt,float climbRate,float temperature,float voltage);
  void setNmeaHumidSentence(int temperatureH11, int humidity);
  void setPTAS1(float cv, float av, long altitudeF);
 char nmeaGforce[20];
 char nmeaMag[20];
 char nmeaVarioLXWP0[61];
 char nmeaVario[36];
 char nmeaHumid[20];
 char nmeaPTAS1[32];
private:
 
  byte getCRC(char *buff);
  
  
};



 
