//eeprom.cpp
#include "eeprom.h"
#include "proc.h"


#ifdef _eeprom_

void writeString(String data)
{
  int stringSize = data.length();
  for (int i = 0; i < stringSize; i++)
  {
    writeEEPROM(disk1, StrAddr + i, data[i]);
  }
  writeEEPROM(disk1, StrAddr + stringSize, '\0');  //Add termination null character
  StrAddr += incriment;
  return;
};





void writeString(String data, unsigned int address)
{
  int stringSize = data.length();
  for (int i = 0; i < stringSize; i++)
  {
    writeEEPROM(disk1, address + i, data[i]);
  }
  writeEEPROM(disk1, address + stringSize, '\0');  //Add termination null character
  StrAddr += incriment;
  return;
};


#endif










String readString(int address)
{
  fOtladkaMes("ReadEprom");
  char data[10]; //Max 10 Bytes
  int len = 0;
  unsigned char k;
  k = readEEPROM(disk1, address);
  while (k != '\0' && len < 10)  //Read until null character
  {
    k = readEEPROM(disk1, address + len);
    data[len] = k;
    len++;
  }
  data[len] = '\0';
  fOtladkaMes("Done");

  return String(data);
}



















void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data)
{
  byte err = 0;
  Wire.beginTransmission(deviceaddress);
  Wire.write((byte)(eeaddress >> 8));
  Wire.write((byte)(eeaddress & 0xFF));
  Wire.write(data);
  err = Wire.endTransmission();

  if (err)
    fErrorMes("EEPROM WRITE ERROR" + String(err));
  delay(5);
}
















byte readEEPROM(byte deviceaddress, unsigned int eeaddress)
{
  byte err = 0;
  byte rdata = 0xFF;

  Wire.beginTransmission(deviceaddress);
  Wire.write((byte)(eeaddress >> 8));
  Wire.write((byte)(eeaddress & 0xFF));
  err = Wire.endTransmission();
  if (err)
  {
    fErrorMes("EEPROM READ ERROR" + String(err));
    return err;
  }
  Wire.requestFrom(deviceaddress, 1);

  if (Wire.available()) rdata = Wire.read();

  return rdata;
}

