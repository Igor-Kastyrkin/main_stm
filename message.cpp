#include "message.h"

#ifndef MESSAGE_H
#define MESSAGE_H

#define _DISP_

//#define _OTLADKA_

#ifdef _DISP_
//#include <LiquidCrystal_I2C_STM32.h>
#endif


void fErrorMes(String mes)
{
#ifdef _DISP_
  lcd.clear();
#endif
#ifdef _NRF24_
  radio.stopListening();
  char buf[mes.length() + 4];
  mes += "_\r\n";
  mes.toCharArray(buf, mes.length() + 1);
  radio.write(buf, mes.length() + 1);
  radio.startListening();
#else
  delay(mes.length() + 1);
  mes += "_\r\n";
  Serial1.print(millis()/1000 + " ");
  Serial1.print(mes);
#endif
  delay(mes.length() + 1);
  delay(50);
#ifdef _TEL_EEPROM_
  writeTelString(mes);
#endif
#ifdef _DISP_
  lcd.print(mes);
#endif
  delay(1000);
  return;
}












void fOtladkaMes(String mes)
{
#ifdef _DISP_
  lcd.clear();
#endif
#ifdef _OTLADKA_
  delay(10);
  delay(mes.length() + 1);
#ifdef _NRF24_
  radio.stopListening();                 // ��������� �������������
  char buf[mes.length() + 4];
  mes += "_\r\n";
  mes.toCharArray(buf, mes.length() + 1);
  radio.write(buf, mes.length() + 1);  // ���������� ������� ����� ������
  radio.startListening();
#else
  mes += "_\r\n";
  //  Serial.print(mes);
  Serial1.print(mes);
#endif
  delay(mes.length() + 1);
  delay(50);
#endif
#ifdef _TEL_EEPROM_
  writeTelString(mes);
#endif
#ifdef _DISP_
  lcd.print(mes);
#endif
  delay(15);
}













void fOtladkaMes(long mes)
{
#ifdef _DISP_
  lcd.clear();
#endif
  String data;
  data = String(mes) + "_\r\n";
#ifdef _OTLADKA_
  //  Serial.print(data);
  Serial1.print(data);
#endif
#ifdef _DISP_
  lcd.print(mes);
#endif
  delay(15);
}

















void CheckUpMes(String mes)
{

//#ifdef _OTLADKA_
  delay(10);
  delay(mes.length() + 1);
  mes += "_\r\n";
  Serial1.print(mes);
  delay(mes.length() + 1);
  delay(15);
//#endif
}



#endif