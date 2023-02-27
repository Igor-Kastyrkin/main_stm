#include "proc.h"

//#define _OTLADKA_



void posOfMotors::SendRightLegCurrentSteps(char BTEHYTb)
{
  static int i = 0;
  ++i;
  if(((BTEHYTb=='h')&&(RightLegCurrentSteps - OldRightLegCurrentSteps<0))||
  ((BTEHYTb=='i')&&(RightLegCurrentSteps - OldRightLegCurrentSteps>0)))
  {
     fErrorMes("WrongRightLegDir"+(String)--i);
	 delay(500);
	 return;
  }
  else if ((BTEHYTb!='h'||(BTEHYTb!='i'))){ 
     fErrorMes("WrongRDirChar");
	 delay(500);
     return;
  }

  char c = BTEHYTb?'h':'i';
  
  SerR.prepareMessage(c, RightLegCurrentSteps);
}

void posOfMotors::SendLeftLegCurrentSteps(char BTEHYTb)
{
  static int i = 0;
  ++i;
  if(((BTEHYTb=='h')&&(LeftLegCurrentSteps - OldLeftLegCurrentSteps<0))||
  ((BTEHYTb=='i')&&(LeftLegCurrentSteps - OldLeftLegCurrentSteps>0)))
  {
     fErrorMes("WrongLeftLegDir"+(String)--i);
	 delay(500);
	 return;
  }
  else if ((BTEHYTb!='h'||(BTEHYTb!='i'))){ 
     fErrorMes("WrongLDirChar");
	 delay(500);
     return;
  }
  char c = BTEHYTb?'h':'i';
  
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



void posOfMotors::MoveRightLegCurrentSteps(long val, char BTEHYTb)
{
  SetRightLegCurrentSteps(val);
  SendRightLegCurrentSteps(BTEHYTb);
}

void posOfMotors::MoveLeftLegCurrentSteps(long val, char BTEHYTb)
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



void posOfMotors::Send_BTAHYTb_Speed(unsigned long Speed)
{
//  vtagSpeed = Speed;
  delay(15);
  SerL.prepareMessage('P', Speed);
  delay(15);
  SerR.prepareMessage('P', Speed);
}

void posOfMotors::Send_BbITAHYTb_Speed(unsigned long Speed)
{
//  vytagSpeed = Speed;
  delay(15);
  SerL.prepareMessage('Q', Speed);
  delay(15);
  SerR.prepareMessage('Q', Speed);
}

void posOfMotors::Send_BTAHYTb_Accel(unsigned long Accel)
{
//  vtagAccel = Accel;
  delay(15);
  SerL.prepareMessage( 'V', Accel);
  delay(15);
  SerR.prepareMessage( 'V', Accel);
}

void posOfMotors::Send_BbITAHYTb_Accel(unsigned long Accel)
{
//  vytagAccel = Accel;
  delay(15);
  SerL.prepareMessage( 'W', Accel);
  delay(15);
  SerR.prepareMessage( 'W', Accel);
}

void posOfMotors::Send_Estimated_BTAHYTb_Speed()
{
  delay(15);
  SerL.prepareMessage('P', vtagSpeed);
  delay(15);
  SerR.prepareMessage('P', vtagSpeed);  
}

void posOfMotors::Send_Estimated_BbITAHYTb_Speed()
{
  delay(15);
  SerL.prepareMessage('Q', vytagSpeed);
  delay(15);
  SerR.prepareMessage('Q', vytagSpeed);
}

void posOfMotors::Send_Estimated_BTAHYTb_Accel()
{
//  vtagAccel = Accel;
  delay(15);
  SerL.prepareMessage( 'V', vtagAccel);
  delay(15);
  SerR.prepareMessage( 'V', vtagAccel);
}

void posOfMotors::Send_Estimated_BbITAHYTb_Accel()
{
//  vytagAccel = Accel;
  delay(15);
  SerL.prepareMessage( 'W', vytagAccel);
  delay(15);
  SerR.prepareMessage( 'W', vytagAccel);
}


  void posOfMotors::Set_Rot_Speed(unsigned long Speed)
  {
    rotSpeed = Speed;
    delay(15);
    SerL.prepareMessage( 'S', rotSpeed);
	delay(15);
    SerR.prepareMessage( 'S', rotSpeed);

  }
  void posOfMotors::Set_Rot_Accel(unsigned long Accel)
  {
    rotAccel = Accel;
    delay(15);
	SerL.prepareMessage( 'X', rotAccel);
    delay(15);
    SerR.prepareMessage( 'X', rotAccel);

  }

  void posOfMotors::SetLeftLegCurrentSteps(long val)
  {
    if(val > 0)
	{
	   fErrorMes("LEFT LEG>0 ERR");
	   return;
	}
    OldLeftLegCurrentSteps = LeftLegCurrentSteps;
    LeftLegCurrentSteps = val;
  }

  void posOfMotors::SetRightLegCurrentSteps(long val)
  {
    if(val > 0)
	{
	   fErrorMes("RIGHT LEG>0 ERR");
	   return;
	}
    OldRightLegCurrentSteps = RightLegCurrentSteps;
    RightLegCurrentSteps = val;
  }


  void posOfMotors::Set_BTAHYTb_Speed(unsigned long Speed)
  {
    vtagSpeed = Speed;
	writeString(String(Speed), VtagSpeedAddr);
  }
  void posOfMotors::Set_BbITAHYTb_Speed(unsigned long Speed)
  {
    vytagSpeed = Speed;
	writeString(String(Speed), VytagSpeedAddr);
  }

  
  void posOfMotors::Set_BTAHYTb_Accel(unsigned long Accel)
  {
    vtagAccel = Accel;
	writeString(String(Accel), VtagAccelAddr);	
  }

  void posOfMotors::Set_BbITAHYTb_Accel(unsigned long Accel)
  {
    vytagAccel = Accel;
	writeString(String(Accel), VytagAccelAddr);	
  }
