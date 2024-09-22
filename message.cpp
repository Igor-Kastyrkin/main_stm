#include "message.h"

#ifndef MESSAGE_H
#define MESSAGE_H

/*
LiquidCrystal_I2C_STM32 initLcd(int a, int b, int c)
{
	return 
}
*/

void printVal(const char mes[], int val)
{
  int size = strlen(mes);
  int i;
  delay(size + 5);
  //  strcat(mes, mes1);
  char result[size + 20];
  for (i = 0; (i < size) && (i < 20)&&(mes[i]!='\0'); i++)
  {
    result[i] = mes[i];
  }
  if (val != -12345)
  {
    char cDataM[15];
    itoa(val, cDataM);
    int sz = strlen(cDataM);
    for (int y = 0; (cDataM[y] != '\0')&&(y < 15)&&(y<sz); i++, y++)
    {
      result[i] = cDataM[y];
    }
  }
  result[i] = '_';
  result[i + 1] = '\r';
  result[i + 2] = '\n';
  result[i + 3] = '\0';
  
  Serial1.println(result);
  
//  return result;
}


void fErrorMes(const char mes[], int val)
{

#ifdef _DISP_
  lcd.clear();
#endif
  printVal(mes,val);
  delay(80);
  
#ifdef _TEL_EEPROM_
  writeTelString(mes);
#endif

#ifdef _DISP_
  lcd.print(mes);
#endif
  delay(1000);
  return;
}





void fOtladkaMes(const char mes[], int val)
{
  //  if(size <= 0) return;
#ifdef _DISP_
  lcd.clear();
#endif

//  Serial1.print(millis() / 1000 + " ");
#ifdef _OTLADKA_
  printVal(mes,val);
#endif

  delay(40);
  delay(50);
#ifdef _TEL_EEPROM_
  writeTelString(mes);
#endif
#ifdef _DISP_
  lcd.print(mes);
#endif
  delay(10);
  return;
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






void CheckUpMes(const char mes[], int val)
{
  printVal(mes,val);
  delay(80);
}


unsigned strlen(const char *str)
{
  unsigned cnt = 0;
  while (*(str + cnt) != 0) ++cnt;
  return cnt;
}





void swap(char *x, char *y)
{
  char t = *x; *x = *y; *y = t;
}



char* itoa(int value, char *buffer)
{

  int n = abs(value);

  int i = 0;

  while (n)
  {
    int r = n % 10;

    if (r >= 10)
    {
      buffer[i++] = 65 + (r - 10);
    }
    else buffer[i++] = 48 + r;
    n = n / 10;
  }
  if (i == 0)
  {
    buffer[i++] = '0';
  }
  if (value < 0) buffer[i++] = '-';
  buffer[i] = '\0';

  strrev(buffer);
  return buffer;
}

void strrev(char *p)
{
  char *q = p;
  while(q && *q) ++q;
  for(--q; p < q; ++p, --q)
    *p = *p ^ *q,
    *q = *p ^ *q,
    *p = *p ^ *q;
}


#endif
