/*
  XCSensors http://XCSensors.org
  
  Copyright (c), PrimalCode (http://www.primalcode.org)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#if defined(BUZZER)
void makeSound(float vario);
void makeSoundp(int32_t tpressure);
void makeVarioAudio(float vario);
void playToneInterval(int freq, int period, int interval);
#endif
