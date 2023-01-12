
#ifndef _mess_
#define _mess_
#include "uart.h"
//#include <wire.h>
#include <LiquidCrystal_I2C_STM32.h>

void fErrorMes(String mes);

void fOtladkaMes(String mes);

void fOtladkaMes(long mes);

extern LiquidCrystal_I2C_STM32 lcd;

#endif