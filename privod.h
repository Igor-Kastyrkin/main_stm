// class
#ifndef PRIVOD_H
#define PRIVOD_H

#include "AccelStepper.h"
#include "proc.h"


class privod {
  public:
    //  privod(void);
    //  ~privod(void);
    //	privod(byte ENDCAP1, byte ENDCAP2, int speed, int accel):ENDCAP(ENDCAP1),ENDCAP_R(ENDCAP2), Speed(speed), Accel(accel){}
    void privod_init(byte ENDCAP1, byte ENDCAP2)
    {
      ENDCAP   = ENDCAP1;
      ENDCAP_R = ENDCAP2;
    }
    //------------------- ТЕЛЕЖКА ЕДЕТ ДО КОНЦА ---------------
    int RotateStps4Step(AccelStepper *motorLink, long stepsToGo);

    // ------------------- ТЕЛЕЖКА ЕДЕТ В ЗАДАННОЕ ПОЛОЖЕНИЕ -------------------
    bool RotateStpsOnly(AccelStepper *motorLink, long stepsPos);

    // ---------------- ДОЕЗД ТЕЛЕГИ ------------------
    int fDoezd(AccelStepper *motorLink);
    // short Motor_number = 1;
    // AccelStepper* motorLink[1];

    // ----------------  ТЕЛЕГА в КРАЙ ------------------
    //    int GoToBegining(AccelStepper *motorLink);

    void setTelegaSpec(telSpec &ts/*int &speed, int &accel*/)
    {
      if (ts.speed != 0) Speed = ts.speed;
      if (ts.accel != 0) Accel = ts.accel;
    }

    long getFullDist(void)
    {
      return MaxDistance;
    }

    void SetMaxDist(unsigned short maxDistance)
    {
      //  FULL_DISTANCE_ROT = cData / 100.0;
      MaxDistance = maxDistance;//FULL_DISTANCE_ROT * MICROSTP * TICS_PER_ROT * REDUCTION;
    }
    /*
        String getRotDist(void)
        {
          return String(FULL_DISTANCE_ROT * 100);
        }
    */
    long DriveLeft(void);

    long DriveRight(void);


  private:
    //    float FULL_DISTANCE_ROT = 0.65;
    //    const byte MICROSTP =   2;   // микрошаг
    //    const short TICS_PER_ROT   =   800; // тиков(шагов) на оборот
    //    const byte REDUCTION    =  5;   // передаточное отношение
    unsigned short MaxDistance = 5200;//FULL_DISTANCE_ROT * MICROSTP * TICS_PER_ROT * REDUCTION;
    byte ENDCAP;
    byte ENDCAP_R;
    const bool EC_LOW = HIGH;
    const bool EC_HIGH = LOW;
    unsigned short Speed = 9000;
    unsigned short Accel = 9000;
    const byte stepper_num = 0;
  public:
    dirflg dir_flg = middle;
};

extern privod pr_telega;

#endif
