//eeprom.h

#ifndef _eeprom_
#define _eeprom_
#include <arduino.h>

void writeString(String data);

void writeString(String data, unsigned int address);

String readString(int address);

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data);

byte readEEPROM(byte deviceaddress, unsigned int eeaddress);


void PlayFromEEPROM(bool zoom);

#endif

