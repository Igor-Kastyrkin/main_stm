//eeprom.h

#ifndef _eeprom_
#define _eeprom_
#include <arduino.h>
#include <Wire.h>
#define disk1 0x50



void writeString(String data);

void writeString(String data, unsigned int address);

String readString(int address);

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data);

byte readEEPROM(byte deviceaddress, unsigned int eeaddress);

bool i2cScan(bool&,bool&);

void PlayFromEEPROM(bool zoom);

#endif

extern unsigned long StrAddr;
extern const byte incriment;