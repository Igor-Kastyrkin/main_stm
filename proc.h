#pragma once

#include <arduino.h>
//#include "constants.h"
#ifndef PROC_H
#define PROC_H
//#define _NRF24_
#include "UART.h"
#include "eeprom.h"
#include "message.h"
#ifdef _NRF24_
//#include <nRF24L01-STM.h>
//#include <RF24-STM.h>
//#include <SPI.h>
#endif


//byte stepDepth = 15; // мм
const short stepsPerLegRot    = 800;//3200; // количество шагов на оборот
const byte lengthPerLegRot   = 4; // за один оборот винта нога перемещается на 8мм
const byte fullRotationLimit = 25; // лимит на количество оборотов
constexpr byte fullRotLengthLimit = lengthPerLegRot * fullRotationLimit;
constexpr byte stepsByMillimeters = stepsPerLegRot / lengthPerLegRot; // 200

const byte steps_rev = 200;
const byte reductor = 50;
const byte m_stp = 4;

constexpr short anglePerStep = steps_rev * reductor * m_stp / 360;

const byte incriment = 15;

constexpr unsigned int VtagSpeedAddr  = 0 + incriment;                  // 15
constexpr unsigned int VytagSpeedAddr = VtagSpeedAddr + incriment;      // 30
constexpr unsigned int VtagAccelAddr  = VytagSpeedAddr + incriment;     // 45
constexpr unsigned int VytagAccelAddr = VtagAccelAddr + incriment;      // 60
constexpr unsigned int FootRotSpeedAddr = VytagAccelAddr + incriment;   // 75
constexpr unsigned int FootRotAccelAddr = FootRotSpeedAddr + incriment; // 90
constexpr unsigned int TelegaSpeedAddr = FootRotAccelAddr + incriment;  // 105
constexpr unsigned int TelegaAccelAddr = TelegaSpeedAddr + incriment;   // 120





class posOfMotors
{

    long rbOrient  = 0;  // угол направления движения в градусах
    // поднимать на 15мм
    unsigned short stepsDepthInSteps = 800L * 15L / 4L;  //12800

    long CurrentZero = 0;



    short LeftLegCurrentSteps   = 0;
    short RightLegCurrentSteps  = 0;

    short OldLeftLegCurrentSteps   = -100;
    short OldRightLegCurrentSteps  = -100;

    long LeftFootCurrentSteps  = 0;
    long RightFootCurrentSteps = 0;

    long OldLeftFootCurrentSteps  = 0;
    long OldRightFootCurrentSteps = 0;


    unsigned short  vtagSpeed = 4500;
    unsigned short vytagSpeed = 3500;

    unsigned short  vtagAccel = 6250;
    unsigned short vytagAccel = 8750;

    unsigned short  rotSpeed = 6000;
    unsigned short  rotAccel = 6500;

  public:
    //  constexpr posOfMotors(){}
    unsigned short GetVtagSpeed() {
      return vtagSpeed;
    }
    unsigned short GetVytagSpeed() {
      return vytagSpeed;
    }
    unsigned short GetVtagAccel() {
      return vtagAccel;
    }
    unsigned short GetVytagAccel() {
      return vytagAccel;
    }

    unsigned short GetRotSpeed() {
      return rotSpeed;
    }
    unsigned short GetRotAccel() {
      return rotAccel;
    }

    unsigned short GetStepsDepthInSteps() {
      return stepsDepthInSteps;
    }

    void SetStepsDepthInSteps(unsigned short steps) {
      stepsDepthInSteps = steps;
    }

    short GetRightLegCurrentSteps() {
      return RightLegCurrentSteps;
    }
    short GetLeftLegCurrentSteps() {
      return LeftLegCurrentSteps;
    }

    long GetRightFootCurrentSteps() {
      return RightFootCurrentSteps;
    }
    long GetLeftFootCurrentSteps() {
      return LeftFootCurrentSteps;
    }


    short GetOrient() {
      return rbOrient;
    }
    long GetCurrentZero() {
      return CurrentZero;
    }

    //  void SetOrient(long _rbOrient) {rbOrient = _rbOrient;}

    long OrientInc(short delta) {
      return rbOrient += delta;
    }

    void SetCurrentZero(long _CurrentZero) {
      CurrentZero = _CurrentZero;
    }

    // save current value of right leg posititon
    void SetRightLegCurrentSteps(const short val);
    // save current value of left leg position
    void SetLeftLegCurrentSteps(const short val);

    void SetRightFootCurrentSteps(const long val) {
      RightFootCurrentSteps = val;
    };
    void SetLeftFootCurrentSteps(const long val) {
      LeftFootCurrentSteps = val;
    };


    void IncRightLegCurrentSteps(long val)
    {
      OldRightLegCurrentSteps = RightLegCurrentSteps;
      RightLegCurrentSteps += val;
    };
    void IncLeftLegCurrentSteps(long val)
    {
      OldLeftLegCurrentSteps = LeftLegCurrentSteps;
      LeftLegCurrentSteps += val;
    };


    void SendRightLegCurrentSteps(const char BTEHYTb);
    void SendLeftLegCurrentSteps(const char BTEHYTb);

    void SendRightFootCurrentSteps();
    void SendLeftFootCurrentSteps();


    void MoveRightLegCurrentSteps(short, char BTEHYTb);
    void MoveLeftLegCurrentSteps(short, char BTEHYTb);

    void MoveRightFootCurrentSteps(long);
    void MoveLeftFootCurrentSteps(long);


    void Set_BTAHYTb_Speed(unsigned short Speed);

    void Send_BTAHYTb_Speed(unsigned short speed = 0);
    void Set_BbITAHYTb_Speed(unsigned short Speed);

    void Send_BbITAHYTb_Speed(unsigned short speed = 0);


    void Set_BTAHYTb_Accel(unsigned short Accel);

    void Send_BTAHYTb_Accel(unsigned short accel = 0);
    void Set_BbITAHYTb_Accel(unsigned short Accel);

    void Send_BbITAHYTb_Accel(unsigned short accel = 0);
	
    /*
      void Send_Estimated_BTAHYTb_Speed();
      void Send_Estimated_BbITAHYTb_Speed();
      void Send_Estimated_BTAHYTb_Accel();
      void Send_Estimated_BbITAHYTb_Accel();
    */

    void Set_Rot_Speed(unsigned short speed);
    void Set_Rot_Accel(unsigned short accel);

    void Send_Rot_Speed(unsigned short speed = 0);
    void Send_Rot_Accel(unsigned short accel = 0);

    void Calibrate_left_leg() {
      SerL.prepareMessage('K');
    }
    void Calibrate_right_leg() {
      SerR.prepareMessage('K');
    }

    void Calibrate_left_foot() {
      SerL.prepareMessage('g');
    }
    void Calibrate_right_foot() {
      SerR.prepareMessage('g');
    }


    //  extern const unsigned int FootRotSpeedAddr;
};
// public:


//};



bool standStill(void);
bool readyForRotLeftFoot(void);
bool readyForRotRightFoot(void);

enum regimRaboty {fast, energySaving}; // 0 - fast, 1 - energySaving;
//enum motor {telega};
enum MKmotor {knee, foot, kal, zero};
enum StadyWork {StRec, StPlay, StWork};
enum actions {walkFf, walkBk, turnL, turnR, wait, standStil, telCenter, goFast, goSlow,
              shakeOn, shakeOff, turnLfst, turnRfst, turnLbst,
              turnRbst, aPause, UgolL, UgolR, DoStep, lUp, rUp,
              shakeRot, Deg10, Deg20, Deg30, Deg45,
              Deg60, Deg70, Deg80, Deg90, Deg180, H20, H30, H50, H80, H120, H160,
              H180, H220, H360, walkUpSt, walkDnSt, legSpeed, legAccel, stDn //, chngOrient
             };

enum dirflg {left, right, left_right, right_left, middle};

enum robot_leg {left_leg, right_leg, all_legs, not_leg};
enum leg_dir {vtianut, vytianut};
enum step_dir {forward, backward, unknown};
//enum leedLeg {leftLeed, rightLeed, noLeed};
//enum mkLegCmd {lSpeedUp, lSpeedDn, lAccelUp, lAccelDn, calibrate, fRollEndCup,
//  fSpeed, fAccel, fSetPos};
enum my_error {no_errors, general_error};
//static my_error err;

struct ledder_var
{
  byte angle = 75;//  90 - 15;
  byte steps_height = 85;
  short StepDepth; // = stepDephCalc(steps_height);
  byte zapas_na_povorot = 14;
  short DeltaStep;// = stepDephCalc(zapas_na_povorot);
  char steps_cnt = 3;
};

struct telSpec {
//TODO add methods to work with eeprom
  int speed = 9000;
  int accel = 9000;
};

byte rotPlace(	  posOfMotors & mot, // структура с исходными данными двигателей
                  short minStep,      // угол маленькие шашки, для полного поворота
                  short newOrient, regimRaboty &mode)  ;

bool Leg_fn( robot_leg leg, const regimRaboty &regim, leg_dir dir, posOfMotors&,
             long lDeep = 0);

bool Leg_pfn( robot_leg leg, const regimRaboty &regim, leg_dir dir, posOfMotors&,
              long lDeep = 0);


bool fCalibration(posOfMotors & mot);

bool fBreak(robot_leg leg, MKmotor Uzel, /*RF24&,*/posOfMotors& );

bool fAnswerWait(robot_leg leg, MKmotor Uzel, posOfMotors&,  char cL = 'T',  char cR = 'T');

// bool change_orient(rot_dir dir, posOfMotors&, float);

bool orient_steps(long stepAngle, robot_leg leg, step_dir dir, posOfMotors&);

void StepsManage(posOfMotors&, const regimRaboty &mode);

void fSendState(StadyWork WorkSt, actions Action,/* RF24&,*/long param = 0/*,posOfMotors&*/);

void fStartPlay(/*RF24&,*/ bool zoom);

byte RF_messege_handle(char *RF_data,/* byte sizeOfRF_data, long x, char *buff,*/ posOfMotors&);

unsigned strlen(const char *str);

byte ReadStream(char&);

//long calc_angle(unsigned short, byte);

void telega_to_center(posOfMotors&);

byte seetUpDown(posOfMotors & mot,  const byte mode = 0);

void fAddInActionInRecordMode(actions Action, long param = 0);

bool fShakeHandWithRotation(posOfMotors & mot, const short stepAngle = 360);

byte upStep(posOfMotors & mot, ledder_var&);

byte dnStep(posOfMotors & mot, step_dir dir,        // FF\BK
            byte BbIcoTa_CTYnEHbKU, long zapas_na_povorot, byte stpsCnt = 3);

byte StepDnWhenBothStepsTogether(posOfMotors & mot, step_dir dir,
                                 short Angle,
                                 long StepDepth,  // BbIcoTa CTYnEHbKU, mm
                                 long DeltaStep) ;

byte goDnstears(posOfMotors & mot, step_dir dir, byte steps_height, long zapas_na_povorot, byte steps_count);

byte StepUpWhenBothStepsTogether(posOfMotors & mot, ledder_var&);
byte stepUpOneStepAlg1(posOfMotors & mot, ledder_var&);

byte stepUpOneStepAlg2(posOfMotors & mot, ledder_var&, robot_leg);

void dataToUpload(void);

// Изменения режима хотьбы быстрый или энергоэффективный
// b = 2 - изменить текущий режим работы на противолополжный
// b = 1 - изменить на режим энергосбережения
// b = 0 - изменить на режим быстрый
void change_mode(regimRaboty &mode, byte b = 2);



//extern const unsigned int VtagSpeedAddr;
//extern const unsigned int VytagSpeedAddr;
//extern const unsigned int VtagAccelAddr;
//extern const unsigned int VytagAccelAddr;



#endif
