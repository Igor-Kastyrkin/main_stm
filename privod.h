// class
#ifndef _PRIVOD_
#define _PRIVOD_

#include <AccelStepper.h>
#include "proc.h"


class privod {
  public:
    //  privod(void);
    //  ~privod(void);
    void privod_init(byte ENDCAP1, byte ENDCAP2)
    {
      ENDCAP   = ENDCAP1;
      ENDCAP_R = ENDCAP2;
    }
    //------------------- ТЕЛЕЖКА ЕДЕТ ДО КОНЦА ---------------
    int RotateStps4Step(AccelStepper *motorLink, motor stepper_num, long stepsToGo);

    // ------------------- ТЕЛЕЖКА ЕДЕТ В ЗАДАННОЕ ПОЛОЖЕНИЕ -------------------
    bool RotateStpsOnly(AccelStepper *motorLink, motor stepper_num, long stepsPos);

    // ---------------- ДОЕЗД ТЕЛЕГИ ------------------
    int fDoezd(AccelStepper *motorLink, motor stepper_num);
    // short Motor_number = 1;
    // AccelStepper* motorLink[1];
	
    // ----------------  ТЕЛЕГА в КРАЙ ------------------
    int GoToBegining(AccelStepper *motorLink, motor stepper_num);
	
    void setTelegaSpec(float speed, float accel)
    {
      Speed = speed;
      Accel = accel;
    }

    long getFullDist(void)
    {
      return MaxDistance;
    }

    void SetMaxDist(int cData)
    {
      FULL_DISTANCE_ROT = cData / 100.0;
      MaxDistance = FULL_DISTANCE_ROT * MICROSTP * TICS_PER_ROT * REDUCTION;
    }

    String getRotDist(void)
    {
      return String(FULL_DISTANCE_ROT * 100);
    }
	
	long DriveLeft(void);
	long DriveRight(void);
	

  private:
    float FULL_DISTANCE_ROT = 0.65;
    const byte MICROSTP =   2;   // микрошаг
    const short TICS_PER_ROT   =   800; // тиков(шагов) на оборот
    const byte REDUCTION    =  5;   // передаточное отношение
    long MaxDistance = FULL_DISTANCE_ROT * MICROSTP * TICS_PER_ROT * REDUCTION;
    byte ENDCAP;
    byte ENDCAP_R;
    const bool EC_LOW = HIGH;
    const bool EC_HIGH = LOW;
    float Speed = 0;
    float Accel = 0;
  public:
    dirflg dir_flg = middle;
};

extern privod pr_telega;

#endif
