
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
#include <TimedAction.h>
#include <Wire.h> //1*
#include <DHT.h>
#include <MPU6050.h> //3*
#include "config.h"
#include <MS5611.h>
#include "Conf.h"
#include "SubFunctions.h"
#include <Average.h>
#include "SendData.h"
#include "Audio.h"

/*
 * * IDE Notes
 * 1) For STM32 copy lib from http://www.stm32duino.com/viewtopic.php?t=6 to 
 *    the stm32 harware lib folder (also correct the .\ error)
 * 3) https://github.com/jrowberg/i2cdevlib
 * 
 */


//----------------------------------------------------------------------------//
// Loadable Config Variables
//----------------------------------------------------------------------------//

conf_t conf;

//----------------------------------------------------------------------------//
// Runtime Variables
//----------------------------------------------------------------------------//
bool runloop = true;
bool startwait = false;
bool takeoff = false;
int32_t realPressureAv = 1; //x100 usable stabalized reading (This is used to calculate vario)
int32_t rawPressurePrev = 0; //previous direct reading
int32_t rawPressurePrev2 = 0;
double previousAltitude;
double vario = 0;
double varioAv = 0;
#if defined(ACCL)
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t aax, aay, aaz;
int16_t vectoraz;
float acclOffset=0;
#endif

byte gi = 0;
bool hasrun = false;
#if defined(DHTH)
byte dhttemperature = 0;
byte dhthumidity = 0;
#endif
//----------------------------------------------------------------------------//
// Class Loaders
//----------------------------------------------------------------------------//

TimedAction timedNmea10 = TimedAction(100, collectNmea10); // 10 times per second

#if defined(VARIO)
Average<float> nmea_varioave(10);
Average<float> nmea_altitudeave(10); //Used for calculating vario
TimedAction readVario = TimedAction(VARIOREADMS, readVarioPressure); //processor to fast.

MS5611 baro(BAROADDR);
#if defined(VARIO2)
MS5611 baro_2(BAROADDR2);
#endif
#endif

#if defined(DHTH)
DHT dht;
#endif

#if defined(ACCL)
TimedAction readACCL = TimedAction(ACCLREADMS, readACCLSensor); //processor to fast
MPU6050 accelgyro;
#endif

#if defined(TEENSYDMA)
DMAChannel dmachannel;
#endif

//VoltageReference vRef = VoltageReference();
NMEA nmea;

//----------------------------------------------------------------------------//
// Functions
//----------------------------------------------------------------------------//

void ledOn() {
  digitalWrite(LEDPIN, HIGH);
}

void ledOff() {
  digitalWrite(LEDPIN, LOW);
}


/*
   Collects and process sensor data

   Sensor readings are processed at different timed actions
   this function collects that data.
*/
void collectNmea10() { //runs every 100ms
#if defined(VARIO)

  double realAltitude = baro.getAltitude( realPressureAv, conf.qnePressure); //Based on QN

  nmea_altitudeave.push(realAltitude);
  
  if (nmea_altitudeave.getCount() > 9) {
    vario = nmea_altitudeave.get(9) - nmea_altitudeave.get(0);
  } else {
    vario = 0;
  }
  nmea_varioave.push(vario);

  
  // Direct call to send ptas1, without deadband
  if (conf.ptas1) {     
    nmea.setPTAS1(vario, varioAv, realAltitude);  
    sendPTAS1();
  }   

  

  checkAdaptiveVario(vario);


  previousAltitude = realAltitude;
#endif
  //get ACCL
#if defined (ACCL)
  aax = (ACCLSMOOTH * aax + ax) / (ACCLSMOOTH + 1);
  aay = (ACCLSMOOTH * aay + ay) / (ACCLSMOOTH + 1);
  aaz = (ACCLSMOOTH * aaz + az) / (ACCLSMOOTH + 1);
  vectoraz = (sqrt(pow(aax, 2) + pow(aay, 2) + pow(aaz, 2))) - 1;
#endif


#if defined(ALLFASTDATA)
  getSlowSensorData();
  sendNmeaAll();
#else
#if !defined(GPSTIMER)

  gi++;
  if (gi > 5) { // 6 samples collected
    ledOn();
    getSlowSensorData();
    sendNmeaAll();
    gi = 0;
    ledOff();
  }
#endif

#endif



}

void readACCLSensor() {
#if defined(ACCL)
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
#endif
}

void resetACCLcompVal() {
#if defined(ACCL)
  acclOffset = -(vectoraz * 1000 / 2048 )/1000 + ACCLOFFSET;
#endif
}

/*
   Initialize all sensors

*/
void initSensors() {
#if defined(VARIO)
  baro = MS5611(BAROADDR);
  baro.begin();

#endif
#if defined(VARIO2)
  baro_2 = MS5611(BAROADDR2);
  baro_2.begin();

#endif

#if defined (DHTH)
  dht.setup(DHT_PIN);
#endif

#if defined(ACCL)
  //Accelerometer - needed for the gy-86 board
  accelgyro.setI2CMasterModeEnabled(false);
  accelgyro.setI2CBypassEnabled(true) ;
  accelgyro.setSleepEnabled(false);
  accelgyro.initialize();
  accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
#endif

}


#if defined(GPS) && defined(GPSSERIALEVENT)
void GPSSERIALEVENT() { //Builtin Arduino Function
  if (startwait && runloop) {
    ledOn();  //Led On

    while (SERIALGPS.available()) {
      char inChar = (char)SERIALGPS.read();
      GPSstuff(inChar);
    }

    ledOff(); //Led Off
  }
}
#endif


/*
   Sends vario data and collects other sensor data
   before calling sendNmeaAll.
*/

void getSlowSensorData() { //Sensor data not needed every 100ms
#if defined(VARIO)
  varioAv = nmea_varioave.mean();
  if (conf.lxnav) {
    nmea.setnmeaVarioLXWP0(previousAltitude, nmea_varioave.get(0), nmea_varioave.get(2), nmea_varioave.get(4), nmea_varioave.get(6), nmea_varioave.get(8), nmea_varioave.get(9));
    //  float volt = vRef.readVcc();
    nmea.setNmeaVarioSentence(realPressureAv, previousAltitude, varioAv, baro.getTemperature(), 0 / 1000);
  }
#endif
#if defined (ACCL)
  float gforce = float((vectoraz * 1000) / 2048)/1000 + acclOffset; //x1000 to prevent values smaller than 0.01 being discarded 
  nmea.setGforce(gforce);
#endif

#if defined (DHTH)
  dhttemperature = dht.getTemperature();
  dhthumidity = dht.getHumidity();
  dhthumidity += DHTOFFSET;
#endif

#if defined(ACCL) && defined(DHTH) // kind of a requirement
 //Send C-probe data
 
   nmea.setNmeaPcProbeSentence(float((aax * 1000) / 2048)/1000 , float((aay * 1000) / 2048)/1000, float((aaz * 1000) / 2048)/1000, dhttemperature, dhthumidity, 0);  
   sendPcProbe();
#endif

}

/*
   Read barometer sensors. Called by timed action
   Uses lowpass filter to produce stable output
   In case of dual baro sensors, two options are available:
   Average calculation or least deviation.
*/

void readVarioPressure() {

#if defined(VARIO)
  int32_t pressure;
  pressure = baro.getPressure();

#if defined(VARIO2)
  int32_t pressure2;
  int32_t pressure1t;
  int32_t pressure2t;
  pressure2 = baro_2.getPressure();

#if defined(VARIO2LEASTDEV)
  int32_t diff = rawPressurePrev - pressure;
  int32_t diff2 = rawPressurePrev2 - pressure2;

  //alter the sensor reading
  if (fabs(diff) > fabs(diff2)) { //if the primary has more deviation use the deviation of the secondary sensor
    pressure1t =  rawPressurePrev + diff2;
    pressure2t = pressure2;

  } else {
    pressure2t = rawPressurePrev2 + diff;
    pressure1t = pressure;

  }
  rawPressurePrev = pressure;
  rawPressurePrev2 = pressure2;
  pressure = (pressure1t + pressure2t) / 2;


#else

  pressure = (pressure + pressure2) / 2;

#endif
#endif

  realPressureAv = (double(conf.variosmooth) * realPressureAv + pressure) / (double(conf.variosmooth) + 1);

#endif

//also fire the buzzer
#if defined(BUZZER)
    if (takeoff) { 
        makeVarioAudio(vario);
     }
#endif

}

void runOnce() {
  if (!hasrun) {
#if defined(ACCL)
    resetACCLcompVal();
#endif
#if defined(BTSLEEP)
    if (runloop && !conf.SerialOutBT) {
      digitalWrite(BTENPIN, LOW); //Make BT go ZZ
    }
#endif
  }
  hasrun = true;
}


//----------------------------------------------------------------------------//
// Setup
//----------------------------------------------------------------------------//


void setup() {
#if defined(DEBUG)
  DEBUGSERIAL.println("Setup phase");
#endif
  pinMode(LEDPIN, OUTPUT); //LED
  ledOn();
#if defined(SERIALOUT_BAUD)
  SERIALOUT.begin(SERIALOUT_BAUD);
#endif

#if defined(SERIALOUTBT_BAUD)
  SERIALOUTBT.begin(SERIALOUTBT_BAUD);
#endif

#if defined(GPS)
  SERIALGPS.begin(SERIALGPSBAUD); //for the gps
#endif
  Wire.begin();



#if defined(SERIALESP)
#if defined(WIFIEN_PIN)
  pinMode(WIFIEN_PIN, OUTPUT);
  if (conf.SerialOutESP) {
    digitalWrite(WIFIEN_PIN, HIGH);
  } else {
    digitalWrite(WIFIEN_PIN, LOW);
  }
#endif //WIFIEN_PIN
  SERIALESP.begin(SERIALESPBAUD); //need for speed
#endif //ESPWIFI


#if defined(BTENPIN)
  pinMode(BTENPIN, OUTPUT);
  digitalWrite(BTENPIN, HIGH);
#endif

#if defined(SERIAL_CONFIG_BAUD)
  SERIAL_CONFIG.begin(SERIAL_CONFIG_BAUD);
#endif

#if defined(CONFIGOPT)
  getConfig();
#else
  getDefaultConfig();
#endif
  initSensors();

#if defined(ESPAT)
 setSendDataMultiCast();
#endif


  ledOff(); //If the led stays on, the sensors failed to init
  //vRef.begin(VREFCAL);
#if defined(DEBUG)
  Serial.println("Finished Setup phase");
#endif

}

//----------------------------------------------------------------------------//
// Loop
//----------------------------------------------------------------------------//

void loop() {
 // long loopstart = micros(); //for testing
  long startTime = millis();

  if ( startTime > STARTDELAY) {    //
    startwait = true;
    runOnce();
    
  }


  if (!startwait) { //init the tables so it won't shock the system
#if defined(VARIO)
    readVarioPressure();   
    int32_t alt =   baro.getAltitude( realPressureAv, conf.qnePressure);    
    nmea_altitudeave.push(alt);
#endif
#if defined(ACCL)
    resetACCLcompVal();
#endif
  }

#if defined(CONFIGOPT) && defined(SERIAL_CONFIG)
  if (SERIAL_CONFIG.available()) {
    char inChar = SERIAL_CONFIG.read();
    getConfVal(inChar);
   
  }
#endif

  if (startwait && runloop) {  //Give the sensors time to warm up

    timedNmea10.check();

#if defined(GPS) && !defined(GPSSERIALEVENT)
    if (SERIALGPS.available()) {
      char inChar = (char)SERIALGPS.read();
      GPSstuff(inChar);
    }
#endif

#if defined(VARIO)
    readVario.check();

#endif

#if defined (ACCL)
    readACCL.check();
#endif

#if defined(TAKEOFFVARIO)
    if ( startTime > STARTDELAY + 4000 && !takeoff) {
      if (fabs(vario) > TAKEOFFVARIO) {
        takeoff = true;
      }

    }
#else
  takeoff = true;
#endif



  }

/*
//Loop timer check (for debuging)
Serial.println();
Serial.print("Loop time: ");
Serial.println(micros() - loopstart);
Serial.println();
//On STM32C8 30us on idle, 740us on tx, and sometimes 6000us
*/
}
