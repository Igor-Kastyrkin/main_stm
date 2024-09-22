//eeprom.cpp
#include "eeprom.h"
#include "proc.h"
#include "message.h"


#ifdef _eeprom_
/*
  void writeString(String data)
  {
  int stringSize = data.length();
  if(stringSize > incriment) Serial1.println("EEPROM_INC_ERR");
  for (int i = 0; i < stringSize; i++)
  {
    writeEEPROM(disk1, StrAddr + i, data[i]);
  }
  writeEEPROM(disk1, StrAddr + stringSize, '\0');  //Add termination null character
  //  Serial1.println("_Mem_<"+data);
  StrAddr += incriment;
  return;
  };
*/


void writeString(const char data[], int size)
{
//  int j;
//  for (j = 0; data[j] != '\0'; j++) {}
  int sz = strlen(data);
  if (sz > 14) return;
 // int stringSize = j + 1;
  if ((sz > size) && (size != 0))
  {
    fErrorMes("stringSize=", sz);
    return;
  }
  if (sz > incriment) Serial1.println("EEPROM_INC_ERR");
  for (int i = 0; i < sz; i++)
  {
    writeEEPROM(disk1, StrAddr + i, *(data + i));
  }
  writeEEPROM(disk1, StrAddr + sz, '\0');  //Add termination null character
//  Serial1.print("_Mem_<");
//  Serial1.println(data);
  StrAddr += incriment;
  return;
};

void writeString(const int data)
{

  char cData[incriment]; // for data to save
  itoa(data, cData);
  
  int stringSize = strlen(cData);
  if (stringSize > (incriment - 1))
  {
    fErrorMes("EEPROM_INC_ERR");
    return;
  }

  for (int i = 0; i < stringSize; i++)
  {
    writeEEPROM(disk1, StrAddr + i, cData[i]);
  }
  writeEEPROM(disk1, StrAddr + stringSize, '\0');  //Add termination null character
//  Serial1.println("_Mem_<");
//  Serial1.println(data);
  StrAddr += incriment;
  return;
};


void writeStringA(const int data, unsigned int address)
{

  char cData[incriment]; // for data to save
//  Serial1.print("Data: ");
//  Serial1.println(data);
  
  itoa(data, cData);
  int stringSize = strlen(cData);
//  Serial1.print("cData: ");
//  Serial1.println(cData);
  if (stringSize > (incriment - 1))
  {
    fErrorMes("EEPROM_INC_ERR");
    return;
  }

  for (int i = 0; i < stringSize; i++)
  {
    writeEEPROM(disk1, address + i, cData[i]);  // 5  8  6
  }                                             //[0][1][2] 
  writeEEPROM(disk1, address + stringSize, '\0');  //Add termination null character
//  Serial1.println("_Mem_<");
//  Serial1.println(data);

  return;
};


void writeStringA(const char data[], unsigned int address, int size)
{
  int j;
  for (j = 0; data[j] != '\0'; j++) {}
  if (j > 14) return;
  int stringSize = j + 1;
  if ((stringSize > size) && (size != 0))
  {
    fErrorMes("stringSize=", stringSize);
    return;
  }
  if (stringSize > incriment) Serial1.println("EEPROM_INC_ERR");
  for (int i = 0; i < stringSize; i++)
  {
    writeEEPROM(disk1, address + i, data[i]);
  }
  writeEEPROM(disk1, address + stringSize, '\0');  //Add termination null character
//  Serial1.println("_Mem_<");
//  Serial1.println(data);

  return;
};


/*
  void writeString(String data, unsigned int address)
  {

  int stringSize = data.length();
  if(stringSize > incriment) Serial1.println("EEPROM_INC_ERR");
  for (int i = 0; i < stringSize; i++)
  {
    writeEEPROM(disk1, address + i, data[i]);
  }
  writeEEPROM(disk1, address + stringSize, '\0');  //Add termination null character
  StrAddr += incriment;
  //  fOtladkaMes("EEPROM: ");
  // Serial1.println(String(data));
  return;
  };
*/

#endif








/*

  String readString(int address)
  {
  //  fOtladkaMes("ReadEprom");
  char data[incriment]; //Max 15 Bytes
  int len = 0;
  unsigned char k;
  k = readEEPROM(disk1, address);
  while (k != '\0' && len < incriment)  //Read until null character
  {
    k = readEEPROM(disk1, address + len);
    data[len] = k;
    len++;
  }
  data[len] = '\0';
  //  fOtladkaMes("EEPROM: ");
  //  Serial1.println(String(data));
  return String(data);
  }

*/



void readString(int address, char data[])
{
  //  fOtladkaMes("ReadEprom");
  //  static char data[incriment]; //Max 15 Bytes
  int len = 0;
  unsigned char k;
  k = readEEPROM(disk1, address);
  while (k != '\0' && (len < incriment))  //Read until null character
  {
    k = readEEPROM(disk1, address + len);
    data[len] = k;
    len++;
  }
  data[len] = '\0';
  fOtladkaMes("EEPROM: ");
//  Serial1.println(data);
	
  //  Serial1.println(String(data));

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
  {
    delay(10000);
    fErrorMes("EEPROM WRITE ERROR", err);
  }
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
    fErrorMes("EEPROM READ ERROR", err);
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
  Serial1.println("I2C_Scanning..."); delay(1000);
  nDevices = 0;
  bDisplay = 0;
  bEeprom  = 0;
  for (address = 0x3F; address <= disk1; address++) {
    //  if(address%10==0) Serial1.print(".");
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.

    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      if (address == 0x3F) {
        Serial1.println("Display Available");
        bDisplay = 1;
      }
      if (address == disk1) {
        Serial1.println("EEPROM Available");
        bEeprom = 1;
      }
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
    fOtladkaMes("No I2C devices found");
  else
    fOtladkaMes("done");
  delay(1000);
  return 0;
  //  delay(5000);           // wait 5 seconds for next scan


}
