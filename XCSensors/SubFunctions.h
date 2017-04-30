/*
  XCSensors by Marco van Zoest

  www.primaldev.nl
  www.primalcode.nl

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

void GPSstuff(char c);
void sendNmeaAll();

void sendPTAS1();
void getConfVal(char c);
void sendAccelerometor();
float getCalcHeading();
void meansensors();
void checkAdaptiveVario(double vario);
#if defined(BUZZER)
void makeSound(float vario);
void makeSoundp(int32_t tpressure);

#endif
