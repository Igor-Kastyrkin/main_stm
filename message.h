
#ifndef _mess_
#define _mess_
#include "uart.h"
//#include <wire.h>
#include <LiquidCrystal_I2C_STM32.h>

#ifdef _DISP_
LiquidCrystal_I2C_STM32 lcd(0x3F, 16, 2);
#endif


//void fErrorMes(String mes);

//void fOtladkaMes(String mes);

void fOtladkaMes(long mes);

//void CheckUpMes(String mes);

void CheckUpMes(const char mes[], int=-12345);

void fErrorMes(const char mes[], int val=-12345);

void fOtladkaMes(const char mes[], int val=-12345);

unsigned strlen(const char *mes);

LiquidCrystal_I2C_STM32 initLcd(int a, int b, int c);

void swap(char *x, char *y);


char* reverse(char *buffer, int i, int j);


char* itoa(int value, char buffer[]);
//int itoa(int value, char* buffer, int base);

#define _DISP_

//#define _OTLADKA_

#ifdef _DISP_
#include <LiquidCrystal_I2C_STM32.h>
static LiquidCrystal_I2C_STM32 lcd(0x3F, 16, 2);
#endif


void strrev(char *p);




#endif
