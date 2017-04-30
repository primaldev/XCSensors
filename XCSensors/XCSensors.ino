
/*
  XCSensors by Marco van Zoest

  www.primaldev.nl
  www.primalcode.nl

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#include "config.h"
#include <Arduino.h>
#include "XCSensors.h"
#include <TimedAction.h>
#include "MS5611.h"
#include <dht.h>
#include <MPU6050.h>
#include <HMC5883L.h>
#include <Wire.h>
//#include <VoltageReference.h>
#include "Conf.h"
#include "SubFunctions.h"
#include "Average.h"
#include "SendData.h"

//----------------------------------------------------------------------------//
// Loadable Config Variables
//----------------------------------------------------------------------------//

conf_t conf;

//----------------------------------------------------------------------------//
// Runtime Variables
//----------------------------------------------------------------------------//
bool runloop = true;
bool startwait = false;
int32_t realPressureAv = 1; //usable stabalized reading
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
#endif
#if defined(MAG)
int16_t mx, my, mz;
#endif
byte gi = 0;
bool hasrun = false;

//----------------------------------------------------------------------------//
// Class Loaders
//----------------------------------------------------------------------------//

TimedAction timedNmea6 = TimedAction(155, collectNmea6); // 6 times per second (155 close enough) / (vario/s is dependant on this)


#if defined(VARIO)
Average<float> nmea_varioave(6);
Average<float> nmea_altitudeave(6); //fixed values i.c.w. timed action
TimedAction readVario = TimedAction(VARIOREADMS, readVarioPressure); //processor to fast.

MS5611 baro(BAROADDR);
#if defined(VARIO2)
MS5611 baro_2(BAROADDR2);
#endif
#endif

#if defined(DHT)
dht dhts;
#endif

#if defined(ACCL)
TimedAction readACCL = TimedAction(ACCLREADMS, readACCLSensor); //processor to fast
MPU6050 accelgyro;
#endif

#if defined(MAG)
HMC5883L mag;
#endif



//VoltageReference vRef = VoltageReference();
NMEA nmea;

//----------------------------------------------------------------------------//
// Functions
//----------------------------------------------------------------------------//

void ledOn() {
  digitalWrite(13, HIGH);
}

void ledOff() {
  digitalWrite(13, LOW);
}

/*
 * Collects and process sensor data
 * Runs every 155 ms a.k.a 6 times per second
 * Sensor readings are processed at different timed actions
 * this function collects that data. 
 */
void collectNmea6() {
#if defined(VARIO)

  double realAltitude = baro.getAltitude( realPressureAv, conf.qnePressure); //Based on QN

  nmea_altitudeave.push(realAltitude);

  if (nmea_altitudeave.getCount() > 5) {
    vario = nmea_altitudeave.get(5) - nmea_altitudeave.get(0);
  } else {
    vario = 0;
  }

  // Direct call to send ptas1
  if (conf.ptas1) { //zero deadband
    float cv = vario * 1.943 * 10 + 200;
    float av = 0;

    long altitudeF = realAltitude * 3.28 + 2000;

#if defined(PTASAVERAGE)
    av = varioAv * 1.943 * 10 + 200;
#endif

    nmea.setPTAS1(cv, av, altitudeF);
    sendPTAS1();
  }

  checkAdaptiveVario(vario);
  
  if (fabs(vario) > 0 && fabs(vario) < conf.varioDeadBand / 1000) {
    vario = 0;
  }
  nmea_varioave.push(vario);
  previousAltitude = realAltitude;
#endif
  //get ACCL
#if defined (ACCL)
  aax = (ACCLSMOOTH * aax + ax) / (ACCLSMOOTH + 1);
  aay = (ACCLSMOOTH * aay + ay) / (ACCLSMOOTH + 1);
  aaz = (ACCLSMOOTH * aaz + az) / (ACCLSMOOTH + 1);
  vectoraz = (sqrt(pow(aax, 2) + pow(aay, 2) + pow(aaz, 2))) - 1;

#endif
#if !defined(GPSTIMER)

  gi++;
  if (gi > 5) { // 6 samples collected
    ledOn();
    sendSensorData();
    gi = 0;
    ledOff();
  }
#endif

}

void readACCLSensor() {
#if defined(ACCL)
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
#endif
}

void resetACCLcompVal() {
#if defined(ACCL)
  conf.accloffset = -(vectoraz / 2048 ) * 1000;
#endif
}

/*
 * Initialize all sensors
 * 
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
#if defined(ACCL)
  //Accelerometer - needed for the gy-86 board
  accelgyro.setI2CMasterModeEnabled(false);
  accelgyro.setI2CBypassEnabled(true) ;
  accelgyro.setSleepEnabled(false);
  accelgyro.initialize();
  accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
#endif
#if defined(MAG)
  mag.initialize();
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
 * Sends vario data and collects other sensor data 
 * before calling sendNmeaAll.
 */

void sendSensorData() {
#if defined(VARIO)
  varioAv = nmea_varioave.mean();
  if (conf.lxnav) {
    nmea.setnmeaVarioLXWP0(previousAltitude, nmea_varioave.get(0), nmea_varioave.get(1), nmea_varioave.get(2), nmea_varioave.get(3), nmea_varioave.get(4), nmea_varioave.get(5), getCalcHeading()); //need to gather 6 samples
    //  float volt = vRef.readVcc();
    nmea.setNmeaVarioSentence(realPressureAv, previousAltitude, varioAv, baro.getTemperature(), 0 / 1000);
  }
#endif
#if defined (ACCL)
  nmea.setGforce((vectoraz / 2048) + (conf.accloffset / 1000) );
#endif

#if defined (DHT)
  dhts.read11(DHT11_PIN);
#endif
#if defined (MAG)
  mag.getHeading(&mx, &my, &mz);
#endif

  sendNmeaAll(); //Send the DATA for all sensors, the vario is leading (except for gps)

}

/*
 * Read barometer sensors. Called by timed action
 * Uses lowpass filter to produce stable output
 * In case of dual baro sensors, two options are available:
 * Average calculation or least deviation. 
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

  realPressureAv = (conf.variosmooth * realPressureAv + pressure) / (conf.variosmooth + 1);

#endif

}

void runOnce() {
  if (!hasrun) {
#if defined(ACCL)
    resetACCLcompVal();
#endif
#if defined(BTSLEEP)
   if (runloop && !conf.SerialMain) {
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
  Serial.println("Setup phase");
#endif
  pinMode(13, OUTPUT); //LED
  ledOn();
#if defined(SERIAL_MAINBAUD)
  SERIAL_MAIN.begin(SERIAL_MAINBAUD);
#endif

#if defined(GPS)
  SERIALGPS.begin(SERIALGPSBAUD); //for the gps
#endif
  Wire.begin();
  


#if defined(SERIALESP)
#if defined(WIFIEN_PIN)
  pinMode(WIFIEN_PIN, OUTPUT);
  if (conf.SerialMain) {
    digitalWrite(WIFIEN_PIN, LOW);
  } else {
    digitalWrite(WIFIEN_PIN, HIGH);
  }
#endif //WIFIEN_PIN
  SERIALESP.begin(SERIALESPBAUD); //need for speed
#endif //ESPWIFI


#if defined(BTENPIN)
  pinMode(BTENPIN, OUTPUT);
  digitalWrite(BTENPIN, HIGH);
#endif 

#if defined(CONFIGOPT)
  getConfig();
#else 
  getDefaultConfig();
#endif
  initSensors();
 
#if defined(ESPAT)
    setSendData();
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
  }

#if defined(CONFIGOPT) && defined(SERIAL_CONFIG)
  if (SERIAL_CONFIG.available()) {
    char inChar = SERIAL_CONFIG.read();
    getConfVal(inChar);

  } 
#endif

  if (startwait && runloop) {  //Give the sensors time to warm up

    timedNmea6.check();

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

#if defined(BUZZER)
    if (conf.buzzer ) {
      makeSound(vario);
    }
#endif
  }

}
