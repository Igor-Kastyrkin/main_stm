#pragma once

#include <arduino.h>
//#include "constants.h"
#ifndef _PROC_
#define _PROC_
//#define _NRF24_

#ifdef _NRF24_
#include <nRF24L01-STM.h>
#include <RF24-STM.h>
//#include <SPI.h>
#endif


struct posOfMotors
{};
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
		turnRbst, aPause, UgolL, UgolR};

enum dirflg {left, right, left_right, right_left, middle};

enum robot_leg {left_leg, right_leg, all_legs, not_leg};
enum leg_dir {vtianut, vytianut};
enum rot_dir {leftA, rightA};
enum step_dir {forward, backward, unknown};
enum leedLeg {leftLeed, rightLeed, noLeed};



byte rotPlace(	  posOfMotors & mot, // структура с исходными данными двигателей
                  short minStep,      // угол маленькие шашки, для полного поворота
                  short newOrient, regimRaboty &mode)  ;

bool Leg_fn(robot_leg leg, regimRaboty &regim, leg_dir dir, posOfMotors&, long lDeep = 0);

bool fBreak(robot_leg leg, MKmotor Uzel, /*RF24&,*/posOfMotors& );

bool fAnswerWait(robot_leg leg, MKmotor Uzel, posOfMotors&);

bool change_orient(rot_dir dir, posOfMotors&, float);

bool orient_steps(robot_leg leg, step_dir dir, posOfMotors&, short min_stp);

void StepsManage(posOfMotors&,regimRaboty &mode);

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

void telega_to_center(void);

byte seetUpDown(posOfMotors & mot, const byte mode = 0);

void fAddInActionInRecordMode(actions Action, long param = 0);

void fErrorMes(String mes);

void fOtladkaMes(String mes);

bool fShakeHandWithRotation(posOfMotors & mot, const short stepAngle = 360);

#endif
