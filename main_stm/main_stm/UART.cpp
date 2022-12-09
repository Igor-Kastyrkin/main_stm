
//20.08.19
/*
  1. Добавил алгоритм расчета для deleyMicroseconds
  2. Добавлена переменная адресата для структуры
  21.08.19
  1. Переписал класс чтобы работал с разными Serial портами.
  2. Добавил функцию установки частоты обмена
  3. Добавил функцю возвращающую полученные по каналу значения.
  4. Добавил функцию ожидающую ответ
  4.09.19
  1. Функция handle_serial возвращает кол. принятых байт
  2. Добавил включение отладочных функций через дефайн
  3. Возможность принимать до 3 переменных с данными в сообщении
  Переписать объединение так, чтобы оно не сохраняло коды символов в переменные
*/

#include "Arduino.h"
#include "UART.h"
//#define _OTLADKA1_
//#define _LED_ON_


// Прием по последовательному порту
int8_t UART_Serial::handle_serial(void)
{ // приемный буфер
  char* tmp_buffer = new char[MAX_READBYTES];
  long byteDelay = (10000000L / comm_rate) + 10L;
  uint8_t i_ = 0; // индикатор ошибок
  //  Serial.println("Before SerialAvailable");
  if (MySerial.available())
  {
#ifdef _OTLADKA1_
    Serial1.println("SerialAvailable_");
#endif
#ifdef _LED_ON_
    digitalWrite(LED_BUILTIN, LOW);
#endif
    tmp_buffer[Serial_amount] = MySerial.read();
    Serial_amount++;
    delayMicroseconds(byteDelay);
    /*
      if(isnan((long)tmp_buffer[0]))
      {
      Serial.println("Not a number is serial count");
      goto label;
      }
    */

    while (MySerial.available() && (Serial_amount < MAX_READBYTES)/* && ((byte)tmp_buffer[0] >= Serial_amount)*/)
    {
      tmp_buffer[Serial_amount] = MySerial.read();
      Serial_amount++;
      if (tmp_buffer[Serial_amount] == '\r')
        break;
      //	  if((byte)tmp_buffer[0] <= Serial_amount)
      //		break;
      delayMicroseconds(byteDelay);
    }
    //label:
    while (MySerial.available())  // читаем лишний мусор
    {
      delayMicroseconds(byteDelay);
      MySerial.read();
    }
#ifdef _OTLADKA1_
    Serial1.print("Message_in: "); // для отладки
    for (int j = 0; j < (Serial_amount - 1); j++)
      Serial1.print(tmp_buffer[j]);
    Serial1.println("_"); // для отладки
    Serial1.print("Prinyato: "); // для отладки
    Serial1.println(Serial_amount + "_"); // для отладки
#endif

    char CRC; // для хранения расчетной КС
    CRC = crc_calc(tmp_buffer, Serial_amount - 1); // расчет контрольной суммы

    if (tmp_buffer[Serial_amount - 1] == CRC) // КС совпала?
    {
#ifdef _OTLADKA1_
      Serial1.println("CRC OK_"); // для отладки
#endif
      i_ = Serial_amount;
      command = tmp_buffer[1];
      //только если есть одна переменная data!!!
      switch (Serial_amount)
      {
        case 3:
          break;
        case 7:
          for (int i = 2; i < 6; i++)
          {
            dtStrIn.b[i] = tmp_buffer[i];
          }
          break;
        /*
          case 11:
            for (int i = 2; i < 6; i++)
            {
              dtStrIn2.b[i] = tmp_buffer[i];
            }
            for (int i = 6; i < 10; i++)
            {
              dtStrIn2.b[i] = tmp_buffer[i];
            }
            break;
          case 15:
            for (int i = 2; i < 6; i++)
            {
              dtStrIn3.b[i] = tmp_buffer[i];
            }
            for (int i = 6; i < 10; i++)
            {
              dtStrIn3.b[i] = tmp_buffer[i];
            }
            for (int i = 10; i < 14; i++)
            {
              dtStrIn3.b[i] = tmp_buffer[i];
            }
            break;
          case 19:
            for (int i = 2; i < 6; i++)
            {
              dtStrIn4.b[i] = tmp_buffer[i];
            }
            for (int i = 6; i < 10; i++)
            {
              dtStrIn4.b[i] = tmp_buffer[i];
            }
            for (int i = 10; i < 14; i++)
            {
              dtStrIn4.b[i] = tmp_buffer[i];
            }
            for (int i = 14; i < 18; i++)
            {
              dtStrIn4.b[i] = tmp_buffer[i];
            }
            break;

          case 27:
            for (int i = 2; i < 6; i++)
            {
              dtStrIn5.b[i] = tmp_buffer[i];
            }
            for (int i = 6; i < 10; i++)
            {
              dtStrIn5.b[i] = tmp_buffer[i];
            }
            for (int i = 10; i < 14; i++)
            {
              dtStrIn5.b[i] = tmp_buffer[i];
            }
            for (int i = 14; i < 18; i++)
            {
              dtStrIn5.b[i] = tmp_buffer[i];
            }
            for (int i = 18; i < 22; i++)
            {
              dtStrIn5.b[i] = tmp_buffer[i];
            }
            for (int i = 22; i < 26; i++)
            {
              dtStrIn5.b[i] = tmp_buffer[i];
            }
            break;
        */
        default:
          Serial1.println("<< SerialAmountError >>_");
          Serial1.print("DataIn:"); // для отладки
          for (int j = 0; j < (Serial_amount - 1); j++)
            Serial1.print(tmp_buffer[j]);
          Serial1.println("_"); // для отладки
          Serial1.print("Lenght:"); // для отладки
          Serial1.print(String(Serial_amount)); // для отладки
          Serial1.println("byte_"); // для отладки
          i_ = -1;
      }
	  if(command == 'E')
	  {
	    long a = dtStrIn.dt.data;
		String b = "<<LegErr#" + String(a) + ">>_\r\n"; 
		Serial1.println(b);
		i_ = 0;
	  }
    }
    else
    {
      Serial1.println("CRC ERROR_"); // для отладки
      i_ = -2;
#ifdef _LED_ON_
      /*      for(int i = 0; i<14;i++)
      	  {
      	    digitalWrite(LED_BUILTIN, i%2);
      		delay(300);
      	  }
      */
#endif
#ifdef _OTLADKA1_
      Serial1.print(tmp_buffer[Serial_amount - 1]); // для отладки
      Serial1.print(" <> ");
      Serial1.println(CRC); // для отладки
#endif
      //     prepareMessage1(0x01, 0x00, 0L, 6); // Запрос на повторную отправку
    }
    Serial_amount = 0;
    MySerial.flush();  // !!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef _LED_ON_
    digitalWrite(LED_BUILTIN, HIGH);
#endif
  }
  //  else
  //    i_ = 100;
  for (uint8_t fl = 0; fl < MAX_READBYTES; fl++)
  {
    tmp_buffer[fl] = 0x0;
  }
  delete [] tmp_buffer;
  //  if(i_ !=0){
  //    Serial1.println("SerHandl="+String(i_));
  //    Serial1.println("_");}
  return i_;
  //  delay(200);
};
/*
  char* UART_Serial::PrintString(void)
  {
  return InString;
  }
*/
// Подсчет контрольной суммы посылки
char UART_Serial::crc_calc(char *addr, uint8_t len)
{
  char crc = 0;
  while (len--)
  {
    char inbyte = *addr++;
    for (uint8_t i = 8; i; i--)
    {
      char mix = (crc ^ inbyte) & 0x01;
      crc >>= 1;
      if (mix) crc ^= 0x8C;
      inbyte >>= 1;
    }
  }

  return crc;
}

void UART_Serial::getString0(char &cmd)
{
  cmd  = command;
}

void UART_Serial::getString1(char &cmd, long &data)
{
  cmd  = command;
  data = long(dtStrIn.dt.data);
#ifdef _OTLADKA1_
  Serial1.print("Data in: ");
  Serial1.println(String(data) + "_");
#endif
}
/*
  void UART_Serial::getString2(char &cmd, long &data1, long &data2)
  {
  cmd  = command;
  data1 = long(dtStrIn2.dt.data1);
  data2 = long(dtStrIn2.dt.data2);
  }

  void UART_Serial::getString3(char &cmd, long &data1, long &data2, long &data3)
  {
  cmd  = command;
  data1 = long(dtStrIn3.dt.data1);
  data2 = long(dtStrIn3.dt.data2);
  data3 = long(dtStrIn3.dt.data3);
  }

  void UART_Serial::getString4(char &cmd, long &data1, long &data2, long &data3, long &data4)
  {
  cmd  = command;
  data1 = long(dtStrIn4.dt.data1);
  data2 = long(dtStrIn4.dt.data2);
  data3 = long(dtStrIn4.dt.data3);
  data4 = long(dtStrIn4.dt.data4);
  }

  void UART_Serial::getString5(char &cmd, long &data1, long &data2, long &data3, long &data4, long &data5, long &data6)
  {
  cmd  = command;
  data1 = long(dtStrIn5.dt.data1);
  data2 = long(dtStrIn5.dt.data2);
  data3 = long(dtStrIn5.dt.data3);
  data4 = long(dtStrIn5.dt.data4);
  data5 = long(dtStrIn5.dt.data5);
  data6 = long(dtStrIn5.dt.data6);
  }
*/
// Формирование и отправка посылки
/*
  void UART_Serial::prepareMessage0(uint8_t cmd, uint8_t length1)
  {
  #ifdef _LED_ON_
  digitalWrite(LED_BUILTIN, LOW);
  #endif
  delayMicroseconds(comDelay);
  digitalWrite(rts, HIGH);
  dtStrOut0.dt.dev = length1 + 1;
  dtStrOut0.dt.command = cmd;
  dtStrOut0.dt.CRC = crc_calc(dtStrOut0.b, length1);
  #ifdef _OTLADKA1_
  Serial.print("LENGTH_out: ");
  Serial.println(length1 + 1);
  Serial.print("Message_out: ");
  Serial.write(dtStrOut0.b, length1 + 1);
  Serial.println(".");
  #endif
  MySerial.write(dtStrOut0.b, length1 + 1);
  delayMicroseconds(comDelay);
  digitalWrite(rts, LOW);
  #ifdef _LED_ON_
  digitalWrite(LED_BUILTIN, HIGH);
  #endif
  }
*/
void UART_Serial::prepareMessage(uint8_t cmd)
{
#ifdef _LED_ON_
  digitalWrite(LED_BUILTIN, LOW);
#endif
  delayMicroseconds(comDelay);
  digitalWrite(rts, HIGH);
  dtStrOut0.dt.dev = 2 + 1;
  dtStrOut0.dt.command = cmd;
  dtStrOut0.dt.CRC = crc_calc(dtStrOut0.b, 2);
#ifdef _OTLADKA1_
  Serial1.print("LENGTH_out: ");
  Serial1.println("3_");
  Serial1.print("Message_out: ");
  Serial1.write(dtStrOut0.b, 2 + 1);
  Serial1.println("_");
#endif
  MySerial.write(dtStrOut0.b, 2 + 1);
  delayMicroseconds(comDelay);
  digitalWrite(rts, LOW);
#ifdef _LED_ON_
  digitalWrite(LED_BUILTIN, HIGH);
#endif
}

// Формирование и отправка посылки
/*
  void UART_Serial::prepareMessage1(uint8_t cmd, long data, uint8_t length1)
  {

  #ifdef _LED_ON_
  digitalWrite(LED_BUILTIN, LOW);
  #endif
  delayMicroseconds(comDelay);
  digitalWrite(rts, HIGH);
  dtStrOut.dt.dev = length1 + 1;
  dtStrOut.dt.command = cmd;
  dtStrOut.dt.data = data;
  dtStrOut.dt.CRC = crc_calc(dtStrOut.b, length1);
  #ifdef _OTLADKA1_
  Serial.print("LENGTH_out: ");
  Serial.println(length1 + 1);

  Serial.print("Message_out: ");
  Serial.write(dtStrOut.b, length1 + 1);
  Serial.println(".");
  #endif
  MySerial.write(dtStrOut.b, length1 + 1);
  delayMicroseconds(comDelay);
  digitalWrite(rts, LOW);
  #ifdef _LED_ON_
  digitalWrite(LED_BUILTIN, HIGH);
  #endif
  }
*/
void UART_Serial::prepareMessage(uint8_t cmd, long data)
{
  byte length1 = 6;
#ifdef _LED_ON_
  digitalWrite(LED_BUILTIN, LOW);
#endif
  delayMicroseconds(comDelay);
  digitalWrite(rts, HIGH);
  dtStrOut.dt.dev = length1 + 1;
  dtStrOut.dt.command = cmd;
  dtStrOut.dt.data = data;
  dtStrOut.dt.CRC = crc_calc(dtStrOut.b, length1);
#ifdef _OTLADKA1_
  Serial1.print("LENGTH_out: ");
  Serial1.println("7_");

  Serial1.print("Message_out: ");
  Serial1.write(dtStrOut.b, length1 + 1);
  Serial1.println("_");
#endif
  MySerial.write(dtStrOut.b, length1 + 1);
  delayMicroseconds(comDelay);
  digitalWrite(rts, LOW);
#ifdef _LED_ON_
  digitalWrite(LED_BUILTIN, HIGH);
#endif
}
// Формирование и отправка посылки
/*
  void UART_Serial::prepareMessage2(uint8_t cmd, long data1, long data2, uint8_t length1)
  {
  #ifdef _LED_ON_
  digitalWrite(LED_BUILTIN, LOW);
  #endif
  digitalWrite(rts, HIGH);
  dtStrOut2.dt.dev = length1 + 1;
  dtStrOut2.dt.command = cmd;
  dtStrOut2.dt.data1 = data1;
  dtStrOut2.dt.data2 = data2;
  dtStrOut2.dt.CRC = crc_calc(dtStrOut2.b, length1);
  #ifdef _OTLADKA1_
  Serial.print("Message_out: ");
  #endif
  MySerial.println((char*)dtStrOut2.b);
  delayMicroseconds(comDelay);
  digitalWrite(rts, LOW);
  #ifdef _LED_ON_
  digitalWrite(LED_BUILTIN, HIGH);
  #endif
  }
*/
// Формирование и отправка посылки
/*
  void UART_Serial::prepareMessage3(uint8_t cmd, long data1, long data2, long data3, uint8_t length1)
  {
  digitalWrite(rts, HIGH);
  dtStrOut3.dt.dev = length1 + 1;
  dtStrOut3.dt.command = cmd;
  dtStrOut3.dt.data1 = data1;
  dtStrOut3.dt.data2 = data2;
  dtStrOut3.dt.data3 = data3;
  dtStrOut3.dt.CRC = crc_calc(dtStrOut3.b, length1);
  #ifdef _OTLADKA1_
  Serial.print("Message_out: ");
  #endif
  MySerial.println((char*)dtStrOut3.b);
  delayMicroseconds(comDelay);
  digitalWrite(rts, LOW);
  }
*/
#ifdef SERIAL_USB
UART_Serial SerL(Serial2);
UART_Serial SerR(Serial3);
#else
UART_Serial SerL(Serial2);//SerL(Serial1);
UART_Serial SerR(Serial3);//SerR(Serial2);
#endif


