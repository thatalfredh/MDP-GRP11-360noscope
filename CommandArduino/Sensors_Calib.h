#ifndef Sensor__Calib_h
#define Sensor_Calib_h

#include "SharpIR.h"

SharpIR LEFT_IR(SharpIR::GP2Y0A02YK0F, A0);
SharpIR FRONTLEFT_IR(SharpIR::GP2Y0A21YK0F, A1);
SharpIR FRONTCENTRE_IR(SharpIR::GP2Y0A21YK0F, A2);
SharpIR FRONTRIGHT_IR(SharpIR::GP2Y0A21YK0F, A3);
SharpIR RIGHTFRONT_IR(SharpIR::GP2Y0A21YK0F, A4);
SharpIR RIGHTBACK_IR(SharpIR::GP2Y0A21YK0F, A5);

class Sensor {
  private:
  public:
    int getDist(SharpIR sensor);
    void insertionSort(float array[],int len);
    void returnSensorData(int count);
    float returnDistance(int count, SharpIR sensor, int sensorPosition);
};

void insertionSort(float array[], int length) {
  int i, j;
  float temp;
  for (i = 1; i < length; i++) {
    for (j = i; j > 0; j--) {
      if (array[j] < array[j - 1]) {
        temp = array[j];
        array[j] = array[j - 1];
        array[j - 1] = temp;
      }
      else
        break;
    }
  }
}

float returnDistance(int count, SharpIR sensor, int sensorPosition){
  float arrayR[count];
  float temp;

  for(int i = 0; i < count; i++){
    temp = sensor.getDistance();
    switch(sensorPosition)
    {
      case 1: // FRONTLEFT
            if(temp <= 29)
                arrayR[i] = (temp+0.5481)/0.9648;
            else if(temp > 29 && temp <= 40)
                arrayR[i] = (temp+20.4)/1.6543;
            else
                arrayR[i] = (temp+99.676)/3.7472;
            break;
            
      case 3: // FRONTRIGHT
            if(temp <= 29)
                arrayR[i] = temp;
            else if(temp > 29 && temp <= 44)
                arrayR[i] = (temp+1.9648)/1.0752;
            else if(temp > 44 && temp <= 56)
                arrayR[i] = (temp+3.229)/1.1255;
            else if(temp > 56 && temp <= 71)
                arrayR[i] = (temp+22.597)/1.5255;
            else
                arrayR[i] = (temp-1.8744)/1.1552;
            break;
            
      case 0: // LEFT
            if(temp <= 21)
                arrayR[i] = temp-0.75;
            else if(temp > 21 && temp <= 22)
                arrayR[i] = temp+0.75;
            else if(temp > 22 && temp <= 23)
                arrayR[i] = temp+2;
            else if(temp > 23 && temp <= 60)
                arrayR[i] = (temp-0.227)/0.8392;
            else
                arrayR[i] = (temp-48.644)/1.5457;
            arrayR[i] += 5;
            break;
            
      case 2: // FRONTCENTRE
            if(temp <= 14.5)
                arrayR[i] = temp+1;
            else if(temp > 14.5 && temp <= 31)
                arrayR[i] = (temp+1.4029)/0.9968;
            else if(temp > 31 && temp <= 49)
                arrayR[i] = (temp+2.1383)/1.0514;
            else
                arrayR[i] = (temp-5.3634)/0.9103;
            break;

      case 4: // RIGHTFRONT
            if(temp <= 18)
                arrayR[i] = temp+1;
            else if(temp > 18 && temp <= 34)
                arrayR[i] = temp+2;
            else if(temp > 34 && temp <= 54)
                arrayR[i] = (temp+7.437)/1.1681;
            else
                arrayR[i] = (temp+39.904)/1.7615;
            break;

      case 5: // RIGHTBACK
            if(temp <= 19)
                arrayR[i] = temp;
            else if(temp > 19 && temp <= 35)
                arrayR[i] = temp+1;
            else if(temp > 35 && temp <= 68)
                arrayR[i] = (temp+3.4114)/1.0456;
            else
                arrayR[i] = (temp+42.768)/1.6854;
            break;

      default:
          break;
    }
  }
  //sortArray(arrayR, count);
  insertionSort(arrayR, count);
  float final = arrayR[count/2];
  return arrayR[count/2];
}

/*int getDist(SharpIR sensor){
  int SAMPLESIZE = 20;
  int i;
  float arrayR[SAMPLESIZE];
  
  for (i = 0; i<SAMPLESIZE; i++){
    arrayR[i] = sensor.getDistance();
  }
  insertionSort(arrayR, SAMPLESIZE);
  return arrayR[SAMPLESIZE/2];
}*/

/* Returns sensor data in string to Algorithm - DEPRECATED */ 
/*void ReturnAlgoData(){
  int LEFT_distance = getDist(LEFT);
  if(LEFT_distance <= 19)
    LEFT_distance = 0;
  else if(LEFT_distance >= 45)                         
    LEFT_distance = (LEFT_distance + 5)/10;
  else if(LEFT_distance >= 40) // 4 blocks away
    LEFT_distance = LEFT_distance/10;
  else if(LEFT_distance >= 30)
    LEFT_distance = LEFT_distance/10;
  else if(LEFT_distance >= 22)
    LEFT_distance = LEFT_distance/10;
  else
    LEFT_distance = (LEFT_distance - 3)/10;
  
  int FRONTLEFT_distance = (getDist(FRONTLEFT)-2)/10;
  int FRONTCENTRE_distance = (getDist(FRONTCENTRE)-2)/10;
  int FRONTRIGHT_distance = getDist(FRONTRIGHT)/10;
  int RIGHTFRONT_distance = (getDist(RIGHTFRONT)-4)/10; //-2
  int RIGHTBACK_distance = (getDist(RIGHTBACK)-4)/10; //-2
  
  Serial.println(String(FRONTLEFT_distance) + '|' + String(FRONTCENTRE_distance) + '|' + String(FRONTRIGHT_distance) + '|' + String(RIGHTFRONT_distance) + '|' + String(RIGHTBACK_distance) + '|' + String(LEFT_distance) + '|'); 
}*/

#endif
