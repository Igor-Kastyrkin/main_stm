#pragma once

#include <arduino.h>
//#include "constants.h"
#ifndef PROC_H
#define PROC_H
//#define _NRF24_
#include "UART.h"

#include "message.h"
#ifdef _NRF24_
//#include <nRF24L01-STM.h>
//#include <RF24-STM.h>
//#include <SPI.h>
#endif


//byte stepDepth = 15; // мм
const short stepsPerLegRot    = 800;//3200; // количество шагов на оборот
const byte lengthPerLegRot   = 4; // за один оборот винта нога перемещается на 8мм
const long fullRotationLimit = 22;

const short steps_rev = 200;
const short reductor = 50;
const short m_stp = 2;

class posOfMotors
{

  long rbOrient  = 0;  // угол направления движения в градусах
  long stepsDepthInSteps = 800L * 15L / 4L;  //12800  

  long CurrentZero = 0;
  


  long LeftLegCurrentSteps   = 0;
  long RightLegCurrentSteps  = 0;
  
  long OldLeftLegCurrentSteps   = 0;
  long OldRightLegCurrentSteps  = 0;
  
public:  
  long LeftFootCurrentSteps  = 0;
  long RightFootCurrentSteps = 0;

  
  long OldLeftFootCurrentSteps  = 0;
  long OldRightFootCurrentSteps = 0;

  

  
  long GetStepsDepthInSteps(){return stepsDepthInSteps;}
  
  void SetStepsDepthInSteps(long steps){stepsDepthInSteps = steps;}
  
  long GetRightLegCurrentSteps() {return RightLegCurrentSteps;}
  long GetLeftLegCurrentSteps() {return LeftLegCurrentSteps;}
  long GetRightFootCurrentSteps() {return RightFootCurrentSteps;}
  long GetLeftFootCurrentSteps() {return LeftFootCurrentSteps;}

  long GetOrient(){return rbOrient;}
  long GetCurrentZero(){return CurrentZero;}
  
  void SetOrient(long _rbOrient) {rbOrient = _rbOrient;}
  long OrientInc(short delta) {return rbOrient += delta;}
  void SetCurrentZero(long _CurrentZero){CurrentZero = _CurrentZero;}
  
  void SetRightLegCurrentSteps(long val)
  {OldRightLegCurrentSteps = RightLegCurrentSteps; RightLegCurrentSteps = val;};
  void SetLeftLegCurrentSteps(long val)
  {OldLeftLegCurrentSteps = LeftLegCurrentSteps; LeftLegCurrentSteps = val;};
  void SetRightFootCurrentSteps(long val){RightFootCurrentSteps = val;};
  void SetLeftFootCurrentSteps(long val){LeftFootCurrentSteps = val;};

  void IncRightLegCurrentSteps(long val)
  {OldRightLegCurrentSteps = RightLegCurrentSteps; RightLegCurrentSteps += val;};
  void IncLeftLegCurrentSteps(long val)
  {OldLeftLegCurrentSteps = LeftLegCurrentSteps; LeftLegCurrentSteps += val;};
  
  void SendRightLegCurrentSteps(char BTEHYTb);
  void SendLeftLegCurrentSteps(char BTEHYTb);
  void SendRightFootCurrentSteps();
  void SendLeftFootCurrentSteps();
  
  void MoveRightLegCurrentSteps(long, char BTEHYTb);
  void MoveLeftLegCurrentSteps(long, char BTEHYTb);
  void MoveRightFootCurrentSteps(long);
  void MoveLeftFootCurrentSteps(long);
  
  
  
};
// public:


//};



bool standStill(void);
bool readyForRotLeftFoot(void);
bool readyForRotRightFoot(void);

enum regimRaboty {fast, energySaving}; // 0 - fast, 1 - energySaving;
enum motor {telega};
enum MKmotor {knee, foot, kal, zero};
enum StadyWork {StRec, StPlay, StWork};
enum actions {walkFf, walkBk, turnL, turnR, wait, standStil, telCenter, goFast, goSlow,
		shakeOn, shakeOff, turnLfst, turnRfst, turnLbst,
		turnRbst, aPause, UgolL, UgolR, DoStep, lUp, rUp,
		shakeRot, stUp, stDn, Deg10, Deg20, Deg30, Deg45,
		Deg60, Deg70, Deg80, Deg90, Deg180, H20, H30, H50, H80, H120, H160,
		H180, H220, H360, walkUpSt, walkDnSt};

enum dirflg {left, right, left_right, right_left, middle};

enum robot_leg {left_leg, right_leg, all_legs, not_leg};
enum leg_dir {vtianut, vytianut};
enum rot_dir {leftA, rightA};
enum step_dir {forward, backward, unknown};
enum leedLeg {leftLeed, rightLeed, noLeed};



byte rotPlace(	  posOfMotors & mot, // структура с исходными данными двигателей
                  short minStep,      // угол маленькие шашки, для полного поворота
                  short newOrient, regimRaboty &mode)  ;

bool Leg_fn(robot_leg leg, const regimRaboty &regim, leg_dir dir, posOfMotors&,
 long lDeep = 0);

bool Leg_pfn (robot_leg leg, const regimRaboty &regim, leg_dir dir, posOfMotors&,
 long lDeep = 0);
 
bool fBreak(robot_leg leg, MKmotor Uzel, /*RF24&,*/posOfMotors& );

bool fAnswerWait(robot_leg leg, MKmotor Uzel, posOfMotors&,  char cL='T',  char cR ='T');

// bool change_orient(rot_dir dir, posOfMotors&, float);

bool orient_steps(long stepAngle, robot_leg leg, step_dir dir, posOfMotors&);

void StepsManage(posOfMotors&, const regimRaboty &mode);

void fSendState(StadyWork WorkSt, actions Action,/* RF24&,*/long param = 0/*,posOfMotors&*/);

void fStartPlay(/*RF24&,*/ bool zoom);

byte RF_messege_handle(char *RF_data,/* byte sizeOfRF_data, long x, char *buff,*/ posOfMotors&);

unsigned strlen(const char *str);

byte fStepFf(leedLeg leedLgOgj, // c какой ноги идти?
             short angle);
byte fStepBk(leedLeg leedLgOgj, // c какой ноги идти?
             short angle);

byte ReadStream(char&);

long calc_angle(unsigned short,byte);

void telega_to_center(posOfMotors&);

byte seetUpDown(posOfMotors & mot,  const byte mode = 0);

void fAddInActionInRecordMode(actions Action, long param = 0);

void fErrorMes(String mes);

void fOtladkaMes(String mes);

bool fShakeHandWithRotation(posOfMotors & mot, const short stepAngle = 360);

byte upStep(posOfMotors & mot, step_dir dir,        // FF\BK
                               byte BbIcoTa_CTYnEHbKU, byte stpsCnt = 3);
							   
byte dnStep(posOfMotors & mot, step_dir dir,        // FF\BK
                               byte BbIcoTa_CTYnEHbKU, byte stpsCnt = 3);
							   
byte StepDnWhenBothStepsTogether(posOfMotors & mot, step_dir dir, 
								 long StepDepth,  // BbIcoTa CTYnEHbKU, mm
								 long DeltaStep) ; 

#endif
