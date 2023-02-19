#include "proc.h"

//#define _OTLADKA_



void posOfMotors::SendRightLegCurrentSteps(char BTEHYTb)
{
  static int i = 0;
  ++i;
  if((BTEHYTb=='h')&&(RightLegCurrentSteps - OldRightLegCurrentSteps<0)||
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
  if((BTEHYTb=='h')&&(LeftLegCurrentSteps - OldLeftLegCurrentSteps<0)||
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