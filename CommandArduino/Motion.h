#include <DualVNH5019MotorShield.h>
#include <EnableInterrupt.h>
#include <PID_v1.h>
/* Encoder Pins - using 3 and 11 */
#define E1A  3
#define E1B  5 
#define E2A  11
#define E2B  13
DualVNH5019MotorShield md; 
const int EXPLORATION_SPEED = 350;
const int F1_SPEED = 390;

double M1_Ticks = 0;
double M2_Ticks = 0;
double tickOffset = 0;
double Prev_Error = 0;
double Prev_Tick_R = 0;

/* Variables for PID */
const double kP_1 = 17, kI_1 = 0.9, kD_1 = 0; // Spd 300 | P=13, I=0.85, D=0;
const double kP_2 = 17, kI_2 = 2, kD_2 = 0;    // to find out

PID myPID(&M2_Ticks, &tickOffset, &M1_Ticks, kP_1, kI_1, kD_1, DIRECT);
PID LongDistPID(&M2_Ticks, &tickOffset, &M1_Ticks, kP_2, kI_2, kD_2, DIRECT);

// Can be size 1 for exploration || size 15 if doing multi-step move during exploration
// But algo will need to make some changes 
// For now, get into leaderboard with current settings with algo

int DistanceNormalTicks[15] = {280,590,890,1160,1475,1780,2070,2380,2676,2970,3265,3580,3845,4120,4420}; // PREVIOUSLY 298 - Spd 290, 282 - Spd 300 - 2 Ticks was the secret sauce, Spd 350 - 280
int DistanceF1Ticks[15] = {280,590,890,1160,1475,1780,2070,2380,2676,2970,3268,3580,3845,4120,4420};  // to find out

int LeftTurn_90_Ticks = 383;        // to find out 383, 385
int LeftTurn_90_Ticks_Fast = 383;   // to find out
int LeftTurn_1_Ticks = 8;           // to find out
int LeftTurn_360_Ticks_Fast = 398;

int RightTurn_90_Ticks = 383;       // to find out PREV 383 385 , Spd 350 381
int RightTurn_90_Ticks_Fast = 383;  // to find out
int RightTurn_1_Ticks = 8;          // to find out


void SetUpTicks(){
  M1_Ticks = 0;
  M2_Ticks = 0;
  tickOffset = 0;
  
}
void CountM1Ticks(){
  M1_Ticks++;
}
void CountM2Ticks(){
  M2_Ticks++;
}

void SetUpEncoder(){
  md.init();
  pinMode(E1A, INPUT);
  pinMode(E2A, INPUT);
  enableInterrupt(E1A, CountM1Ticks, RISING);
  enableInterrupt(E2A, CountM2Ticks, RISING);
}
void DisableEncoder(){
  disableInterrupt(E1A);
  disableInterrupt(E2A);
}

void SetUpPID(){
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-400,400);
  myPID.SetSampleTime(5);
}

void StartMotor(){
  md.setSpeeds(0,0);
  md.setBrakes(0,0);
  delay(10);
}
void StopMotor(){
  md.setSpeeds(0,0);
  md.setBrakes(400,400);
  //delay(100);
  delay(75);
}

int ConvertToNormalTicks(int cm){
  return DistanceNormalTicks[(cm/10) - 1];
}

int ConvertToFastTicks(int cm){
  return DistanceF1Ticks[(cm/10) - 1];
  
}
void Forward(int cm, int iAmSpeed){
  SetUpTicks();
  StartMotor();
  int dist;
  double currentSpeed;
  double offset = 0;
  int last_tick_R = 0;
  
  if(iAmSpeed){
    currentSpeed = F1_SPEED;
    dist = ConvertToFastTicks(cm);
  }
  else{
    currentSpeed = EXPLORATION_SPEED;
    dist = ConvertToNormalTicks(cm);
  }
  while( M1_Ticks <= dist || M2_Ticks <= dist){
    if(dist - M2_Ticks < 100) // INITIAL 150
      currentSpeed = 200; // INITIAL 100
    if((M2_Ticks - last_tick_R) >= 10 || M2_Ticks == 0 || M2_Ticks == last_tick_R){
      last_tick_R = M2_Ticks;
      offset += 0.1;
    }
    if(myPID.Compute()||M2_Ticks == last_tick_R){ 
      if(offset >= 1){
        md.setM2Speed(currentSpeed + tickOffset);
        delay(5);
        md.setM1Speed(currentSpeed - tickOffset);
      }
      else{
        md.setM2Speed(offset * (currentSpeed + tickOffset));
        delay(5);
        md.setM1Speed(offset * (currentSpeed - tickOffset));
      }
    }
 }
 StopMotor();
}

void ForwardTicks(int ticks){
  SetUpTicks();
  StartMotor();
  int dist = ticks;
  double currentSpeed = 250;
  while (M2_Ticks <= dist || M1_Ticks <= dist)
  {
    if (myPID.Compute())
      md.setSpeeds(currentSpeed - tickOffset, currentSpeed + tickOffset);
  }
  StopMotor();
}

void Backward(int cm, int iAmSpeed){
  SetUpTicks();
  StartMotor();
  int dist;
  double currentSpeed;
  if(iAmSpeed){
    currentSpeed = F1_SPEED;
    dist = ConvertToNormalTicks(cm);
  }
  else{
   currentSpeed = EXPLORATION_SPEED;
   dist = ConvertToFastTicks(cm);
  }
   
  while (M2_Ticks <= dist || M1_Ticks <= dist){
    if (myPID.Compute()){
      md.setSpeeds(-(currentSpeed - tickOffset), -(currentSpeed + tickOffset));
    }
  }
  StopMotor();
}

void BackwardTicks(int ticks){
  SetUpTicks();
  StartMotor();
  int dist = ticks;
  double currentSpeed = 100;
  while (M2_Ticks <= dist || M1_Ticks <= dist){
    if (myPID.Compute())
      md.setSpeeds(-(currentSpeed - tickOffset), -(currentSpeed + tickOffset));
  }
  StopMotor();
}

void TurnLeft(int angle, int iAmSpeed){
  if (angle == 0)
    angle = 90;

  int i = 0;
  double currentSpeed;
  int ticksToMove;
  
  if(iAmSpeed){
    currentSpeed = 320;
    ticksToMove = LeftTurn_90_Ticks_Fast;
  }
  else{
    currentSpeed = 320;
    ticksToMove = LeftTurn_90_Ticks;
  }
  if(angle == 360){
    ticksToMove = LeftTurn_360_Ticks_Fast;
  }

  if (angle >= 90 && angle % 90 == 0){
    for (i = 0; i<angle; i += 90){
      SetUpTicks();
      StartMotor();
      while (M2_Ticks < ticksToMove || M1_Ticks < ticksToMove)
      {
        if (myPID.Compute())
          md.setSpeeds(-(currentSpeed - tickOffset), currentSpeed + tickOffset);
      }
    }
  }
  else{
    currentSpeed = 100;
    for (i = 0; i < angle; i++){
      SetUpTicks();
      StartMotor();
      while (M2_Ticks < LeftTurn_1_Ticks || M1_Ticks < LeftTurn_1_Ticks)
      {
        if (myPID.Compute())
          md.setSpeeds(-(currentSpeed - tickOffset), currentSpeed + tickOffset);
      }
    }
  }
  StopMotor();
}

void TurnRight(int angle, int iAmSpeed){
  int i = 0;
  double currentSpeed;
  int ticksToMove;
  
  if(iAmSpeed){
    currentSpeed = 320;
    ticksToMove = RightTurn_90_Ticks_Fast;
  }
  else{
    currentSpeed = 320;
    ticksToMove = RightTurn_90_Ticks;
  }

  if (angle >= 90 && angle % 90 == 0){
    for (i=0; i < angle; i += 90){
      SetUpTicks();
      StartMotor();
      while (M2_Ticks < ticksToMove || M1_Ticks < ticksToMove){
        if (myPID.Compute())
          md.setSpeeds((currentSpeed - tickOffset), -(currentSpeed + tickOffset));
      }
    }
  }
  else{
    currentSpeed = 100;
    for (i = 0; i < angle; i++){
      SetUpTicks();
      StartMotor();
      while (M2_Ticks < RightTurn_1_Ticks || M1_Ticks < RightTurn_1_Ticks){
        if(myPID.Compute())
          md.setSpeeds((currentSpeed - tickOffset), -(currentSpeed + tickOffset));
      }
    }
  }
  StopMotor();
} 
