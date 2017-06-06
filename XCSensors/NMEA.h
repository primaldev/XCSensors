/*
  XCSensors http://XCSensors.org
  
  Copyright (c), PrimalCode (http://www.primalcode.org)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/
class NMEA {
  public:
    NMEA();

    void setnmeaVarioLXWP0(double varioAlt, float a, float b, float c, float d, float e, float f);
    void setVarioAlt(float tvarioAlt);
    void setGforce(float gforce);
    void setNmeaVarioSentence(long rawPressure, double varioAlt, float climbRate, float temperature, float voltage);
    void setNmeaHumidSentence(int temperatureH11, int humidity);
    void setPTAS1(float cv, float av, long altitudeF);
    char nmeaGforce[15];
    char nmeaMag[15];
    char nmeaAtt[30];
    char nmeaVarioLXWP0[60];
    char nmeaVario[44];
    char nmeaHumid[22];
    char nmeaPTAS1[44];
  private:

    void getCRC(char *buff);


};




