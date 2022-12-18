//#pragma once


#ifndef UART_H
#define UART_H
#include <arduino.h>

class UART_Serial
{
  public:
    // копирующий конструктор для члена stream
    UART_Serial(Stream& U): MySerial(U) {};
    int8_t handle_serial(void);
    //    char* PrintString(void);
    union unionDtStr0 {
#pragma pack(push, 1)
      struct Data {
        uint8_t dev;
        char command;
        char CRC;
      } dt;
      char b[3];
#pragma pack(pop)
    };

    union unionDtStr {
#pragma pack(push, 1)
      struct Data {
        uint8_t dev;
        char command;
        long data;
        char CRC;
      } dt;
      char b[7];
#pragma pack(pop)
    };

    union unionDtStr2 {
#pragma pack(push, 1)
      struct Data {
        uint8_t dev;
        char command;
        long data1;
        long data2;
        char CRC;
      } dt;
      char b[11];
#pragma pack(pop)
    };

    union unionDtStr3 {
#pragma pack(push, 1)
      struct Data {
        uint8_t dev;
        char command;
        long data1;
        long data2;
        long data3;
        char CRC;
      } dt;
      char b[15];
#pragma pack(pop)
    };

    /*
        union unionDtStr4 {
      #pragma pack(push, 1)
          struct Data {
            uint8_t dev;
            char command;
            long data1;
            long data2;
            long data3;
            long data4;
            char CRC;
          } dt;
          char b[19];
      #pragma pack(pop)
        };

        union unionDtStr5 {
      #pragma pack(push, 1)
          struct Data {
            uint8_t dev;
            char command;
            long data1;
            long data2;
            long data3;
            long data4;
            long data5;
            long data6;
            char CRC;
          } dt;
          char b[27];
      #pragma pack(pop)
        };
    */

    void setComRate(unsigned long rate, byte RTS_pin)
    {
      if ((rate > 0) && (rate <= 2000000))
      {
        comm_rate = rate;
        pinMode(RTS_pin, OUTPUT);
        delay(100);
        digitalWrite(RTS_pin, LOW);
        delay(100);
        rts = RTS_pin;
        //        comDelay = ((10000000 + comm_rate * 100) / comm_rate) * 4;
        comDelay = ((72000000L + comm_rate / 2L) / comm_rate) + 10L;
      }
    }
    /*
      unsigned long getComRate(void)
      {
        if (comm_rate)
          return comm_rate;
      }
    */

    unionDtStr0 dtStrOut0, dtStrIn0;
    unionDtStr  dtStrOut,  dtStrIn;
    unionDtStr2 dtStrOut2, dtStrIn2;
    unionDtStr3 dtStrOut3, dtStrIn3;
    //   unionDtStr4 dtStrOut4, dtStrIn4;
    //    unionDtStr5 dtStrIn5;
    void getString0(char &cmd);
    void getString1(char &cmd, long & data);
    void getString2(char &cmd, long & data1, long & data2);
    void getString3(char &cmd, long & data1, long & data2, long & data3);
    //   void getString4(char &cmd, long & dt1, long & dt2, long & dt3, long & dt4);
    //    void getString5(char &cmd, long & dt1, long & dt2, long & dt3, long & dt4, long & dt5, long & dt6);
    //    void prepareMessage0(/*uint8_t dev, */uint8_t cmd, uint8_t length);
    //    void prepareMessage1(/*uint8_t dev, */uint8_t cmd, long data, uint8_t length);
    //    void prepareMessage2(/*uint8_t dev, */uint8_t cmd, long data1, long data2, uint8_t length);
    //    void prepareMessage3(/*uint8_t dev, */uint8_t cmd, long data1, long data2, long data3, uint8_t length);
    // переписать юинт8т в чар
    void prepareMessage(uint8_t cmd);
    void prepareMessage(uint8_t cmd, long data);
    //    void prepareMessage(uint8_t cmd, long data1, long data2);
    //    void prepareMessage(uint8_t cmd, long data1, long data2, long data3);

    char command = '0';
    long Data = 0;

    char crc_calc(char *addr, uint8_t len);

  protected:

  private:
    unsigned long comm_rate;
    const byte MAX_READBYTES = 16;
    byte Serial_amount = 0;
    //    char command_in;
    //    long data_in;
    //    char navSpeed = 0;
    //   long FocusPosition = 0;
    //   char InString[4];
    byte rts;
    long comDelay = 0;//(10000000L + 115200L * 100L) / 115200L;
    //    Stream* MySerial = NULL;
    Stream& MySerial;
};
#endif

extern UART_Serial SerL;
extern UART_Serial SerR;

