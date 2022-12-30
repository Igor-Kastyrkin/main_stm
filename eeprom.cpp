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
//  Serial1.println("_Mem_<"+data);
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
  Serial1.println(String(data));
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


bool i2cScan(bool &bEeprom, bool &bDisplay)
{
  byte error, address;
  int nDevices;
  Serial1.println("I2C_Scanning...");
  nDevices = 0;
  bDisplay = 0;
  bEeprom  = 0;
  for(address = 1; address < 127; address++) {
   if(address%10==0) Serial1.print(".");
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.

    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      if (address == disk1) {
      Serial1.println("EEPROM Available");
      bEeprom = 1;}
      if(address == 0x3F){
	  Serial1.println("Display Available");
	  bDisplay = 1;}
      nDevices++;
    }
    else if (error == 4) {
//	  delay(50);
//      Serial1.print("Unknown error at address 0x");
//      if (address < 16) 
//        Serial1.print("0");
//      Serial1.println(address, HEX);
    }    
  }
  if (nDevices == 0)
    Serial1.println("No I2C devices found"); 
  else
    Serial1.println("done");
  
  return 0;
//  delay(5000);           // wait 5 seconds for next scan


}