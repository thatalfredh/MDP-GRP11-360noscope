  /* Things to always check
 *  1. Ticks per Grid 
 *  2. Ticks for Burst Move ( > 1 grid per advancement)
 *  3. Accuracy of turns 
 *  4. Calibration
 *  5. Comment out functional testing loop for exploratory run
 */

#include "Calibration_V.h"
#include "Sensors_Algo.h"

#define STR_SIZE 512
char instruction[STR_SIZE];
int instIn, instOut;
int I_AM_SPEED = 0;
//int BAD_MANNERS = 1;

void setup(){
  Serial.begin(115200);
  SetUpEncoder();
  SetUpPID();
}

/* === Run with testString (no Algo/Sensors) === */
/*bool testRun = true;
//String testString = "K"; // right calibration -ok , left turn short | now ok
//String testString = "H"; // corner calibration - ok
//String testString = "A"; // test left turn - ok
//String testString = "D"; // right turn - ok
//String testString = "R"; // forward steps
//String testString = "I"; // 9 grid burst

void loop(){
  while(testRun){
    int i;
    for(i=0; i<testString.length(); i++){
      instruction[i] = testString[i];
    }
    i=0;
    while(i<testString.length()){
      Execute(instruction[i]);
      delay(75);
      i++;
    }
    testRun = false;
  }
}*/

/* === Main Loop | Arduino - RPI communication === */
void loop() { 
  if(ThereIsCommand()){ 
    while(instOut < instIn){
      Execute(instruction[instOut]);
      delay(75);
      instOut++;
    }
    instOut = 0;
  }
  delay(50);
}

/* Get commands from RPI */ 
boolean ThereIsCommand(){
  instIn = 0;
  while(Serial.available() > 0){
    instruction[instIn] = Serial.read();
    instIn++;
    delay(1);
    if(instIn >= STR_SIZE)
      break;
  }
  if(instIn > 0)
    return true;
  else
    return false;
}

/* Execute command given */
void Execute(char instruction){ 
  switch (instruction) {
    case 'L':
      ReturnAlgoData();
      break;
    case 'W':
      Forward(10, I_AM_SPEED);
      break;
    case 'A':
      TurnLeft(90, I_AM_SPEED);
      break;
    case 'D':
      TurnRight(90, I_AM_SPEED);
      break;
    case 'K' :
      RightCalibration();
      break;
    case 'H' :
      CornerCalibration();
      break;
    case 'B':
      FrontWallCalibration();
      break;
    case 'Q':
      Forward(20, I_AM_SPEED);
      break;
    case 'E':
      Forward(30, I_AM_SPEED);
      break;
    case 'R':
      Forward(40, I_AM_SPEED);
      break;  
    case 'T':
      Forward(50, I_AM_SPEED);
      break;
    case 'Y':
      Forward(60, I_AM_SPEED);
      break;
    case 'U':
      Forward(70, I_AM_SPEED);
      break;
    case 'I':
      Forward(80, I_AM_SPEED);
      break;
    case 'O':
      Forward(90, I_AM_SPEED);
      break;
    case 'P':
      Forward(100, I_AM_SPEED);
      break;
    /*case '1':
      Forward(110, I_AM_SPEED);
      break;
    case '2':
      Forward(120, I_AM_SPEED);
      break;
    case '3':
      Forward(130, I_AM_SPEED);
      break;
    case '4':
      Forward(140, I_AM_SPEED);
      break;
    case '5':
      Forward(150, I_AM_SPEED);
      break;*/
    case 'S':
      Backward(10, I_AM_SPEED);
      break;
    case 'J':
      TurnLeft(90, I_AM_SPEED);
      delay(50);
      TurnLeft(90, I_AM_SPEED);
      break;
    case 'V': //ENDEXPLORATION - SOUTH
      CornerCalibration();
      I_AM_SPEED = 1;
      BAD_MANNERS = 1;
      break;
    case 'M': //ENDEXPLORATION - WEST
      TurnLeft(90, I_AM_SPEED);
      CornerCalibration();
      I_AM_SPEED = 1;
      BAD_MANNERS = 1;
      break;
    case 'X':
      delay(100);
      break;
    case 'Z':
      I_AM_SPEED = 1;
      break;
    default:
      break;
  }
} 

void BM_360NoScope(){
 TurnLeft(360, I_AM_SPEED);
 delay(25);
 //Backward(10, I_AM_SPEED);
 //delay(25); 
}  
