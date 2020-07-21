#ifndef Calibration_V_h
#define Calibration_V_h

#include "Sensors_Calib.h"
#include "Motion.h"

DualVNH5019MotorShield md_calib;
float calibrationSetSpeed1 = 220; //220; - worked fine //120;
float calibrationSetSpeed2 = 260; //260; - worked fine //160;


void CalibrateParallel(SharpIR sensor1, SharpIR sensor2, long difference) {
  
  float leftDistance = round(returnDistance(1, sensor1, 1));
  float rightDistance = round(returnDistance(1, sensor2, 3));

  while (fabs(rightDistance - leftDistance) != difference) {
    if (rightDistance - leftDistance > difference) {
      md_calib.setSpeeds(-calibrationSetSpeed1, calibrationSetSpeed2);
      delay(100);
      md_calib.setM1Brake(360);
      md_calib.setM2Brake(360);
      delay(150);
    }
    if (rightDistance - leftDistance < difference) {
      md_calib.setSpeeds(calibrationSetSpeed1, -calibrationSetSpeed2);
      delay(100);
      md_calib.setM2Brake(360);
      md_calib.setM1Brake(360);
      delay(150);
    }
    leftDistance = round(returnDistance(1, sensor1, 1));
    rightDistance = round(returnDistance(1, sensor2, 3));
  }
}

void CalibrateDistance(SharpIR sensor1, long offset1, SharpIR sensor2,  long offset2, long difference) {

  float leftDistance = round(returnDistance(1, sensor1, 1));
  float rightDistance = round(returnDistance(1, sensor2, 3));
  
  if (fabs(rightDistance - leftDistance) != difference) {
    CalibrateParallel(sensor1, sensor2, difference);
  }

  leftDistance = round(returnDistance(1, sensor1, 1));
  rightDistance = round(returnDistance(1, sensor2, 3));
  
  float setM1, setM2;
  do {
    if (leftDistance < offset1)
      setM1 = -calibrationSetSpeed1;
    else if (leftDistance > offset1)
      setM1 = calibrationSetSpeed1;
    else
      setM1 = 0;

    if (rightDistance < offset2)
      setM2 = -calibrationSetSpeed2;
    else if (rightDistance > offset2)
      setM2 = calibrationSetSpeed2;
    else
     setM2 = 0;

    md_calib.setSpeeds(setM1, setM2);
    delay(10);
    md_calib.setM2Brake(360);
    md_calib.setM1Brake(360);
    delay(15);

    leftDistance = round(returnDistance(1, sensor1, 1));
    rightDistance = round(returnDistance(1, sensor2, 3));
  }  while (!(rightDistance == offset2 && leftDistance == offset1));
}

void FrontCalibration() {
  CalibrateDistance(FRONTLEFT_IR, 12, FRONTRIGHT_IR, 11, 1);
}

// ALL INTIAL DELAYS WERE delay(50);
// DELAYS(25) WORKED FINE;

void FrontWallCalibration(){
  FrontCalibration();
  delay(25);
  ForwardTicks(114);
}
void RightCalibration(){
  TurnRight(90, 0);
  delay(25);
  FrontCalibration();
  delay(25);
  ForwardTicks(114);
  delay(25);
  TurnLeft(90, 0); 
}
void CornerCalibration(){
  TurnRight(90, 0);
  delay(25);
  FrontCalibration();
  delay(25);
  ForwardTicks(114);
  delay(25);
  TurnLeft(90,0);
  delay(25);
  FrontCalibration();
  delay(25);
  ForwardTicks(114); // PREVIOUSLY 104
  delay(25);
  TurnLeft(90, 0);
}
#endif
