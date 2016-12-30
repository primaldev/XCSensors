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
#include "SubFunctions.h"
#include "XCSensors.h"
#include "config.h" //All other files must use return values or classes
#include "Conf.h"
#include "SendData.h"

byte state=0;  

#if defined(BUZZER)

  unsigned long ttime = 0;
  bool toneon=true;
  bool playtone=false;
#endif 
#if defined(TESTBUZZER)
  unsigned long btime = 0;
  float testvario=0;
#endif
//

void GPSstuff(char c) {                                         // GPSbuffer[] is global
  static int i, j;                                              //   persistent within function scope
  static char q;
  static bool flag = false;
  static char GPSbuffer[120];                                   // GPS serial line input buffer
  bool startSen = false;                                      // Start of NMEA Code
  bool endSen = false;                                        // End NMEA Code
  String nmeaCode;   
  q = c;

  if ( q == 0x24 )                                              // '$'
  {
    i = 0;                                                      // brute force sync on '$' to GPSbuffer[0]
    // Serial << "Found $" << endl;
  }

  if ( i < 120) GPSbuffer[i++] = q;
  
  if (q == 0x0d) {
    flag = true;                                                // is the character a CR for eol
    GPSbuffer[i++] = '\0';
    i = 0;
  }
  
  if (flag) {                                                   // test for end of line and if the right GPSbuffer
    flag = false;                                               // reset for next time    
     sendData(GPSbuffer, conf.GPSChannel); //TODO need testing     
     for (j = 0; j < 120 ; j++) {
        GPSbuffer[j]=0;
     } 
  }

}

void getConfVal(char c) {
    static char Confbuffer[5];
    static char Valbuffer[50];
    static int i, j,y;                                            
    static char q;
    static bool flag = false;
    static bool split =false;
    q = c;

  if ( q == 0x3c)                                              // '<'
  {
    i = 0;                                                      
    y = 0;
  }

  if ( q == 0x3d)                                              // '='
  {
    split=true;
  }

  if ( q == 0x3c || q == 0x3d || q == 0x3e){
    
  }else{
    if (split) {
   
        if ( y < 50) Valbuffer[y++] = q;
      
    }else{
       
          if ( i < 5) Confbuffer[i++] = q;
       
    }
  }
  
  if (q == 0x3e) { // '>'
    flag = true;                                               
    i = 0;
    y = 0;
  }
   //Serial.print(c);
  if (flag) {                                                   
    flag = false;                                               
    split = false;
   #if defined(DEBUG)
      Serial.println(atoi(Confbuffer));
      Serial.println(Valbuffer);
   #endif   
      setConf(atoi(Confbuffer),Valbuffer); 
       
     for (j = 0; j < 5 ; j++) {
        Confbuffer[j]=0;
     }

     for (j = 0; j < 50 ; j++) {
        Valbuffer[j]=0;
     }
  }

}



void sendNmeaDHT(){
    #if defined(DHT11)      
    int temp = (dhts.temperature + 273.15) * 10;
    nmea.setNmeaHumidSentence(temp, dhts.humidity);
    sendData(nmea.nmeaHumid,conf.humidChannel);
    #endif

}

void sendNmeaHEading() {
   #if defined(MAG)
    nmea.setMagneticHeading(getCalcHeading());    
    sendData(nmea.nmeaMag,conf.magChannel);
  #endif
  
}

float getCalcHeading() {
 #if defined(MAG)
    float heading = atan2(my, mx);
    if(heading < 0)
    heading += 2 * M_PI;
    return heading * 180/M_PI;
     
  #else
    return 999;
  #endif
}

void sendAccelerometor() {
  #if defined(ACCL) 
 
   sendData(nmea.nmeaGforce,conf.AcclChannel);
 
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
   
   #if defined(DHT11)
   sendNmeaDHT();
   #endif
   
   #if defined(ACCL)
    sendAccelerometor();
   #endif

   
}

#if defined(BUZZER)
void makeSound(float vario){
  int pulse;

  #if defined(TESTBUZZER)
       vario = testvario;
       int tpassed = millis() - btime;
       if (tpassed > 5000){
         testvario = testvario + 0.5;
         btime = millis();
       }

       if (testvario > 9 ) {
         testvario = 0;
       }
  #endif
  
  if (vario > 9) {
    vario = 9;
    
  }

  
 int variof = (abs(vario) * 200 ) + 800;
  
 
  
  if (abs(vario) >= conf.varioAudioDeadBand/1000) {
    
   pulse = 600/vario;
  } else {
   pulse =600;
  }
  
   if (playtone) {
    if(vario > conf.varioAudioDeadBand/1000 ) {
     
      tone(BUZZPIN,variof);      
       playtone = false;
    } else if(vario < -conf.varioAudioSinkDeadBand/1000 ) {
      
      tone(BUZZPIN,200);
       playtone = false;
    } else {
        noTone(BUZZPIN);  
        pinMode(BUZZPIN, INPUT);
    }
   }
   
    int diff = millis() - ttime;

    if (diff > pulse) {
        noTone(BUZZPIN);
         pinMode(BUZZPIN, INPUT);
        
        if (toneon) {
          toneon = false;
          playtone = false;
          pinMode(BUZZPIN, INPUT);
          ttime = millis() - 50;
        } else  {
          toneon = true;
          playtone = true;
          ttime = millis() + 50;
         
        }
    }    
    
}

#endif



