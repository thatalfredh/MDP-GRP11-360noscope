#ifndef Sensor_Algo_h
#define Sensor_Algo_h

#include "RunningMedian.h"
#include "avdweb_AnalogReadFast.h"
#include <math.h>

#define LEFT A0
#define FRONTLEFT A1
#define FRONTCENTRE A2
#define FRONTRIGHT A3
#define RIGHTFRONT A4
#define RIGHTBACK A5

int SAMPLESIZE = 20;
//int DELAYTIME = 50;

float getVoltage(int sensor){
  //int SAMPLESIZE = 20;
  int SAMPLESIZE = 10;
  //int MEDIANLISTCOUNT = 5;
  int j;
  float calVoltage;
  
  RunningMedian sample = RunningMedian(SAMPLESIZE);;

    for (j = 0; j<SAMPLESIZE; j++){
      //calVoltage = analogReadFast(sensor)*5.0/1023.0;
      
      calVoltage = analogReadFast(sensor);
      calVoltage *= 5.0;
      calVoltage /= 1023.0;
      
      sample.add(calVoltage);
    }
    return sample.getMedian();
}

// returnCalibratedDistance - translates voltage into distance - DEPRECATED
float returnCalibratedDistance(int sensor, int sensorPosition){
    float mVoltage = getVoltage(sensor);
    float dist = 0;
    switch(sensorPosition){
      case 1: //FRONTLEFT
        dist = 1.9703*pow(mVoltage,4)-18.573*pow(mVoltage,3)+66.982*pow(mVoltage,2)-114.45*mVoltage+82.11;
        break;
        
      case 3: //FRONTRIGHT
        dist = -2.9767*pow(mVoltage,5)+30.828*pow(mVoltage,4)-126.76*pow(mVoltage,3)+262.57*pow(mVoltage,2)-284.49*mVoltage+139.31;
        break;

      case 0: //LEFT
        //dist = -12.643*pow(mVoltage,3)+81.122*pow(mVoltage,2)-187.99*mVoltage+166.65;
        dist = -43.96*log(mVoltage) + 44.604;
        break;

      case 2: //FRONTCENTRE
        dist = -3.5759*pow(mVoltage,5)+35.226*pow(mVoltage,4)-136.27*pow(mVoltage,3)+263.31*pow(mVoltage,2)-267.28*mVoltage+127.36;
        break;

      case 4: //RIGHTFRONT
        dist = -16.454*pow(mVoltage,5)+131.7*pow(mVoltage,4)-418.46*pow(mVoltage,3)+670.54*pow(mVoltage,2)-562.99*mVoltage+212.84;
        break;

      case 5: //RIGHTBACK
        dist = -4.9079*pow(mVoltage,5)+44.369*pow(mVoltage,4)-158.25*pow(mVoltage,3)+286.72*pow(mVoltage,2)-279.77*mVoltage+126.03;
        break;

      default:
        break;
    }
    return dist;
}

float ReturnLeftDistance(int sensor, int sensorPosition){
  //float x = getVoltage(sensor);
  //float y;
  /*if(x > 1.53)
    //y = -18.509*x + 51.598;
    return (-18.509*x + 51.598);
  else if(x > 1.338 && x < 1.53)
    //y =  -30.691*x + 71.057;
    return (-30.691*x + 71.057);
  else if(x < 1.338)
    //y = -50.825*x + 97.136;
    return (-50.825*x + 97.136);
  //return y;*/
  float x = getVoltage(sensor);
  x = log(x);
  x *= -43.96;
  x += 44.604;
  return x;
  //return (-43.96*log(x) + 44.604);
}

float ReturnLeftGridDist(int sensor){
  int i;
  float volt, gridDist, medianVolt;
  RunningMedian sample = RunningMedian(SAMPLESIZE);
  for(i = 0; i<SAMPLESIZE; i++){
    volt = analogReadFast(sensor)*5.0/1023.0;
    sample.add(volt);
  }
  medianVolt = sample.getMedian();
  gridDist = -0.4944*pow(medianVolt,4);
  gridDist += 2.5504*pow(medianVolt,3);
  gridDist -= 2.6039*pow(medianVolt,2);
  gridDist -= 5.7038*medianVolt;
  gridDist += 10.426;
  return round(gridDist);
}

//NEW ReturnSensorData - for Algo - DEPRECATED
void ReturnSensorData(){

  //Add offset values
  int LEFT_offset = 0;
  int FRONTLEFT_offset = 0;
  int FRONTCENTRE_offset = 0;
  int FRONTRIGHT_offset = 0;
  int RIGHTFRONT_offset = 0;
  int RIGHTBACK_offset = 0;
  
  int LEFT_distance = (int)(returnCalibratedDistance(LEFT, 0) + LEFT_offset)/10;
  int FRONTLEFT_distance = (int)(returnCalibratedDistance(FRONTLEFT, 1) + FRONTLEFT_offset)/10;
  int FRONTCENTRE_distance = (int)(returnCalibratedDistance(FRONTCENTRE, 2) + FRONTCENTRE_offset)/10;
  int FRONTRIGHT_distance = (int)(returnCalibratedDistance(FRONTRIGHT, 3) + FRONTRIGHT_offset)/10;
  int RIGHTFRONT_distance = (int)(returnCalibratedDistance(RIGHTFRONT, 4) + RIGHTFRONT_offset)/10;
  int RIGHTBACK_distance = (int)(returnCalibratedDistance(RIGHTBACK, 5) + RIGHTBACK_offset)/10;
  
  Serial.println(String(FRONTLEFT_distance) + '|' + String(FRONTCENTRE_distance) + '|' + String(FRONTRIGHT_distance) + '|' + String(RIGHTFRONT_distance) + '|' + String(RIGHTBACK_distance) + '|' + String(LEFT_distance) + '|'); 
}

int getDist(SharpIR sensor){
  int SAMPLESIZE = 10;
  int i;
  float arrayR[SAMPLESIZE];
  
  for (i = 0; i<SAMPLESIZE; i++){
    arrayR[i] = sensor.getDistance();
  }
  insertionSort(arrayR, SAMPLESIZE);
  return arrayR[SAMPLESIZE/2];
}

/* Returns sensor data in string to Algorithm */ 
void ReturnAlgoData(){
  
  //int LEFT_distance = (ReturnLeftDistance(LEFT, 0) + 5)/10;
  /* Used in Week 9 */
  /*int LEFT_distance;
  float LEFT_RAW = ReturnLeftDistance(LEFT, 0);
  if(LEFT_RAW > 41)
    LEFT_distance = (LEFT_RAW + 10)/10;
  else
    LEFT_distance = (LEFT_RAW + 5)/10;*/

  /* testing for Week 10 */
  int LEFT_distance = ReturnLeftGridDist(LEFT);
  
  int FRONTLEFT_distance = (getDist(FRONTLEFT_IR)-2)/10;
  int FRONTCENTRE_distance = (getDist(FRONTCENTRE_IR)-2)/10;
  int FRONTRIGHT_distance = getDist(FRONTRIGHT_IR)/10;
  int RIGHTFRONT_distance = (getDist(RIGHTFRONT_IR)-4)/10; //-2
  int RIGHTBACK_distance = (getDist(RIGHTBACK_IR)-4)/10; //-2
  Serial.println(String(FRONTLEFT_distance) + '|' + String(FRONTCENTRE_distance) + '|' + String(FRONTRIGHT_distance) + '|' + String(RIGHTFRONT_distance) + '|' + String(RIGHTBACK_distance) + '|' + String(LEFT_distance) + '|'); 
}

#endif  
