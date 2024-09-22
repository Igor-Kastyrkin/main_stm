#include "proc.h"

//#define _OTLADKA_



void posOfMotors::SendRightLegCurrentSteps(const char BTEHYTb)
{
  static int i = 0;
  ++i;
  if (((BTEHYTb == 'h') && (RightLegCurrentSteps - OldRightLegCurrentSteps >= 0)) ||
      ((BTEHYTb == 'i') && (RightLegCurrentSteps - OldRightLegCurrentSteps <= 0) && (RightLegCurrentSteps != 0)))
  {
    fErrorMes("WrongRightLegDir", --i);
    delay(500);
    return;
  }
  else if ((BTEHYTb != 'h') && (BTEHYTb != 'i')) {
    fErrorMes("WrongRDirChar");
    //   fErrorMes(BTEHYTb); delay(5000);
    delay(500);
    return;
  }

  char c = BTEHYTb ? 'h' : 'i';

  SerR.prepareMessage(c, RightLegCurrentSteps);
}

void posOfMotors::SendLeftLegCurrentSteps(const char BTEHYTb)
{
  static int i = 0;
  ++i;
  if (((BTEHYTb == 'h') && (LeftLegCurrentSteps - OldLeftLegCurrentSteps >= 0)) ||
      ((BTEHYTb == 'i') && (LeftLegCurrentSteps - OldLeftLegCurrentSteps <= 0) && (LeftLegCurrentSteps != 0)))
  {
    fErrorMes("WrongLeftLegDir", --i);
    delay(500);
    return;
  }
  else if ((BTEHYTb != 'h') && (BTEHYTb != 'i')) {
    fErrorMes("WrongLDirChar");
    delay(500);
    return;
  }
  char c = BTEHYTb ? 'h' : 'i';

  SerL.prepareMessage(c, LeftLegCurrentSteps);
}

void posOfMotors::SendRightFootCurrentSteps()
{
  SerR.prepareMessage('c', RightFootCurrentSteps);
}

void posOfMotors::SendLeftFootCurrentSteps()
{
  SerL.prepareMessage('c', LeftFootCurrentSteps);
}



void posOfMotors::MoveRightLegCurrentSteps(short val, char BTEHYTb)
{
  SetRightLegCurrentSteps(val);
  SendRightLegCurrentSteps(BTEHYTb);
}

void posOfMotors::MoveLeftLegCurrentSteps(short val, char BTEHYTb)
{
  SetLeftLegCurrentSteps(val);
  SendLeftLegCurrentSteps(BTEHYTb);
}

void posOfMotors::MoveRightFootCurrentSteps(long val)
{
  SetRightFootCurrentSteps(val);
  SendRightFootCurrentSteps();
}

void posOfMotors::MoveLeftFootCurrentSteps(long val)
{
  SetLeftFootCurrentSteps(val);
  SendLeftFootCurrentSteps();
}



void posOfMotors::Send_BTAHYTb_Speed(unsigned short Speed)
{
  char tmp1[15];
  if (Speed == 0)
  {
    readString(VtagSpeedAddr, tmp1);
    Speed = atoi(tmp1);
  }
  //  else vtagSpeed = Speed;
  // увеличил делей с 15 до 25, потому что однажды при калибровке скорости ног отличались
  delay(25);
  SerL.prepareMessage('P', Speed);
  delay(25);
  SerR.prepareMessage('P', Speed);
  delay(15);
}

void posOfMotors::Send_BbITAHYTb_Speed(unsigned short Speed)
{
  char tmp1[15];
  if (Speed == 0)
  {
    readString(VytagSpeedAddr, tmp1);
    Speed = atoi(tmp1);
  }
  //  else vytagSpeed = Speed;
  delay(15);
  SerL.prepareMessage('Q', Speed);
  delay(15);
  SerR.prepareMessage('Q', Speed);
  delay(15);
}

void posOfMotors::Send_BTAHYTb_Accel(unsigned short Accel)
{
  char tmp1[15];
  if (Accel == 0)
  {
    readString(VtagAccelAddr, tmp1);
    Accel = atoi(tmp1);
  }
  //  else vtagAccel = Accel;
  delay(15);
  SerL.prepareMessage( 'V', Accel);
  delay(15);
  SerR.prepareMessage( 'V', Accel);
}

void posOfMotors::Send_BbITAHYTb_Accel(unsigned short Accel)
{
  char tmp1[15];
  if (Accel == 0)
  {
    readString(VytagAccelAddr, tmp1);
    Accel = atoi(tmp1);
  }
  //  else vytagAccel = Accel;
  delay(15);
  SerL.prepareMessage( 'W', Accel);
  delay(15);
  SerR.prepareMessage( 'W', Accel);
}


void posOfMotors::Set_Rot_Speed(unsigned short Speed)
{
  //    char temp1[15];
  //	itoa(Speed, temp1,DEC);
  writeStringA(Speed, FootRotSpeedAddr);
  rotSpeed = Speed;
  delay(15);
  SerL.prepareMessage( 'S', rotSpeed);
  delay(15);
  SerR.prepareMessage( 'S', rotSpeed);

}
void posOfMotors::Set_Rot_Accel(unsigned short Accel)
{
  //    char temp1[15];
  //	itoa(Accel, temp1,DEC);
  writeStringA(Accel, FootRotAccelAddr);
  rotAccel = Accel;
  delay(15);
  SerL.prepareMessage( 'X', rotAccel);
  delay(15);
  SerR.prepareMessage( 'X', rotAccel);

}


void posOfMotors::Send_Rot_Speed(unsigned short speed)
{
  char tmp1[15];
  if (speed == 0)
  {
    readString(FootRotSpeedAddr, tmp1);
    rotSpeed = atoi(tmp1);
  }
  else rotSpeed = speed;
  delay(25);
  SerL.prepareMessage( 'S', rotSpeed);
  delay(25);
  SerR.prepareMessage( 'S', rotSpeed);
}

void posOfMotors::Send_Rot_Accel(unsigned short accel)
{
  char tmp1[15];
  if (accel == 0)
  {
    readString(FootRotAccelAddr, tmp1);
    rotAccel = atoi(tmp1);
  }
  else rotAccel = accel;
  delay(25);
  SerL.prepareMessage( 'X', rotAccel);
  delay(25);
  SerR.prepareMessage( 'X', rotAccel);

}

// save current value of left leg position
void posOfMotors::SetLeftLegCurrentSteps(const short val)
{
  if (val > 0)
  {
    fErrorMes("LEFT LEG>0 ERR");
    return;
  }
  OldLeftLegCurrentSteps = LeftLegCurrentSteps;
  LeftLegCurrentSteps = val;
}

// save current value of right leg posititon
void posOfMotors::SetRightLegCurrentSteps(const short val)
{
  if (val > 0)
  {
    fErrorMes("RIGHT LEG>0 ERR");
    return;
  }
  OldRightLegCurrentSteps = RightLegCurrentSteps;
  RightLegCurrentSteps = val;
}


void posOfMotors::Set_BTAHYTb_Speed(unsigned short Speed)
{
  vtagSpeed = Speed;
  // char temp1[15];
  // itoa(Speed, temp1,DEC);
  writeStringA(Speed, VtagSpeedAddr);
}

void posOfMotors::Set_BbITAHYTb_Speed(unsigned short Speed)
{
  vytagSpeed = Speed;
  //	char temp1[15];
  //	itoa(Speed, temp1,DEC);
  writeStringA(Speed, VytagSpeedAddr);
}


void posOfMotors::Set_BTAHYTb_Accel(unsigned short Accel)
{
  vtagAccel = Accel;
  //	char temp1[15];
  //	itoa(Accel, temp1,DEC);
  writeStringA(Accel, VtagAccelAddr);
}

void posOfMotors::Set_BbITAHYTb_Accel(unsigned short Accel)
{
  vytagAccel = Accel;
  //	char temp1[15];
  //	itoa(Accel, temp1,DEC);
  writeStringA(Accel, VytagAccelAddr);
}
