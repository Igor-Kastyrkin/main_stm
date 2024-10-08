//eeprom.h

#ifndef _eeprom_
#define _eeprom_
#include <arduino.h>
#include <Wire.h>
#define disk1 0x50
#include "proc.h"


//void writeString(String data);

void readString(int address, char a[]);



void writeString(const char data[], int size = 0);

void writeString(const int data);

void writeStringA(const int data, unsigned int address);



void writeStringA(const char data[], unsigned int address , int size = 0);

//void writeString(String data, unsigned int address);

//String readString(int address);

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data);

byte readEEPROM(byte deviceaddress, unsigned int eeaddress);

bool i2cScan(bool&, bool&);

//void PlayFromEEPROM(bool zoom);

#endif

extern unsigned long StrAddr;
//extern const byte incriment;
