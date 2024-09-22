// проблемы с записью тележки и предпоследние действие выполняется без паузы почему-то
//( исправил закомментировав лишнюю запись конца времени, заменил функцию на такую, чтобы записывалась пауза при хотьбе)
// разобраться с дисплеем
// (исправил, используя дисплей как статическую переменную)
// один раз была ошибка после перекалибровки с типом шагания. Он почему-то поменялся
// (исправил ошибки при смене режима шагания)
// один раз после калибровки у ног были разные скорости или ускореиня подъема и опускания
// (увеличил паузу между командами для ног с 15 до 25 мс)
// уменьшить задержку при воспроизведении (уменьшил, убрав комментарии)


/* глюки

  3. При записи пусть записывается и угол дошагания

  4. Предусмотреть, чтоб не делал лишних шагов



*/


// ПРОВЕРИТЬ ПРИ ОТЛАДКЕ В ПЕРВУЮ ОЧЕРЕДЬ:
// 1. ЗАПОМИНАНИЕ СКОРОСТЕЙ И УСКОРЕНИЙ ПОСЛЕ ВЫКЛЮЧЕНИЯ
// 2. РЕЖИМЫ ЗАПИСИ И ВОСПРОИЗВЕДЕНИЯ
// 3. ПРОВЕРИТЬ КОРРЕКТНОСТЬ СКОРОСТЕЙ ВТЯГИВАНИЯ И ВЫТЯГИВАНИЯ
// 4. СРАВНИТЬ СКОРОСТИ ВТЯГИВАНИЯ И ВЫТЯГИВАНИЯ СО ЗНАЧЕНИЯМИ В ФАЙЛЕ left_leg.INO
// 5. ПРОВЕРИТЬ В ЦЕЛОМ РАБОТАЮТ ЛИ ВТЯГИВАНИЯ И ВЫТЯГИВАНИЯ. ПРОВЕРИТЬ ПОВОРОТЫ.
// 6. ПРОВЕРИТЬ ХОЖДЕНИЕ ПО СТУПЕНЬКАМ ВВЕРХ И ВНИЗ
// 7. ДОМА СДЕЛАТЬ ЗАПРОС И В ОТВЕТ ПОСЫЛАТЬ ФУНКЦИЮ ЗАГРУЗКИ ЗНАЧЕНИЙ В ПОЛЯ.




//#include <stdlib.h>
#include <stdio.h>
//#include <string.h>

#define _STEPPER_
#include "AccelStepper.h"
#include "UART.h"
#include "proc.h"


#include "privod.h"

#include "message.h"

#define _EPROM1_

#ifdef _EPROM1_

#include "Eeprom.h"
// #define _TEL_EEPROM_

#endif

#define _DISP_


#define _CHKDATA_         // проверка обмена с ногами
//#define _BUILTIN_LED_ON_  // отключение светодиода
//#ifdef ARDUINO_MAIN
#define STOP_ON_ERROR


#define ENDCAP    PB4 //PB7
#define ENDCAP_R  PB5 //PB8

#define StepPin   PB13
#define DirPin    PB12//PA4

#define RTS_L     PB14
#define RTS_R     PB15


#define CE_PIN    PB0// PC15 синий
#define CS_PIN    PA4 //PC14


#define SerL_RX_PIN   PA3
#define SerR_RX_PIN   PB11



// Высокий или низкий уровень для концевиков
#define EC_HIGH  LOW
#define EC_LOW   HIGH

#ifdef _BUILTIN_LED_ON_
//long ledPinTimer = 0;
//bool ledPinActive = 0;
#endif
#define _TIHOHOD_





//leedLeg leed_leg = noLeed;

//long mot.CurrentZero = 0;

//unsigned long  vtagSpeed = 5750;
//unsigned long vytagSpeed = 4500;

//unsigned long  vtagAccel = 6250;
//unsigned long vytagAccel = 8750;

unsigned long  rotSpeed = 3000;
//unsigned long  rotAccel = 6500;




// long mot.rbOrient  = 0;  // угол направления движения в градусах
//long oldOrient = 0;

//long stepAngle = angleToStep(10, 2); //1111;//555.5555556;//2222.2222;// 10 град, 2 микрошаг




const byte Motor_number = 1;

// LiquidCrystal_I2C_STM32 lcd = initLcd(0x3F, 16, 2);

bool LoopPlay = 0;

bool withPauses = 0;
bool bPause = 0;


// время отказа робота в случае неполучения ответной комманды от ноги.
const unsigned long return_time = 20000;


byte minRbtStep = 15;

long recordTimer = 0;
long StartRecordTime = 0;
//long playTimer = 0;

#ifdef _EPROM1_
const unsigned long StartEEPROM = 610;
unsigned long StrAddr = StartEEPROM;


#ifdef _TEL_EEPROM_
const unsigned long StartTelEEPROM = 510;
unsigned long StrTelAddr = StartTelEEPROM;
const byte TelIncriment = 30;
#endif
#endif

// Используется при записи как значения по умолчанию
char currSpeed[3] = "S2";
char currAccel[3] = "A0";
char currStpDpth[4] = "K29";
char currStpAngl[4] = "K22";
char currRbStps[4]  = "d65";
//String currMinStepRot = "m" + String(minRbtStep);
char cMinRbtStep[5];
char currMinStepRot[5] = "m";
//strcpy(currMinStepRot, cMinRbtStep);


unsigned long startTime = 0;
unsigned long endTime = 0;


enum cmdSelectRecordable
{
  goBackCmd = 13,
  goForwardCmd = 14,
  liftLeftCmd = 15,
  liftRightCmd = 16,
  telegaToCenterCmd = 18,

  walkAngle10Cmd = 20,
  walkAngle20Cmd = 21,
  walkAngle30Cmd = 22,
  walkAngle45Cmd = 23,
  walkAngle60Cmd = 24,
  walkAngle70Cmd = 25,
  walkAngle80Cmd = 26,
  walkAngle90Cmd = 27,

  liftLength20Cmd = 28,
  liftLength30Cmd = 29,
  liftLength50Cmd = 30,
  liftLength80Cmd = 31,
  liftLength120Cmd = 32,
  liftLength160Cmd = 33,

  fastModeCmd = 34,
  walkTypeCmd  = 35,

  liftLength180Cmd = 36,
  liftLength220Cmd = 37,
  liftLength360Cmd = 38,

  walkAngle180Cmd = 47,

  seedUpDnCmd = 40,

  standStillCmd = 45,
  
  shakeCmd = 60,
  afterShakeCmd = 61,
  
  rotRightCmd = 64,
  rotLefttCmd = 65,
  rotRightFfCmd = 66,
  rotLefttFfCmd = 67,
  rotRightBkCmd = 68,
  rotLefttBkCmd = 69,
  goUpstearsCmd = 70,
  goDnStearsCmd = 71
};


enum cmdSelect {

  telegaToLeftCmd = 17,
  telegaToRightCmd = 19,

  KalibrovkaCmd = 39,

  recCmd = 42,
  playCmd = 44,

  pauseCmd = 46,
  memoryPausesCmd = 54,

  dataToUploadCmd = 72

//  shakeHandCmd = 63,
 // downloadCmd = 50,
 // demo1Cmd = 51,
 // demo2Cmd = 52,
 // demo3Cmd = 53,
// liftUpDnRightCmd = 41,
 // stopCmd = 10,
//  turnLeftCmd = 11,
//  turnRightCmd = 12,
  
};

struct LegUpClass
{
  const byte s0  = 0;
  const byte s10 = 10;
  const byte s15 = 15;
  const byte s25 = 25;
  const byte s40 = 40;
  const byte s60 = 60;
  const byte s80 = 80;
  const byte s90 = 90;
  const byte s110 = 110;
  const byte s180 = 180;
} sAng_t;


telSpec TelSpec;



//byte nul_data = 3, one_data = 7, two_data = 11, three_data = 15;//, four_data = 19, six_data = 27;

const unsigned long Com1Rate = 115200L;

// системная пауза между коммандами.
const byte pauseForSerial = 15; // 15 ms

const char BbITANYTb_CH = 'i';
const char BTANYTb_CH = 'h';

robot_leg rLeg;
//robot_leg old_leg = rLeg;
//regimRaboty mode = fast;
leg_dir LegUpL = vtianut, LegUpR = vtianut;
//MKmotor uzel;
//dirflg dir_flg = middle;
//step_dir StpDir = forward;
StadyWork stWork = StWork;
actions Actions  = standStil;
actions oldActions = Actions;

unsigned short maxStepsCount = 0;

unsigned long parkingTimer;

//char **work_data;
//short fLength = 0;

//RF24 radio1(CE_PIN, CS_PIN);
//RF24& radio = radio1;
#ifdef _NRF24_
RF24 radio(CE_PIN, CS_PIN);
#endif

#ifdef _STEPPER_
AccelStepper* motorLink[Motor_number];
#endif


#include "math.h"


const int baudRateCom1 = 9600;

//#include "ladder.h"

void setup()
{
  bool bEeprom = 0, bDisplay = 0;
  posOfMotors mot;
#ifdef _BUILTIN_LED_ON_
  pinMode(LED_BUILTIN, OUTPUT);
#endif

#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, HIGH);
#endif
  delay(1000);
#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, LOW);
#endif

#ifdef _EPROM1_
  Wire.begin();
#endif
  delay(100);
#ifdef _DISP_
  lcd.begin();
  delay(100);
  lcd.backlight();
  lcd.print("Loading..");
#endif


  pinMode(ENDCAP, INPUT_PULLUP);
  delay(400);
  pinMode(ENDCAP_R, INPUT_PULLUP);
  delay(400); // +100
  //  pinMode(CS_PIN, OUTPUT);
  //  digitalWrite(CS_PIN, LOW);
#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, HIGH);
#endif
  delay(200); // +200
  motorLink[0] = new AccelStepper(AccelStepper::DRIVER, StepPin, DirPin);
  //  fOtladkaMes("AccelStepper"); delay(1000);
  delay(2000); // +3400
#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, LOW);
#endif

  // передача концевиков

  //  privod pr_telega(ENDCAP, ENDCAP_R, 9000,9000);


  pr_telega.privod_init(ENDCAP, ENDCAP_R);
//  pr_telega.
  pr_telega.setTelegaSpec(TelSpec);
  //  fOtladkaMes("Caps"); delay(1000);




  // по результатам испытаний немного подразогнали
  // скорость была 9200, ускорение 10000

  //readString(TelegaSpeedAddr).toInt();
  //  fOtladkaMes(telegaSpeed); delay(1000);
  //readString(TelegaAccelAddr).toInt();
  //  fOtladkaMes(telegaAccel); delay(1000);
  //  fOtladkaMes("Caps"); delay(1000);
  //  fOtladkaMes("Read_Ok"); delay(1000);


  //  pr_telega.setTelegaSpec(TelSpec);

  motorLink[0]->setMinPulseWidth(20);

  Serial1.begin(baudRateCom1);

  SerL.setComRate(Com1Rate, RTS_L);
  //  pinMode(SerL_RX_PIN, INPUT_PULLUP);
  delay(100);
  SerR.setComRate(Com1Rate, RTS_R);
  //  pinMode(SerR_RX_PIN, INPUT_PULLUP);
  delay(100);
#ifdef SERIAL_USB
  Serial2.begin(Com1Rate);
  delay(100);
  Serial3.begin(Com1Rate);
//  Serial1.begin(Com1Rate);
//  delay(100);
//  Serial2.begin(Com1Rate);
#endif


  Serial1.println("");
  Serial1.println("");
  Serial1.println("_-_-_-_-_-_-_-_-_-_-_-_");
  Serial1.println("-_-_-_-_-_-_-_-_-_-_-_-");
  delay(50);
  Serial1.println("");
  Serial1.println("Loading............");
  delay(10);

#ifdef _CHKDATA_
  // long waitTimer;
  delay(2000); // +6000
  // очистка буфера от возможного мусора
  while ((SerL.handle_serial() < 0) ||
         (SerR.handle_serial() < 0)) {};
  while (Serial1.available()) {
    Serial1.read();
    delay(1);
  };
  delay(10);

#endif
#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, HIGH);
#endif


  fCalibration(mot);
  fOtladkaMes("End_Cal"); delay(1000);


  i2cScan(bEeprom, bDisplay);
  delay(300);
  if (!bEeprom)  fErrorMes("No EEPROM Available");
  if (!bDisplay) fErrorMes("No Display Available");
  fOtladkaMes("ScanDone"); delay(1000);

  
 // После проверки и2ц, можно вернуть скорости измененные  при калибровке в обычное состояние
  delay(10);
  /*
//  unsigned short vtagSpeed  = 4500;
  // убрал параметр, так как без него скорость установится из еепром
  mot.Send_BTAHYTb_Speed();
  delay(pauseForSerial);
  // после калибровки установить режим с поднятыми ногами

  // скорости и ускорения стоп
  fOtladkaMes("SetRotSpeed");
  // аналогично предыдущему комменту
  mot.Send_Rot_Speed(4000);
  fOtladkaMes("SetRotAccel");
  mot.Send_Rot_Accel(3000);

  char buff[100];
  readString(TelegaSpeedAddr, buff);
  TelSpec.speed = atoi(buff);
  buff[0] = '\0';
  pr_telega.setTelegaSpec(TelSpec);
  delay(pauseForSerial);

  readString(TelegaAccelAddr, buff);
  TelSpec.accel = atoi(buff);
  pr_telega.setTelegaSpec(TelSpec);
  delay(pauseForSerial);
*/
  EndOfCalibration(mot);
// Конец калибровки
 
  fErrorMes("Starting-working"); 
 
  /*
    #ifdef SERIAL_USB
    Serial1.print("Starting working\r\n");
    #else
    Serial.print("Starting working\r\n");
    #endif
  */


  //   ————————LOOP—————————
  while (1)
  {
    //  byte sizeOfRFdata = 60;
    //  char buff[sizeOfRFdata - 1];
    bool RadioAval = 0;
    char RF_data[60] = {'~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '\0'};
    byte serialAmount = 0;
    char tmp;
#ifdef _BUILTIN_LED_ON_
    /*
        if (((millis() - ledPinTimer) > 500) && ledPinActive)
        {
          ledPinActive = 0;
          digitalWrite(LED_BUILTIN, HIGH);
        } // через заданное время отключаем зеленый светодиод
    */
#endif

#ifdef _NRF24_
    if (radio.available())
#else
    if (Serial1.available())
#endif
    {
#ifdef _BUILTIN_LED_ON_
      digitalWrite(LED_BUILTIN, LOW);
      ledPinActive = 1;
      ledPinTimer = millis();
#endif
      RadioAval = 1;

#ifdef _NRF24_
      radio.read(&RF_data, sizeof(RF_data));
#else

      // читам пока не наткнемся на конец строки или условие while
      while (Serial1.available() && (serialAmount < 30))
      {
        RF_data[serialAmount] = Serial1.read();
        if (RF_data[serialAmount] == '\n') break;
        serialAmount++;
        delayMicroseconds((10000000 / baudRateCom1) + 500);  //23.7.20 было +50мкс

      }
      RF_data[serialAmount] = '\0';

      while (Serial1.available())
      {

        if (Serial1.read() == '\r') break;
        delay(2);
        // фильтр сообщений с ошибкой
        RadioAval = 0;
        fErrorMes("MisData");
      }
      if (serialAmount < 2) {
        RadioAval = 0;
        fOtladkaMes("Amount<2");
      }
      if (SerL.handle_serial() < 0) // чего-то принято от левой?
      {
        SerL.getString0(tmp); // смотрим чего пришло
        fErrorMes("LeftLegShum");
      }

      if (SerR.handle_serial() < 0) // чего-то принято от левой?
      {
        SerR.getString0(tmp); // смотрим чего пришло
        fErrorMes("RightLegShum");
      }
#endif

      if ((RF_data[0] == 'K') && (RF_data[1] == '4') && (RF_data[2] == '3'))
      {
        if (stWork == StRec)
        {
          fErrorMes("ErrorPlayInRecMode");
          RadioAval = 0;
        }
        else
        {
          stWork = StPlay; //3.3.2020
          //      fStartPlay(LoopPlay);
          Serial.println("PlayIsOn");
          PlayFromEEPROM(LoopPlay, mot);
          stWork = stWork == StWork ? StPlay : StWork; //10.7.2020
          RadioAval = 0;
          //     ledPinActive = 0;
        }
      }
      //   delay(100);
    }

    if (RadioAval == 1)
    {
      fOtladkaMes(">>");
      RF_messege_handle(RF_data, mot);
    } // radio.available
    if (stWork != StPlay)
      while ((SerL.handle_serial() < 0) || (SerR.handle_serial() < 0)) {};
    //  StepsManage();
  }

}  /*-------------------// END SETUP //---------------------*/

// --ГЛАВНЫЙ ЦИКЛ ПРОГРАММЫ!!!--
void loop() {} // ------- LOOP --------



byte RF_messege_handle(char *RF_data, posOfMotors & mot)
{
  static regimRaboty mode = energySaving;

  static long stepAngle = angleToStep(30); 

  static short UgolPovorotaL = -30;
  static short UgolPovorotaR = 30;
 
//   for(auto &bin:RF_data)
//   {
//	auto bin = 6;
//   }


  char step_height = 85; // высота ступеньки 170
  long zapas_na_povorot = 14;
  char steps_cnt = 3;    // количество ступенек

  const char cmdR = RF_data[0];

  byte D_Len = strlen(RF_data);
  if (D_Len >= 30)
  {
    fOtladkaMes("ShouldBeMesError ", D_Len);
    return 0;
  }
//  char cpyData[D_Len + 1];
//  for (int f = 0; f < (D_Len - 1) ; f++)
//  {
//    cpyData[f] = RF_data[f + 1];
//  }

  for (int f = 0; f < D_Len ; f++)
  {
    RF_data[f] = RF_data[f+1];
  }

  RF_data[D_Len - 1] = 0;
  //  fOtladkaMes(RF_data);

//  short x = atoi(&RF_data[0]);
  short x = atoi(RF_data);
  unsigned long  x_ul = atol(RF_data);






  bool SaveTimeToPlay = false;



// обработка комманд без привязки времени их начала

  switch (cmdR)
  {

    case 'K':
	  switch (x)
      {
	    case telegaToRightCmd: // telega vpravo
          telega_right(mot);
          break;

        case telegaToLeftCmd: // telega vlevo
          telega_left(mot);
          break;
		  
        case KalibrovkaCmd: // калибровка
		  moveMassLeft(mot);
          fCalibration(mot);
		  EndOfCalibration(mot);
          break;

        case recCmd: //запись
          if (stWork == StPlay)
          {
            fErrorMes("ErrorRecInPlayMode");
            return general_error;
          }
          stWork = (stWork == StWork) ? StRec : StWork;
          // записать в память скорости и ускорения
#ifdef _EPROM1_
          if (stWork != StRec)
          {
            CheckUpMes("RecIsOff");

            recordTimer = 0;
			mode = energySaving;
          }
          if (stWork == StRec)
          {
            CheckUpMes("RecIsOn");
            StartRecordTime = millis();
            //           fOtladkaMes("StrAdr1 = " + String(StrAddr));
            itoa(minRbtStep, cMinRbtStep);
            strcat(currMinStepRot, cMinRbtStep);
            StrAddr = StartEEPROM;
            writeString(currSpeed);
            writeString(currAccel);
///			delay(100);
            writeString(currStpDpth);
//			delay(100);
            writeString(currStpAngl);
            delay(100);
            // convert data to char*
            //			strcpy(currMinStepRot, cMinRbtStep);
            //            writeString(currMinStepRot);
            // add converted data to result to write
            writeString(currMinStepRot);
          }
          else
          {
            //	    char tmpSt[10];
            //	    itoa(StrAddr, tmpSt, DEC);
            writeStringA(StrAddr , 0);
          }
#endif
          break;
        //      case 43: //воспроизведение

        case playCmd:
          //         LoopPlay == 0 ? LoopPlay = 1 : LoopPlay = 0;
          if (LoopPlay == 0)
          {
            fOtladkaMes("LoopPlayOn");
            LoopPlay = 1;
          }
          else
          {
            fOtladkaMes("LoopPlayOff");
            LoopPlay = 0;
          }
          break;


        case pauseCmd:
          //pause
          //одиночная пауза
          //wait вызывается  всегда, пауза просто вставляется в него
          bPause ? bPause = 0 : bPause = 1;
          fAddInActionInRecordMode(aPause);
          break;

        case memoryPausesCmd: // режим записи с запоминанием времени
          withPauses == 1 ? withPauses = 0 : withPauses = 1;
          if (withPauses == 1) CheckUpMes("withPauses");
          else CheckUpMes("nooPauses");
          break;


        case dataToUploadCmd:
          dataToUpload();
          break;
		  
		default:
			SaveTimeToPlay = true;
	  }
	  break;
	  
	  
    // s, L, R, S, A, Y, W, d, m, D, J, H,
    // q , h, p, o, P, C

    case 'q':
      // скорость вверх
      //записать в еепром
      mot.Set_BTAHYTb_Speed(x);
      // прочесть скорость из епром и отправить ногам
      mot.Send_BTAHYTb_Speed();
      // отправить на телефон принятую комманду
      CheckUpMes("TT", x);
      break;
    case 'h':
      // скорость вниз
      mot.Set_BbITAHYTb_Speed(x);
      // прочесть скорость с епром и отправить ногам
      mot.Send_BbITAHYTb_Speed();
      // отправить на телефон принятую комманду
      CheckUpMes("TT", x);
      break;

    case 'p':
      // ускорение вверх
      mot.Set_BTAHYTb_Accel(x);
      // прочесть ускорение с еепром и отправить ногам
      mot.Send_BTAHYTb_Accel();
      // отправить на телефон принятую команду
      CheckUpMes("TT", x);
      break;
    case 'o':
      // ускорение вниз
      mot.Set_BbITAHYTb_Accel(x);
      // прочесть ускорение с еепром и отпрвить ногам
      mot.Send_BbITAHYTb_Accel();
      CheckUpMes("TT", x);
      break;


    case 's':
      // Если не воспроизведение тогда выходим
      if (stWork != StPlay) break;
      // сохраняем параметр х
      if (x < 0) break;
      maxStepsCount = x;
      fOtladkaMes("maxStepsCount=", maxStepsCount);
      break;
	  

    case 'Y': // Скорости и ускорения стопы
      fOtladkaMes("SpeedFoots=", x);
      mot.Set_Rot_Speed(x);
      rotSpeed = x;
      CheckUpMes("TT", x);
      break;

    case 'W':
      fOtladkaMes("AccelFoots=", x);
      mot.Set_Rot_Accel(x);
      CheckUpMes("TT", x);
      break;

    case 'd':
      strcpy(currRbStps, "d");
      // convert x to char* cTmp;
      char cTmp[15];
      itoa(x, cTmp);
      // add cTmp to tail
      strcat(currRbStps, cTmp);
      pr_telega.SetMaxDist(x);
      break;

    case 'D': //
      if (x_ul <= 0) break;
      do {} while (RecordingTime() < x_ul);
      break;
	  
    case 'P':
	  delay(x_ul);
	  break;
	  
    case 'J': // скорость телеги
      TelSpec.speed = x;
      pr_telega.setTelegaSpec(TelSpec);
      //	  char tmp3[15];
      //	  itoa(x, tmp3,DEC);
      writeStringA(x, TelegaSpeedAddr);
      CheckUpMes("TT", x);
      break;

    case 'H': // ускорение телеги
      TelSpec.accel = x;
      pr_telega.setTelegaSpec(TelSpec);
      //	  char tmp4[15];
      //	  itoa(x, tmp4,DEC);
      writeStringA(x, TelegaAccelAddr);
      CheckUpMes("TT", x);
      break;
	  
	case 'C':
	  mot.OrientInc(x);
	  break;
    default:
//      goto notToSaveTimeToPlay;
	  SaveTimeToPlay = true;
//      fErrorMes("unknownCmd:", cmdR);
//      fErrorMes(RF_data);
      //	fErrorMes(String(buff[0]));
      //	fErrorMes("\r\n");
      break;

  }




  
  if(!SaveTimeToPlay) return 0;

  recordTimer = RecordingTime();

  switch (cmdR)
  {
    case 'K':
      //  auto choise = static_cast<cmdSelect>(x);
      switch (x)
      {
 //       case stopCmd: // stop
 //         fOtladkaMes("Stop");
 //         break;

        case goBackCmd:
          fOtladkaMes("backward");
          walkBackward(stepAngle, mode, mot);
          break;

        case goForwardCmd: //forward шагаем
          fOtladkaMes("forward");
          walkForward(stepAngle, mode, mot);
          break;

        case telegaToCenterCmd: // telega v centr
          telega_to_center(mot,1);
          break;

        case liftLeftCmd:  // выдвинуть левую (если смотреть спереди робота против его хода)
          topLeft(mot);
          break;

        case liftRightCmd:  // выдвинуть правую (если смотреть спереди робота против его хода)
          topRight(mot);
          break;

        case walkAngle10Cmd:
          fOtladkaMes("10 Degree");
//          currStpAngl[1] = '2'; currStpAngl[2] = '0';
//          stepAngle = angleToStep(10); //4444L; // 10 град
		  stepAngle = setWalkAngleCmd((char*)"20", 10);
          fAddInActionInRecordMode(Deg10);
          break;

        case walkAngle20Cmd:
          fOtladkaMes("20 Degree");
//          currStpAngl[1] = '2'; currStpAngl[2] = '1';
		  stepAngle = setWalkAngleCmd((char*)"21", 20);
          fAddInActionInRecordMode(Deg20);
          break;

        case walkAngle30Cmd:
          fOtladkaMes("30 Degree");
//          currStpAngl[1] = '2'; currStpAngl[2] = '2';
		  stepAngle = setWalkAngleCmd((char*)"22", 30);
          fAddInActionInRecordMode(Deg30);
          break;

        case walkAngle45Cmd:
          fOtladkaMes("45 Degree");
//          currStpAngl[1] = '2'; currStpAngl[2] = '3';
		  stepAngle = setWalkAngleCmd((char*)"23", 45);
          fAddInActionInRecordMode(Deg45);
          break;

        case walkAngle60Cmd:
          fOtladkaMes("60 Degree");
//          currStpAngl[1] = '2'; currStpAngl[2] = '4';
		  stepAngle = setWalkAngleCmd((char*)"24", 60);
          fAddInActionInRecordMode(Deg60);
          break;

        case walkAngle70Cmd:
          fOtladkaMes("70 Degree");
//          currStpAngl[1] = '2'; currStpAngl[2] = '5';
		  stepAngle = setWalkAngleCmd((char*)"25", 70);
          fAddInActionInRecordMode(Deg70);
          break;

        case walkAngle80Cmd:
          fOtladkaMes("80 Degree");
//          currStpAngl[1] = '2'; currStpAngl[2] = '6';
		  stepAngle = setWalkAngleCmd((char*)"26", 80);
          fAddInActionInRecordMode(Deg80);
          break;

        case walkAngle90Cmd:
          fOtladkaMes("90 Degree");
//          currStpAngl[1] = '2'; currStpAngl[2] = '7';
		  stepAngle = setWalkAngleCmd((char*)"27", 90);
          fAddInActionInRecordMode(Deg90);
          break;

        case walkAngle180Cmd:
          fOtladkaMes("180 Degree");
 //         currStpAngl[1] = '4'; currStpAngl[2] = '7';
		  stepAngle = setWalkAngleCmd((char*)"47", 180);
          fAddInActionInRecordMode(Deg180);
          break;

        case liftLength20Cmd: // 6000
          fOtladkaMes("20 mm");
//          currStpDpth[0] = 'K'; currStpDpth[1] = '2'; currStpDpth[2] = '8';
          composeNumCmdCode(currStpDpth, (char*)"28");
          mot.SetStepsDepthInSteps(stepDephCalc(sAng_t.s10));
          fAddInActionInRecordMode(H20);
          break;

        case liftLength30Cmd: // 12000
          fOtladkaMes("30 mm");
//          currStpDpth[0] = 'K'; currStpDpth[1] = '2'; currStpDpth[2] = '9';
          composeNumCmdCode(currStpDpth, (char*)"29");
          mot.SetStepsDepthInSteps(stepDephCalc(sAng_t.s15));
          fAddInActionInRecordMode(H30);
          break;

        case liftLength50Cmd: // 18000
          fOtladkaMes("50 mm");
//          currStpDpth[0] = 'K'; currStpDpth[1] = '3'; currStpDpth[2] = '0';
          composeNumCmdCode(currStpDpth, (char*)"30");
          mot.SetStepsDepthInSteps(stepDephCalc(sAng_t.s25));
          fAddInActionInRecordMode(H50);
          break;

        case liftLength80Cmd: // 24000
          fOtladkaMes("80 mm");
//          currStpDpth[0] = 'K'; currStpDpth[1] = '3'; currStpDpth[2] = '1';
          composeNumCmdCode(currStpDpth, (char*)"31");
          mot.SetStepsDepthInSteps(stepDephCalc(sAng_t.s40));
          fAddInActionInRecordMode(H80);
          break;

        case liftLength120Cmd:
          fOtladkaMes("120 mm");
//          currStpDpth[0] = 'K'; currStpDpth[1] = '3'; currStpDpth[2] = '2';
          composeNumCmdCode(currStpDpth, (char*)"32");
          mot.SetStepsDepthInSteps(stepDephCalc(sAng_t.s60));
          fAddInActionInRecordMode(H120);
          break;

        case liftLength160Cmd: //36000
          fOtladkaMes("160 mm");
//          currStpDpth[0] = 'K'; currStpDpth[1] = '3'; currStpDpth[2] = '3';
          composeNumCmdCode(currStpDpth, (char*)"33");
          mot.SetStepsDepthInSteps(stepDephCalc(sAng_t.s80));
          fAddInActionInRecordMode(H160);
          break;

        case liftLength180Cmd:
          fOtladkaMes("180 mm");
//          currStpDpth[0] = 'K'; currStpDpth[1] = '3'; currStpDpth[2] = '6';
          composeNumCmdCode(currStpDpth, (char*)"36");
          mot.SetStepsDepthInSteps(stepDephCalc(sAng_t.s90));
          fAddInActionInRecordMode(H180);
          break;

        case liftLength220Cmd:
          fOtladkaMes("220 mm");
//          currStpDpth[0] = 'K'; currStpDpth[1] = '3'; currStpDpth[2] = '7';
          composeNumCmdCode(currStpDpth, (char*)"37");
          mot.SetStepsDepthInSteps(stepDephCalc(sAng_t.s110));
          fAddInActionInRecordMode(H220);
          break;

        case liftLength360Cmd:
          fOtladkaMes("360 mm");
//          currStpDpth[0] = 'K'; currStpDpth[1] = '3'; currStpDpth[2] = '8';
          composeNumCmdCode(currStpDpth, (char*)"38");
          mot.SetStepsDepthInSteps(stepDephCalc(sAng_t.s180));
          fAddInActionInRecordMode(H360);
          break;

        case fastModeCmd:
          if (stWork == StPlay)
          {
            change_mode(mode, 1); // energySaving;
          }
          break;

        case walkTypeCmd:// Режим хотьбы, быстрый или медленный
            change_mode(mode, 0);// change;
          break;

 //       case turnLeftCmd: // поворот влево
          //          change_orient(leftA, mot,minRbtStep);

 //         break;

 //       case turnRightCmd: // поворот вправо (это назад идти)
          //          change_orient(rightA, mot,minRbtStep);

//          break;

        // меняется текущее положение. Если сидел, то привстаем, или наоборот
        case seedUpDnCmd:
 //         fAddInActionInRecordMode(wait);

          seetUpDown(mot);
          fAddInActionInRecordMode(stDn);
          break;
/*
        case liftUpDnRightCmd: //   вытягиваем и втягиваем правую чтобы разработать
          while (1)
          {
            if (fBreak(right_leg, foot, mot)) break;
            mot.MoveRightLegCurrentSteps(- stepsPerLegRot * fullRotationLimit, BTANYTb_CH);
            //       SerR.prepareMessage( 'h', mot.RightLegCurrentSteps); // вытягиваем
            if (fAnswerWait(right_leg, knee, mot)) break;

            mot.MoveRightLegCurrentSteps(0L, BbITANYTb_CH);
            //            SerR.prepareMessage( 'i', mot.RightLegCurrentSteps); // втягиваем
            if (fAnswerWait(right_leg, knee, mot)) break;
          }
          break;
*/

        case standStillCmd:
          fOtladkaMes("StandStill");
 //         fAddInActionInRecordMode(wait);
          StepsManage(mot, mode);
          fAddInActionInRecordMode(standStil);
          break;

 //       case downloadCmd: // обработка команды загрузки данных из ПК
          //       fDload(); // функция загрузки
  //        break;

 //       case demo1Cmd:
 //         demo1(mot);
 //         break;

 //       case demo2Cmd:
 //         demo2(mot);
 //         break;

 //       case demo3Cmd:
 //         demo3(mot);
 //         break;
        // качаем один раз
        case shakeCmd:
          fShake(mot, mode, 0);
          fAddInActionInRecordMode(shakeOn);
          break;

        case afterShakeCmd: // переход в исходное после качания
          fShake(mot, mode, 1);
          fAddInActionInRecordMode(shakeOff);
          break;

        case 62:
          //        fRoll(mot);
          break;

 //       case shakeHandCmd:
//          fShakeHandWithRotation(mot, 360);
//          fAddInActionInRecordMode(shakeRot);
//          break;

        case rotRightCmd:
          // поворот вправо
          fOtladkaMes("UgolPovorotaR=", UgolPovorotaR);
          rotPlace1(mot, minRbtStep, UgolPovorotaR, mode);
          // При смене ориентации не стоит отправлять об этом сообщение в ПК,
          // т.к. подтверждение этой операции не требуется
          fAddInActionInRecordMode(turnR);
          break;

        case rotLefttCmd:
          // поворот влево
          fOtladkaMes("UgolPovorotaL=", UgolPovorotaL);
          rotPlace1(mot, minRbtStep, UgolPovorotaL, mode);
          // При смене ориентации не стоит отправлять об этом сообщение в ПК,
          // т.к. подтверждение этой операции не требуется
          fAddInActionInRecordMode(turnL);
          break;

        case rotRightFfCmd:
          // повернуть вправо вперед
          fOtladkaMes("UgolPovorotaR=", UgolPovorotaR);
          rotateRightFF(UgolPovorotaR, mode, mot);
          fAddInActionInRecordMode(turnRfst);
          break;

        case rotLefttFfCmd:
          // повернуть влево вперед
          fOtladkaMes("UgolPovorotaL=", UgolPovorotaL);
          rotateLeftFF(UgolPovorotaL, mode, mot);
          fAddInActionInRecordMode(turnLfst);
          break;

        case rotRightBkCmd:
          // повернуть вправо назад
          fOtladkaMes("UgolPovorotaR=", UgolPovorotaR);
          rotateRightBK(UgolPovorotaR, mode,  mot);
          fAddInActionInRecordMode(turnRbst);
          break;

        case rotLefttBkCmd:
          // повернуть влево назад
          fOtladkaMes("UgolPovorotaL=", UgolPovorotaL);
          rotateLeftBK(UgolPovorotaL, mode, mot);
          fAddInActionInRecordMode(turnLbst);
          break;

        case goUpstearsCmd:  //70
          // проверить работу порта можно вставив deley
   //       fAddInActionInRecordMode(wait);
          goUpstears(mot);
          fAddInActionInRecordMode(walkUpSt);
          break;

        case goDnStearsCmd:  //71
  //        fAddInActionInRecordMode(wait);
          goDnstears(mot, forward, step_height, zapas_na_povorot, steps_cnt);
          fAddInActionInRecordMode(walkDnSt);
          break;
		  
//        case 73:
		  
//          break;
      }
      break;

    // s, L, R, S, A, Y, W, d, m, D, J, H,
    // q , h, p, o, P, C
    case 'R': // влево с дошаганием
      UgolPovorotaR = x;
      fAddInActionInRecordMode(UgolR, x);
      break;
    case 'L': // вправо с дошаганием
      UgolPovorotaL = -x;
      fAddInActionInRecordMode(UgolL, x);
      break;
    case 'S': // задаем скорость для колен
      fOtladkaMes("SpeedLegs=", x);
      //	    currSpeed[0] = 'S';
      if (x == 0)
      {
        unsigned short  vtagSpeed  = 5750;
        unsigned short vytagSpeed = 4500;
        currSpeed[1] = '0';
        mot.Send_BTAHYTb_Speed(vtagSpeed);
        mot.Send_BbITAHYTb_Speed(vytagSpeed);
        //     fOtladkaMes("0");
      }

      else if (x == 1)
      {
        unsigned short vtagSpeed  = 5000;
        unsigned short vytagSpeed = 3750;
        currSpeed[1] = '1';
        mot.Send_BTAHYTb_Speed(vtagSpeed);
        mot.Send_BbITAHYTb_Speed(vytagSpeed);
      }

      else if (x == 2)
      {
        // х = 2 стандартные значения
        //      vtagSpeed  = 4500;
        //      vytagSpeed = 3500;
        mot.Send_BTAHYTb_Speed();
        mot.Send_BbITAHYTb_Speed();
        currSpeed[1] = '2';

      }

      else if (x == 3)
      {
        unsigned short vtagSpeed  = 4000;
        unsigned short vytagSpeed = 3000;
        currSpeed[1] = '3';
        mot.Send_BTAHYTb_Speed(vtagSpeed);
        mot.Send_BbITAHYTb_Speed(vytagSpeed);
      }
      fAddInActionInRecordMode(legSpeed, x);
      break;

    case 'A': // задаем ускорение для колен
      fOtladkaMes("AccelLegs=", x);
      if (x == 0)
      {
        //      vtagAccel = 6250;
        //      vytagAccel = 8750;
        mot.Send_BTAHYTb_Accel();
        mot.Send_BbITAHYTb_Accel();
        currAccel[1] = '0';
      }
      else if (x == 1)
      {
        unsigned short vtagAccel = 8750;
        unsigned short vytagAccel = 11250;
        mot.Send_BTAHYTb_Accel(vtagAccel);
        mot.Send_BbITAHYTb_Accel(vytagAccel);
        currAccel[1] = '1';
      }
      else if (x == 2)
      {
        unsigned short vtagAccel = 5000;
        unsigned short vytagAccel = 5000;
        mot.Send_BTAHYTb_Accel(vtagAccel);
        mot.Send_BbITAHYTb_Accel(vytagAccel);
        currAccel[1] = '2';
      }
      else if (x == 3)
      {
        unsigned short vtagAccel = 15000;
        unsigned short vytagAccel = 11250;
        mot.Send_BTAHYTb_Accel(vtagAccel);
        mot.Send_BbITAHYTb_Accel(vytagAccel);
        currAccel[1] = '3';
      }

      delay(pauseForSerial);
      fAddInActionInRecordMode(legAccel, x);

      break;
    case 'm': // маленький шажок робота
      minRbtStep = x;
      strcpy(currMinStepRot, "m");
      itoa(minRbtStep, cMinRbtStep);
      strcat(currMinStepRot, cMinRbtStep);
      //    currMinStepRot = "m" + String(x);
      fOtladkaMes("MinStp=", x);
      fAddInActionInRecordMode(DoStep, x);
      break;

    default:
      fErrorMes("unknownCmd:", cmdR);
      fErrorMes(RF_data);
      //	fErrorMes(String(buff[0]));
      //	fErrorMes("\r\n");
      break;
  }

  fOtladkaMes("<<");
#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, HIGH);
#endif
  return 0;
}








byte walkForward(long stepAngle, const regimRaboty &mode, posOfMotors & mot)
{
  unsigned long halfLengthStepsCount = 0;
  Actions = walkFf;
  pr_telega.fDoezd(*motorLink);
  if (readyForRotRightFoot(mot) ) // стоим на правой ноге?
  {
    oldActions = walkFf;
    while (1)
    {
      // поворачиваемся вперед на правой ноге на нужный угол
      if (orient_steps(stepAngle, right_leg, forward, mot) && (stWork == StWork)) return general_error;

      // опускаем левую ногу (будем стоять на обоих)
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;
      if (fStepsCounter(mot, halfLengthStepsCount)) return general_error;
      // перевозим тележку влево
      if (moveMassLeft(mot)) return general_error;
      // если стоп в обычном режиме работы, то остановится
      if ((stWork == StWork) && fBreak(not_leg, knee, mot)) return general_error;
      // поднимаем(втягиваем) правую ногу.
      if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
      // поворачиваемся на левой ноге
      if (orient_steps(stepAngle, left_leg, forward, mot) && (stWork == StWork)) return general_error;

      // опускаем правую ногу
      if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;
      if (fStepsCounter(mot, halfLengthStepsCount)) return general_error;
      // перевозим тележку вправо
      if (moveMassRight(mot)) return general_error; // ехать вправо
      if ((stWork == StWork) && fBreak(not_leg, knee, mot)) return general_error;
      // поднимаем левую ногу
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
      // слушаем ББ на случай команды стоп выходим из цикла
      //                if (fBreak(left_leg, knee)) break;
    }
  }// подумать о погрешности между обеими ногами и как ее убрать.

  else if (fstandStill(mot)) // стоим на обеих ногах?
  { // если левая впереди, то телега справа и надо идти вперед, то много лишних действий.
    // если левая сзади и надо идти назад == много лишних действий.
    // для того чтобы небыло лишних движений при смене направления движения на противоположное

    if (oldActions == walkBk)
    {
      oldActions = walkFf;
    }

    // левая нога впереди? (смотрим вправо?)
    if (fEastOrWestStandingCalc(mot) == 1)
    {
      //	  Serial1.println("Smotrim_vpravo?");
      //	  delay(30);
      if (moveMassRight(mot)) return general_error;
      if ((stWork == StWork) && fBreak(not_leg, knee, mot)) return general_error;
      // поднимаем левую ногу
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
      // слушаем ББ на случай команды стоп выходим из цикла
      //                if (fBreak(left_leg, knee)) break;
      // поворачиваемся вперед на правой ноге на нужный угол
      if (orient_steps(stepAngle, right_leg, forward, mot) && (stWork == StWork)) return general_error;
      // опускаем левую ногу            (будем стоять на обоих)
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;
      if (fStepsCounter(mot, halfLengthStepsCount)) return general_error;
    }


    while (1)
    {
      if (moveMassLeft(mot)) return general_error; // ехать влево
      if ((stWork == StWork) && fBreak(not_leg, knee, mot)) return general_error;
      // поднимаем(втягиваем) правую ногу.
      if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
      //                if (fBreak(right_leg, knee)) break;
      // поворачиваемся на левой ноге
      if (orient_steps(stepAngle, left_leg, forward, mot) && (stWork == StWork)) return general_error;
      halfLengthStepsCount++;
      // опускаем правую ногу             (выставляем в 0)
      if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;
      //                if (fBreak(right_leg, knee)) break;
      if (fStepsCounter(mot, halfLengthStepsCount)) return general_error;
      // перевозим тележку вправо
      if ((stWork == StWork) && fBreak(not_leg, knee, mot)) return general_error;
      if (moveMassRight(mot)) return general_error; // ехать вправо
      // поднимаем левую ногу
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
      // слушаем ББ на случай команды стоп выходим из цикла
      //                if (fBreak(left_leg, knee)) break;
      // поворачиваемся вперед на правой ноге на нужный угол
      if (orient_steps(stepAngle, right_leg, forward, mot) && (stWork == StWork)) return general_error;
      // опускаем левую ногу            (будем стоять на обоих)
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;
      if (fStepsCounter(mot, halfLengthStepsCount)) return general_error;
      //      if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return general_error;
      //                if (fBreak(left_leg, knee)) break;
    }
  }
  //              стоим на левой ноге
  else if (readyForRotLeftFoot(mot))
  {
    oldActions = walkFf;
    while (1)
    {
      // поворачиваемся на левой ноге
      if (orient_steps(stepAngle, left_leg, forward, mot) && (stWork == StWork)) return general_error;
      // опускаем правую ногу
      if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;
      //                if (fBreak(right_leg, knee)) break;
      if (fStepsCounter(mot, halfLengthStepsCount)) return general_error;
      // перевозим тележку вправо
      if (moveMassRight(mot)) return general_error; // ехать вправо
      if ((stWork == StWork) && fBreak(not_leg, knee, mot)) return general_error;
      // поднимаем левую ногу
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
      // слушаем ББ на случай команды стоп выходим из цикла
      //                if (fBreak(left_leg, knee)) break;
      // поворачиваемся вперед на правой ноге на нужный угол
      if (orient_steps(stepAngle, right_leg, forward, mot) && (stWork == StWork)) return general_error;

      // опускаем левую ногу (будем стоять на обоих)
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;
      //                if (fBreak(left_leg, knee)) break;
      if (fStepsCounter(mot, halfLengthStepsCount)) return general_error;
      // перевозим тележку влево
      if (moveMassLeft(mot)) return general_error; // ехать влево
      if (fBreak(not_leg, knee,  mot) && (stWork == StWork)) return general_error;
      // поднимаем(втягиваем) правую ногу.
      if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
      //                if (fBreak(right_leg, knee)) break;
    }
  }
  return 0;
}



byte walkBackward(long stepAngle, const regimRaboty &mode, posOfMotors & mot)
{
 // fAddInActionInRecordMode(wait);
  Actions = walkBk;
  unsigned long halfLengthStepsCount = 0;
  pr_telega.fDoezd(*motorLink);
  if (readyForRotRightFoot(mot) ) // стоим на правой ноге?
  {
    oldActions = walkBk;
    while (1)
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    { //!!! добавить условие записи шагов чтобы break не останавливал цикл шагания в затруднительных местах !!!
      //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      // поворачиваемся назад на правой ноге на нужный угол
      if (orient_steps(stepAngle, right_leg, backward, mot) && (stWork == StWork)) return general_error;
      // опускаем левую ногу (будем стоять на обоих)
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;

      if (fStepsCounter(mot, halfLengthStepsCount)) return general_error;
      // перевозим тележку влево
      if (moveMassLeft(mot)) return general_error; // ехать влево
      if ((stWork == StWork) && fBreak(not_leg, knee, mot)) return general_error;
      // поднимаем(втягиваем) правую ногу.
      if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
      //                if (fBreak(left_leg, foot)) break;
      // поворачиваемся на левой ноге
      if (orient_steps(stepAngle, left_leg, backward, mot) && (stWork == StWork)) return general_error;
      // опускаем правую ногу
      if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;

      if (fStepsCounter(mot, halfLengthStepsCount)) return general_error;
      //                if (fBreak(right_leg, knee)) break;
      // перевозим тележку вправо
      if (moveMassRight(mot)) return general_error; // ехать вправо
      if ((stWork == StWork) && fBreak(not_leg, knee, mot)) return general_error;
      // поднимаем левую ногу
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
    }
  }// подумать о погрешности между обеими ногами и как ее убрать.
  else if (fstandStill(mot)) // стоим на обеих ногах?
  {
    // для того чтобы небыло лишних движений при смене направления движения на противоположное

    if (oldActions == walkFf) // шел вперед?
    {
      oldActions = walkBk;
    }
    //      if (old_leg == left_leg) // последнее опускание левой ноги на опору?
    // левая нога впереди? (смотрим вправо?)
    if (fEastOrWestStandingCalc(mot) == 0)
    {
      if (moveMassRight(mot)) return general_error; // ехать вправо
      if ((stWork == StWork) && fBreak(not_leg, knee, mot)) return general_error;
      // поднимаем левую ногу
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
      // слушаем ББ на случай команды стоп выходим из цикла
      //                if (fBreak(left_leg, knee)) break;
      // поворачиваемся вперед на правой ноге на нужный угол
      if (orient_steps(stepAngle, right_leg, backward, mot) && (stWork == StWork)) return general_error;
      // опускаем левую ногу            (будем стоять на обоих)
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;

      if (fStepsCounter(mot, halfLengthStepsCount)) return general_error;
    }

    while (1)
    {
      if (moveMassLeft(mot)) return general_error; // ехать влево
      if ((stWork == StWork) && fBreak(not_leg, knee, mot)) return general_error;
      // поднимаем(втягиваем) правую ногу.
      if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
      // поворачиваемся на левой ноге
      if (orient_steps(stepAngle, left_leg, backward, mot) && (stWork == StWork)) return general_error;
      // опускаем правую ногу
      if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;
      if (fStepsCounter(mot, halfLengthStepsCount)) return general_error;
      //               if (fBreak(right_leg, knee)) break;
      // перевозим тележку вправо
      //      if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return general_error;
      if (moveMassRight(mot)) return general_error; // ехать вправо
      if ((stWork == StWork) && fBreak(not_leg, knee, mot)) return general_error;
      // поднимаем левую ногу
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
      // слушаем ББ на случай команды стоп выходим из цикла
      //           if (fBreak(left_leg, knee)) break;
      // поворачиваемся вперед на правой ноге на нужный угол
      if (orient_steps(stepAngle, right_leg, backward, mot) && (stWork == StWork)) return general_error;
      // опускаем левую ногу (будем стоять на обоих)
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;
      if (fStepsCounter(mot, halfLengthStepsCount)) return general_error;
      //     if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return general_error;
    }
  }
  //              стоим на левой ноге
  else if (readyForRotLeftFoot(mot))
  {
    oldActions = walkBk;
    while (1)
    {
      // поворачиваемся на левой ноге
      if (orient_steps(stepAngle, left_leg, backward, mot) && (stWork == StWork)) return general_error;
      // опускаем правую ногу
      if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;
      if (fStepsCounter(mot, halfLengthStepsCount)) return general_error;
      // перевозим тележку вправо
      if (moveMassRight(mot)) return general_error; // ехать вправо
      if ((stWork == StWork) && fBreak(not_leg, knee, mot)) return general_error;
      // поднимаем левую ногу
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
      // слушаем ББ на случай команды стоп выходим из цикла
      // поворачиваемся вперед на правой ноге на нужный угол
      if (orient_steps(stepAngle, right_leg, backward, mot) && (stWork == StWork)) return general_error;
      // опускаем левую ногу (будем стоять на обоих)
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;
      if (fStepsCounter(mot, halfLengthStepsCount)) return general_error;
      // перевозим тележку влево
      if (moveMassLeft(mot)) return general_error; // ехать влево
      if ((stWork == StWork) && fBreak(not_leg, knee, mot)) return general_error;
      // поднимаем(втягиваем) правую ногу.
      if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
      //                if (fBreak(right_leg, knee)) break;
    }
  }
  return 0;
}














//  ------------------- КАЛИБРОВКА ТЕЛЕЖКИ-----------
int Cal_telega(void)
{
  int err = 0;
#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, LOW);
#endif
  // ехать медленно вправо если смотреть спереди
  err = pr_telega.RotateStps4Step(*motorLink,  -60L);
  if (err == 2) 	fErrorMes("Cal-telega-input-err");
  if (err == 3)
  { fErrorMes("Wrong Direction");
  }
#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, HIGH);
#endif
  return err;
}









// ------------------ КАЛИБРОВКА ВСЕГО РОБОТА -------------------------
bool fCalibration(/*UART_Serial & SerL, UART_Serial & SerR,*/ posOfMotors& mot)
{
  char cmdL = 0x0, cmdR = 0x0;
  //  long mot.LeftLegCurrentSteps, mot.RightLegCurrentSteps, mot.rbOrient;

  fOtladkaMes("Starting-calibration");
#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, LOW);
#endif

 // moveMassLeft(mot);
  

  // Калибровка обоих ног
  delay(50);
  mot.Calibrate_left_leg();
  delay(10);
  Serial.println("Left leg strength to endcup");
  mot.Calibrate_right_leg();
  Serial.println("Right leg strength to endcup");


  // ждем пока не получим ответ от двух
  // концевиков ног о выполненной калибровке
  long kal_time = millis();
  bool in_cycle = 1;
  bool AnswerL = 0;
  bool AnswerR = 0;
  while (in_cycle)
  {
    if (SerL.handle_serial() > 0) // чего-то принято от левой?
    {
      SerL.getString0(cmdL); // смотрим чего пришло
      if (cmdL != 'C')
      {
        fErrorMes("LeftLegWrongAnswer");
        mot.Calibrate_left_leg();
        //  SerL.prepareMessage('K');
        //       delay(10);
      }
      else {
        AnswerL = 1;
        //        fOtladkaMes("EstOtvetLevoy");
      }
    }

    if (SerR.handle_serial() > 0) // чего-то принято от левой?
    {
      SerR.getString0(cmdR); // смотрим чего пришло
      if (cmdR != 'C')
      {
        fErrorMes("RightLegWrongAnswer");
        mot.Calibrate_right_leg();
        //       delay(10);
      }
      else {
        AnswerR = 1;

        //        fOtladkaMes("EstOtvetPravoy");
      }
    }
    if (AnswerL && AnswerR)
    {
      mot.SetLeftLegCurrentSteps(0);
      mot.SetRightLegCurrentSteps(0);
      in_cycle = 0;
      cmdL = cmdR = 0;
      break;
    }
    if ((millis() - kal_time) > 20000) // если через 20 секунд не вышли из цикла
    {
      fErrorMes("KalibrovkaError");
      return general_error;
    }
    if ((millis() - kal_time) > 5000) // если через 5 секунд не вышли из цикла
    {
      if ((AnswerL == 0) && (AnswerR == 0))
      {
        fErrorMes("ZhdemObeNogi");
        mot.Calibrate_left_leg();  // вначале вытягиваем левую ногу
        delay(10);
        mot.Calibrate_right_leg(); // потом правую
      }
      else if (AnswerL == 0)
      {
        fErrorMes("Zhdem pravuyu");
        mot.Calibrate_left_leg();
        //        SerL.prepareMessage('K'); // вначале вытягиваем левую ногу
      }
      else if (AnswerR == 0)
      {
        fErrorMes("Zhdem levuyu");
        mot.Calibrate_right_leg();
        //        rSer.prepareMessage('K'); // потом правую
      }
      delay(10);
      kal_time = kal_time + 5000;
      //		fErrorMes("DolgoNetOtveta");
    }
  }     // in cycle

  if (Cal_telega())
  { fErrorMes("NoTelega");
    return general_error;
  }
  //  else fOtladkaMes("Telega");// переносим центр на правую ногу
  // fOtladkaMes("SetVtygSpeed");
  mot.Send_BTAHYTb_Speed(3500); //14000 / 4


  // втянем правую
  //  fOtladkaMes("Vtyag"); delay(1000);
  mot.MoveRightLegCurrentSteps(-stepsPerLegRot * 15L / lengthPerLegRot, BTANYTb_CH);
  if (fAnswerWait(right_leg, knee, mot, 'T', 'h')) return general_error;


  // повернем стопу до концевика
  delay(50);

  mot.Calibrate_right_foot();
  delay(50);
  if (fAnswerWait(right_leg, zero, mot)) return general_error;
  //  fOtladkaMes("->0_Done");

  mot.MoveLeftLegCurrentSteps(-stepsPerLegRot * 15L / lengthPerLegRot , BTANYTb_CH);

  if (fAnswerWait(left_leg, knee, mot, 'h', 'T')) return general_error;


  // двигаем тележку влево
  moveMassRight(mot);
  // втягиваем на 30 мм

  // еще втянем левую
  mot.MoveLeftLegCurrentSteps(-stepsPerLegRot * 30L / lengthPerLegRot, BTANYTb_CH);
  if (fAnswerWait(left_leg, knee, mot, 'h', 'T')) return general_error;

  // повернем стопу до концевика
  mot.Calibrate_left_foot();
  delay(50);
  if (fAnswerWait(left_leg, zero, mot)) return general_error;

  // устанавливаем левую ногу вровень с правой
  mot.MoveRightLegCurrentSteps(-stepsPerLegRot * 30L / lengthPerLegRot, BTANYTb_CH);
  if (fAnswerWait(right_leg, knee, mot, 'T', 'h')) return general_error;


  // двигаемся в исходное положение
  delay(100);

  mot.MoveLeftLegCurrentSteps(0, BbITANYTb_CH);
  delay(pauseForSerial);
  mot.MoveRightLegCurrentSteps(0, BbITANYTb_CH);

  if (fAnswerWait(all_legs, knee, mot, BbITANYTb_CH, BbITANYTb_CH)) return general_error;
  fOtladkaMes("SetVtygSpeed");


#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, HIGH);
#endif
  return 0;
}


void EndOfCalibration( posOfMotors& mot)
{
  delay(10);
  
//  unsigned short vtagSpeed  = 4500;
  // убрал параметр, так как без него скорость установится из еепром
  mot.Send_BTAHYTb_Speed();
  delay(pauseForSerial);
  // после калибровки установить режим с поднятыми ногами

  // скорости и ускорения стоп
  fOtladkaMes("SetRotSpeed");
  // аналогично предыдущему комменту
  mot.Send_Rot_Speed(/*4000*/);
  fOtladkaMes("SetRotAccel");
  mot.Send_Rot_Accel(/*3000*/);

  char buff[100];
  readString(TelegaSpeedAddr, buff);
  TelSpec.speed = atoi(buff);
  buff[0] = '\0';
  pr_telega.setTelegaSpec(TelSpec);
  delay(pauseForSerial);

  readString(TelegaAccelAddr, buff);
  TelSpec.accel = atoi(buff);
  pr_telega.setTelegaSpec(TelSpec);
  delay(pauseForSerial);
// Конец калибровки
	
}












// --------- ОСТАНОВКА ИЛИ ОБРАБОТКА ПОВОРОТА ----------------
bool fBreak(robot_leg leg, MKmotor Uzel, posOfMotors & mot)
{
  //  fOtladkaMes("InBreak");
  char cmdL = 0x0, cmdR = 0x0;
  //  const char Stop_immidiatly = 'F';
  int k = 0;
  unsigned long x = 0;
  char buff[3];
  bool out = 0;
  long Data1L, Data1R;
  byte sizeOfRFdata = 32;
  const short delay_nrf_pause = 10;


  // Обработка данных, получаемых с канала обмена
#ifdef SERIAL_USB
  if (Serial1.available())
#else
  if (Serial.available())
#endif
  {
    byte RF_data[sizeOfRFdata];
    //  delay(10);
    byte serialAmount = 0;
    // читаем канал обмена
#ifdef SERIAL_USB
    while (Serial1.available())
    {
      RF_data[serialAmount] = Serial1.read();
#else
    while (Serial.available())
    {
      RF_data[serialAmount] = Serial.read();
#endif
      serialAmount++;
      delayMicroseconds((10000000 / baudRateCom1) + 500);
    }

    //   delay(100);
    // ===== Режим воспроизвдения ??? =======
    buff[0] = RF_data[1]; buff[1] = RF_data[2]; buff[2] = '\0';
    x = atoi(&buff[0]);


    // это нужно, чтобы команды поворота могли исполняться во время ходьбы,
    // к остановке они отношения не имеют.
    if (RF_data[0] == 'J') // команда повернуть влево
    {
      mot.OrientInc(x);
      //  rotPlace(mot, minRbtStep, x);
 //     Actions = chngOrient;
      //      fSendState(stWork,  Actions, x);
//      fAddInActionInRecordMode(Actions, x);
    }
    if (RF_data[0] == 'H') // команда повернуть вправо
    {
      //   rotPlace(mot, minRbtStep, -x);
      mot.OrientInc(x);
 //     Actions = chngOrient;
      //      fSendState(stWork,  Actions, x);
//      fAddInActionInRecordMode(Actions, x);
    }

    // команда повтора воспроизведения
    if (((RF_data[0] == 'K') && (RF_data[1] == '4') && (RF_data[2] == '4')) && (stWork == StPlay))
      LoopPlay == 0 ? LoopPlay = 1 : LoopPlay = 0;

    // ОСНОВНАЯ КОНМАДА " П Р Е Р В А Т Ь "
    if ((RF_data[0] == 'K') && (RF_data[1] == '1') && (RF_data[2] == '0'))
    { // если прерываетия режим воспроизведения, то включается режим "работа"
      if (stWork == StPlay) stWork = StWork;
      out = 1;
      if (leg == left_leg)  // для левой ноги
      {
        //    fOtladkaMes("LeftLegBreak");
        //        SerL.prepareMessage( Stop_immidiatly);
        //        SerL.prepareMessage('F'); команда на экстренную остановку ног
        if (Uzel == knee)
        {
          //	  fOtladkaMes("LftKneeNswrWt..");
          // ожидание ответа от ноги
          while (cmdL != 'A')
          { // ждем ответ от колена
            if ((k % 100) == 0) //остаток от деления равен нулю?
              Serial1.print(".");
            k++;
            delay(delay_nrf_pause);
            if (SerL.handle_serial() > 0) // что-то принято?
            {
              SerL.getString1(cmdL, Data1L);
              if (cmdL == 'A')
              {
                // mot.LeftLegCurrentSteps = Data1L;
                //		fOtladkaMes("ok\r\n");
              } else if (cmdL == 'B')
              {
                fErrorMes("LegNotFootAnswer");
              }
              else
              {
                fErrorMes("LEFT-KNEE-ERROR");
              }
            }
          }
        }
        if (Uzel == foot)
        { // ждем ответа от стопы

          //	  fOtladkaMes("Lft Ft nswr wt");
          k = 0;
          while (cmdL != 'B')
          {
            if ((k % 100) == 0) //остаток от деления равен нулю?
              Serial1.print(".");
            k++;
            delay(delay_nrf_pause);
            if (SerL.handle_serial() > 0)
            {
              SerL.getString1(cmdL, Data1L);
              if (cmdL == 'B')
              {
                //                if (mot.LeftFootCurrentSteps != Data1L)
                //                  fOtladkaMes("LftFtStpsErr");
              } else if (cmdL == 'A')
              {
                fErrorMes("FootNotLegAnswer");
              }
              else {
                fErrorMes("LEFT-FOOT-ERROR");
              }
              // При записи, работе, воспроизведении вызов работает кооректно
              // при записи и воспроизвдении при простой хотьбе это дублирует имеющуюся команду
              //         fSendState(stWork, Actions);
            }
          }
        }
        parkingTimer = millis();// запоминаем время для парковки
      }
      delay(10);
      if (leg == right_leg)
      {
        //    fOtladkaMes("RghtLgBrk\r\n");
        //        SerR.prepareMessage('F');
        if (Uzel == knee)
        { // ожидание ответа от ноги
          //	  fOtladkaMes("RghtKneeNswrWt..");
          k = 0;
          while (cmdR != 'A')
          { // ждем ответ от колена
            if ((k % 100) == 0) //остаток от деления равен нулю?
              Serial1.print(".");
            k++;
            delay(delay_nrf_pause);
            if (SerR.handle_serial() > 0) // что-то принято?
            {
              SerR.getString1(cmdR, Data1R);
              if (cmdR == 'A')
              {
                //              mot.RightLegCurrentSteps = Data1R;
                //			fOtladkaMes("ok\r\n");
              } else if (cmdR == 'B')
              {
                fErrorMes("LegNotFootAnswer");
              }
              else
              {
                fErrorMes("RIGHT-KNEE-ERROR");
              }
            }
          }
        }
        if (Uzel == foot)
        { // ожидание ответа от ноги
          //	  fOtladkaMes("RghtFtAnswrWt..");
          k = 0;
          while (cmdR != 'B')
          {
            if ((k % 100) == 0) //остаток от деления равен нулю?
              Serial1.print(".");
            k++;
            delay(delay_nrf_pause);
            if (SerR.handle_serial() > 0)
            {
              SerR.getString1(cmdR, Data1R);
              if (cmdR == 'B')
              {
                //            if (mot.RightFootCurrentSteps != Data1R)
                //              fOtladkaMes("RghtFtStpsErr");
              } else if (cmdR == 'A')
              {
                fErrorMes("FootNotLegAnswer");
              }
              else
              {
                fErrorMes("RIGHT FOOT ERROR");
              }
              // в режиме воспроизведения при остановке нужно послать сигнал наверх
              // при записи и воспроизвдении при простой хотьбе это дублирует имеющуюся команду
              //    fSendState(stWork, Actions);
            }
          }
        }
        parkingTimer = millis();// запоминаем время для парковки
      }
    }
    pr_telega.fDoezd(*motorLink);
    if (out == 1)
    {
      fOtladkaMes("BreakOK=", out);
    }
  }
  //  RF_data[0] = '0';
  cmdL = cmdR = 0;
  //  fOtladkaMes("Break_ok");
  return out;
} // ==== fBreak =======





// опускаем поднятую ногу
void StepsManage(posOfMotors &mot, const regimRaboty &mode)
{
  //  fOtladkaMes("StepManage");
  if (mot.GetLeftLegCurrentSteps() != mot.GetRightLegCurrentSteps())
  {

    //    if ((millis() - parkingTimer) >= waitTimeForParkingLeg)
    {
      if (mot.GetLeftLegCurrentSteps() < mot.GetRightLegCurrentSteps()) //стоим на правой ноге?
      {
        // опускаем левую ногу (будем стоять на обоих)
        if (mot.GetRightLegCurrentSteps() == 0)
          Leg_fn(left_leg, mode, vytianut, mot);
        else
        {
          mot.MoveLeftLegCurrentSteps(mot.GetRightLegCurrentSteps(), BbITANYTb_CH);
          fAnswerWait(left_leg, knee, mot);
        }
        //      if (fBreak(left_leg, knee, mot)) return;
      }
      if (mot.GetRightLegCurrentSteps() < mot.GetLeftLegCurrentSteps()) //стоим на левой ноге?
      {
        // опускаем правую ногу (будем стоять на обоих)
        if (mot.GetLeftLegCurrentSteps() == 0)
          Leg_fn(right_leg, mode, vytianut, mot);
        else {
          mot.MoveRightLegCurrentSteps(mot.GetLeftLegCurrentSteps(), BbITANYTb_CH);
          fAnswerWait(right_leg, knee, mot);
        }
        //      if (fBreak(right_leg, knee, mot)) return;
      }
    }
  }
}



// Отправка текущего состояния для записи и воспроизведения
void fSendState(StadyWork WorkSt, actions Action, long param)
{
  //  char   Str3[50];
  //  String out = "";
  if ((WorkSt == StRec))
  {
    char Stp[10]  = "";
    char Str1[10];
    char Str2[10] = "";
    fOtladkaMes("");
    fOtladkaMes("InfSendState");
    fOtladkaMes("");
    switch (Action)
    {
      case walkFf:
        //   Stp = "K14"; // малая s означает количество пройденных шагов
        strcpy(Stp, "K14");
        //        delay(20);
        //	    Str1 = "s";
        strcpy(Str1, "s");

        if (param > 999) break;
        itoa(param, Str2);
        //        Str2 = String(param); // число шагов конвертируем в строку
        //        Str1.concat(Str2);  // добавляем к 's' число шагов и сохраняем в Str1
        strcat(Str1, Str2);
        writeString(Str1);
        break;

      case walkBk:
        strcpy(Stp, "K13");
        //        Stp = "K13"; // малая s означает количество пройденных шагов
        //	    Str1 = "s";
        strcpy(Str1, "s");
        /*
                Str2 = String(param); // число шагов конвертируем в строку
                Str1.concat(Str2);
        		writeString(Str1);
        */
        if (param > 999) break;
        itoa(param, Str2);
        strcat(Str1, Str2);
        writeString(Str1);

        break;

      case lUp:
        strcpy(Stp, "K15");
        //        Stp = "K15";
        break;

      case rUp:
        strcpy(Stp, "K16");
        //        Stp = "K16";
        break;

      case turnL:
        //        Stp = "K65";
        strcpy(Stp, "K65");
        break;

      case turnR:
        //        Stp = "K64";
        strcpy(Stp, "K64");
        break;

      case standStil:
        //        Stp = "K45"; //
        strcpy(Stp, "K45");
        break;

      case telCenter:
        //        Stp = "K18"; //
        strcpy(Stp, "K18");
        break;

      case shakeOn:
        //        Stp = "K60";
        strcpy(Stp, "K60");
        break;

      case shakeOff:
        //        Stp = "K61";
        strcpy(Stp, "K61");
        break;
        // повернуться влево вперед
      case turnLfst:
        //        Stp = "K67";
        strcpy(Stp, "K67");
        break;
        // повернуться вправо вперед
      case turnRfst:
        //        Stp = "K66";
        strcpy(Stp, "K66");
        break;
        // повернутся влево назад
      case turnLbst:
        //        Stp = "K69";
        strcpy(Stp, "K69");
        break;
        // повернуться вправо назад
      case turnRbst:
        //        Stp = "K68";
        strcpy(Stp, "K68");
        break;
        // поприветстовать
 //     case shakeRot:
        //        Stp = "K63";
//        strcpy(Stp, "K63");
//        break;

      case stDn:
        //        Stp = "K40";
        strcpy(Stp, "K40");
        break;

      case walkUpSt:
        //        Stp = "K70";
        strcpy(Stp, "K70");
        break;

      case walkDnSt:
        //        Stp = "K71";
        strcpy(Stp, "K71");
        break;







      case legSpeed:
#ifdef _EPROM1_
        //        Stp = "S";
        strcpy(Stp, "S");
        //        Str2 = String(param); // конвертируем в строку
        //        Stp.concat(Str2);
        itoa(param, Str2);
        strcat(Stp, Str2);
        writeString(Stp);
#endif
        return;
      case legAccel:
#ifdef _EPROM1_
        //        Stp = "A";
        strcpy(Stp, "A");
        //        Str2 = String(param); // конвертируем в строку
        //        Stp.concat(Str2);
        itoa(param, Str2);
        strcat(Stp, Str2);
        //        writeString(Str1, 5);
        writeString(Stp);
#endif
        return;

      case wait:
#ifdef _EPROM1_
        //        Stp = "D";
 

        //        RecordingTime(lPreviosCmdTime, 0);
        if (bPause) // кнопка паузы
        {
          strcpy(Stp, "P");
          itoa(startTime - endTime, Str2);
          //          Str2 = String(startTime - endTime);
          bPause = 0;
        }
        else
        {
		  strcpy(Stp, "D");
		  ltoa(param, Str2, 10); // запись текущего времени в память
		}
        strcat(Stp, Str2);
        writeString(Stp);
        //       fOtladkaMes(readString(StrAddr - incriment));
#endif
        return;
      case goSlow: // режим энергосбережения (1)
        //       fastModeCmd //  
        strcpy(Stp, "K34");

#ifdef _EPROM1_
        writeString(Stp);
        //        fOtladkaMes(readString(StrAddr - incriment));
#endif
        return;
      case goFast: // смена режима на противоположный (0)
        strcpy(Stp, "K35");
        //        walkTypeCmd; // малая s означает количество пройденных шагов
#ifdef _EPROM1_
        writeString(Stp);
        //        fOtladkaMes(readString(StrAddr - incriment));
#endif
        return;
      case aPause:
        //      Stp = "K46";
        strcpy(Stp, "K46");
        //      oPause = 1;
#ifdef _EPROM1_
        writeString(Stp);
#endif
        return;
      case UgolL:
#ifdef _EPROM1_
        //        Stp = "L";
        strcpy(Stp, "L");
        itoa(param, Str2);
        strcat(Stp, Str2);
        //        Str2 = String(param); // конвертируем в строку
        //        Stp.concat(Str2);
        writeString(Stp);
        //        fOtladkaMes(readString(StrAddr - incriment));

#endif
        return;
      case UgolR:
#ifdef _EPROM1_
        strcpy(Stp, "R");
        //        Stp = "R";
        itoa(param, Str2);
        strcat(Stp, Str2);
        //        Str2 = String(param); // конвертируем в строку
        //        Stp.concat(Str2);
        writeString(Stp);
        //       fOtladkaMes(readString(StrAddr - incriment));
#endif
        return;
      // угол дошагания
      case DoStep:
#ifdef _EPROM1_
        //        Stp = "m";
        strcpy(Stp, "m");

        //        Str2 = String(param); // конвертируем в строку
        //        Stp.concat(Str2);
        itoa(param, Str2);
        strcat(Stp, Str2);
        writeString(Stp);
        //       fOtladkaMes(readString(StrAddr - incriment));
#endif
        return;
      case Deg10:
        //      Stp = "K20";
        strcpy(Stp, "K20");
        break;

      case Deg20:
        //        Stp = "K21";
        strcpy(Stp, "K21");
        break;

      case Deg30:
        //        Stp = "K22";
        strcpy(Stp, "K22");
        break;

      case Deg45:
        //        Stp = "K23";
        strcpy(Stp, "K23");
        break;

      case Deg60:
        //        Stp = "K24";
        strcpy(Stp, "K24");
        break;

      case Deg70:
        //        Stp = "K25";
        strcpy(Stp, "K25");
        break;

      case Deg80:
        //        Stp = "K26";
        strcpy(Stp, "K26");
        break;

      case Deg90:
        //        Stp = "K27";
        strcpy(Stp, "K27");
        break;

      case Deg180:
        //        Stp = "K47";
        strcpy(Stp, "K47");
        break;

      case H20:
        //        Stp = "K28";
        strcpy(Stp, "K28");
        break;

      case H30:
        //        Stp = "K29";
        strcpy(Stp, "K29");
        break;

      case H50:
        //        Stp = "K30";
        strcpy(Stp, "K30");
        break;

      case H80:
        //        Stp = "K31";
        strcpy(Stp, "K31");
        break;

      case H120:
        strcpy(Stp, "K32");
        //        Stp = "K32";
        break;

      case H160:
        strcpy(Stp, "K33");
        //      Stp = "K33";
        break;

      case H180:
        strcpy(Stp, "K36");
        //    Stp = "K36";
        break;

      case H220:
        strcpy(Stp, "K37");
        //      Stp = "K37";
        break;

      case H360:
        strcpy(Stp, "K38");
        //      Stp = "K38";
        break;

      default:
        fErrorMes("UnknownCommand");
        //       Stp = "";
        return;
    }
    // запоминаем время повледней комманды
    /*   if (oPause)
       {
         String Stp1 = "D";
         long lT = millis() - lPreviosCmdTime;
         Str2 = String(lT); // конвертируем в строку
         Stp1.concat(Str2);
      #ifdef _EPROM1_
         writeString(Stp1);
      #endif
       }            */
#ifdef _EPROM1_

    writeString(Stp);
    //    fOtladkaMes(Stp);
#endif
    //	Serial1.println("GotPreviosTime!");
    //    lPreviosCmdTime = millis();

  }
  /*   //  УСЛОВИЕ НИКОГДА НЕ ВЫПОЛНИТСЯ!!!!!!!!!!!!
    if ((WorkSt == StPlay))
    {
    Str1 = "K43";
    }
  */
}











// --------- ПОДНЯТИЕ ИЛИ ОПУСКАНИЕ ЛЮБОЙ НОГИ ---------------
//Вытягиваем/втягиваем ногу относительно текущего положения.
/* Входные данные:
  leg - левая или правая нога
  regim - быстрый или надежный (менее затратный на вытянутых ногах)
  dir - втягивание (подъем) или вытягивание (опускание)
  lDeep - глубина подъема или опускания, относительно текущего положения
  ( если не заданна то используется глобальное значение) должна быть больше 0


*/
bool Leg_fn( robot_leg leg, const regimRaboty &regim, leg_dir dir, posOfMotors & mot, long lDeep)
{


  // Если готов повернуться на LeftFoot и нужно вытянуть leftLeg то ошибка
  if (readyForRotLeftFoot(mot) && (dir == vytianut) && (leg == left_leg))
  {
    fErrorMes("WrongLeg1"); return 0;
  }

  // Если готов повернуться на RightFoot и нужно вытянуть RightLeg то ошибка
  if (readyForRotRightFoot(mot) && (dir == vytianut) && (leg == right_leg))
  {
    fErrorMes("WrongLeg2"); return 0;
  }

  // Если готов повернуться на RightFoot и нужно втянуть RightLeg то ошибка
  if (readyForRotRightFoot(mot) && (dir == vtianut) && (leg == right_leg))
  {
    fErrorMes("WrongLeg3"); return 0;
  }

  // Если готов повернуться на LeftFoot и нужно втянуть leftLeg то ошибка
  if (readyForRotLeftFoot(mot) && (dir == vtianut) && (leg == left_leg))
  {
    fErrorMes("WrongLeg4"); return 0;
  }

  return Leg_pfn(leg, regim, dir, mot, lDeep);

} // ========= Leg_fn ==========



bool Leg_pfn( robot_leg leg, const regimRaboty &regim, leg_dir dir, posOfMotors & mot, long lDeep)
{
  //  fOtladkaMes("Leg_up/dn");
  // old_leg = leg;
  bool out = 0;
  if ((mot.GetCurrentZero() == 0) && (regim == fast)) {
    fErrorMes("PEREKLUCHI REGIM!!!");
    return general_error;
  }
  // save current StepsDepth
  auto old_steps_depth = mot.GetStepsDepthInSteps();
  if (lDeep <= 0) lDeep = mot.GetStepsDepthInSteps();
  else  mot.SetStepsDepthInSteps(lDeep);
  long halfLdeep = lDeep / 2;

  if (dir == vytianut)
  {
    if (leg == left_leg) // левая нога
    {
      if (regim == fast) // БЫСТРЫЙ РЕЖИМ
      {
        // вытягивается медленнее, поэтому его выше
        mot.MoveLeftLegCurrentSteps(mot.GetCurrentZero(), BbITANYTb_CH);
        delay(pauseForSerial);
        mot.MoveRightLegCurrentSteps(mot.GetCurrentZero(), BTANYTb_CH);
        delay(1);
        //       delay(200);
#ifndef _TIHOHOD_
        if (moveMassLeft(mot)) out = 1;
        pr_telega.fDoezd(*motorLink);
#endif
        if (fAnswerWait(all_legs,  knee, mot, BbITANYTb_CH, BTANYTb_CH)) out = 1; //ждем
        // не хватает алгоритма приведения переменной вытянуто в соответствие!!!!!!!
      }
      else
      { // energysaving
        mot.MoveLeftLegCurrentSteps(mot.GetCurrentZero(), BbITANYTb_CH);
        delay(1);
        if (fAnswerWait(left_leg, knee, mot, BbITANYTb_CH)) out =  1; //ждем
        LegUpL = vytianut; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      }
    }
    else if (leg == right_leg)
    { // правая нога
      if (regim == fast) // БЫСТРЫЙ РЕЖИМ
      {
        mot.MoveRightLegCurrentSteps(mot.GetCurrentZero(), BbITANYTb_CH);
        delay(pauseForSerial);
        mot.MoveLeftLegCurrentSteps(mot.GetCurrentZero(), BTANYTb_CH);
        delay(1);
#ifndef _TIHOHOD_
        //      delay(200);
        if (moveMassRight(mot)) out = 1;
        pr_telega.fDoezd(*motorLink);
#endif

        if (fAnswerWait(all_legs,  knee, mot, BTANYTb_CH, BbITANYTb_CH)) out = 1; //ждем
        // не хватает алгоритма приведения переменной вытянуто в соответствие!!!!!!!
      }
      else
      { // energysaving
        mot.MoveRightLegCurrentSteps(mot.GetCurrentZero(), BbITANYTb_CH);
        delay(1);
        if (fAnswerWait(right_leg, knee, mot, 'T', BbITANYTb_CH)) out = 1; //ждем
        LegUpR = vytianut;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      }
    }
  }
  else if (dir == vtianut)
  {
    if (leg == left_leg) // левая нога
    {
      if (regim == fast) // БЫСТРЫЙ РЕЖИМ
      { //         текущее положение
        mot.MoveRightLegCurrentSteps(mot.GetCurrentZero() + halfLdeep, BbITANYTb_CH);
        delay(pauseForSerial);
        mot.MoveLeftLegCurrentSteps(mot.GetCurrentZero() - halfLdeep, BTANYTb_CH);
        delay(1);
        if (fAnswerWait(all_legs,  knee, mot, BTANYTb_CH, BbITANYTb_CH)) out = 1; //ждем
        // не хватает алгоритма приведения переменной вытянуто в соответствие!!!!!!!
      }
      else
      { // energysaving
        mot.MoveLeftLegCurrentSteps(mot.GetCurrentZero() - lDeep, BTANYTb_CH);
        delay(1);
        if (fAnswerWait(left_leg, knee, mot, BTANYTb_CH, 'T')) out = 1; //ждем
        LegUpL = vtianut; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      }
    }
    else if (leg == right_leg)
    { // правая нога
      if (regim == fast) // БЫСТРЫЙ РЕЖИМ
      {
        mot.MoveLeftLegCurrentSteps(mot.GetCurrentZero() + halfLdeep, BbITANYTb_CH);
        delay(pauseForSerial);
        mot.MoveRightLegCurrentSteps(mot.GetCurrentZero() - halfLdeep, BTANYTb_CH);
        delay(1);
        if (fAnswerWait(all_legs,  knee, mot, BbITANYTb_CH, BTANYTb_CH)) out = 1; //ждем
        // не хватает алгоритма приведения переменной вытянуто в соответствие!!!!!!!
      }
      else
      { // energysaving
        mot.MoveRightLegCurrentSteps(mot.GetCurrentZero() - lDeep, BTANYTb_CH);
        delay(1);
        if (fAnswerWait(right_leg, knee, mot, 'T', BTANYTb_CH)) out = 1; //ждем
        LegUpR = vtianut;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      }
    }
  }
  //  fOtladkaMes("Done: " + String(out));
  parkingTimer = millis(); // отсчет для совпадения ног
  mot.SetStepsDepthInSteps(old_steps_depth);
  return out;
}




// двигать двумя ногами сразу

byte LegsMoveTo(posOfMotors & mot, long LeftLegPos, char L_BTAHYTb, long RightLegPos, char R_BTAHYTb)
{

  if (mot.GetCurrentZero() != 0) {
    fErrorMes("LegsMoveTo: CurrZer!=0"); delay(5000);
    return general_error;
  }
  int out = 0;
  mot.MoveLeftLegCurrentSteps(LeftLegPos,   L_BTAHYTb);
  mot.MoveRightLegCurrentSteps(RightLegPos, R_BTAHYTb);
  if (fAnswerWait(all_legs,  knee, mot, L_BTAHYTb, R_BTAHYTb)) out = 1; //ждем
  return out;
}



byte ZadatPolozhenieLevoy(posOfMotors & mot, long LegPos, char BTAHYTb)
{
  if (mot.GetCurrentZero() != 0) {
    fErrorMes("LegLMoveTo: CurrZer!=0");
    return general_error;
  }
  int out = 0;
  mot.MoveLeftLegCurrentSteps(LegPos, BTAHYTb);
  if (fAnswerWait(left_leg,  knee, mot, BTAHYTb, 'T')) out = 1; //ждем
  return out;
}



byte ZadatPolozheniePravoy(posOfMotors & mot, long LegPos, char BTAHYTb)
{
  if (mot.GetCurrentZero() != 0) {
    fErrorMes("LegRMoveTo: CurrZer!=0");
    return general_error;
  }
  int out = 0;
  mot.MoveRightLegCurrentSteps(LegPos, BTAHYTb);
  if (fAnswerWait(right_leg,  knee, mot, BTAHYTb, 'T')) out = 1; //ждем
  return out;
}





//отправляет насколько нужно втянуть/вытянуть в нужную ногу
// Точнее отправляет координаты положения ноги
/*
  byte LegXXXMoveTo(posOfMotors & mot, long LegPos, char BTAHYTb, robot_leg leg)
  {
  if (mot.GetCurrentZero() != 0) {
    fErrorMes("LegRMoveTo: CurrZer!=0");
    return general_error;
  }
  int out = 0;
  if (leg == right_leg)  mot.MoveRightLegCurrentSteps(LegPos, BTAHYTb);
  if (leg == left_leg)   mot.MoveLeftLegCurrentSteps(LegPos, BTAHYTb);

  if (fAnswerWait(leg,  knee, mot)) out = 1; //ждем
  return out;
  }
*/



// изменяет текущее положение на заданную величину

bool OpustytLevuyNoguNa(leg_dir dir, posOfMotors & mot, long lDeep)
{
  // глобальные переменные: mot.LeftLegCurrentSteps, mot.CurrentZero,mot.stepsDepthInSteps,
  // mot.RightLegCurrentSteps,LegUpL
  fOtladkaMes("Leg_up/dn");
  bool out = 0;

  if (lDeep <= 0) lDeep = mot.GetStepsDepthInSteps();
  if (dir == vytianut)
  {
    {
      mot.IncLeftLegCurrentSteps(lDeep);
      mot.SendLeftLegCurrentSteps(BbITANYTb_CH);
      //        SerL.prepareMessage( 'i', mot.GetLeftLegCurrentSteps()); // вытягиваем
      delay(1);
      if (fAnswerWait(left_leg, knee, mot)) out =  1; //ждем
      //     LegUpL = vytianut; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
  }
  else if (dir == vtianut)
  {
    {
      mot.IncLeftLegCurrentSteps(-lDeep);
      mot.SendLeftLegCurrentSteps(BTANYTb_CH);
      //        SerL.prepareMessage( 'h', mot.GetLeftLegCurrentSteps()); // втягиваем
      delay(1);
      if (fAnswerWait(left_leg, knee, mot)) out = 1; //ждем
      //    LegUpL = vtianut; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
  }







  //  fOtladkaMes("Done: " + String(out));
  // parkingTimer = millis(); // отсчет для совпадения ног
  return out;
} // ========= LegToPos ==========



bool OpustytPravuyNoguNa(leg_dir dir, posOfMotors & mot, long lDeep)
{
  // глобальные переменные: mot.LeftLegCurrentSteps, mot.CurrentZero,mot.stepsDepthInSteps,
  // mot.RightLegCurrentSteps,LegUpL
  //  fOtladkaMes("Leg_up/dn");
  bool out = 0;

  if (lDeep <= 0) lDeep = mot.GetStepsDepthInSteps();
  if (dir == vytianut)
  {
    { // правая нога
      mot.IncRightLegCurrentSteps(lDeep);
      mot.SendRightLegCurrentSteps(BbITANYTb_CH);

      //        SerR.prepareMessage( 'i', mot.RightLegCurrentSteps); // вытягиваем
      delay(1);
      if (fAnswerWait(right_leg, knee, mot)) out = 1; //ждем
      //    LegUpR = vytianut;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
  }
  else if (dir == vtianut)
  {
    { // правая нога
      mot.IncRightLegCurrentSteps(-lDeep);
      mot.SendRightLegCurrentSteps(BTANYTb_CH);
      //        SerR.prepareMessage( 'h', mot.RightLegCurrentSteps); // втягиваем
      delay(1);
      if (fAnswerWait(right_leg, knee, mot)) out = 1; //ждем
      //     LegUpR = vtianut;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
  }
  //  fOtladkaMes("Done: " + String(out));
  // parkingTimer = millis(); // отсчет для совпадения ног
  return out;
} // ========= LegToPos ==========












//  ------ ОЖИДАНИЕ ОТВЕТА ОТ ПРИВОДА НОГ -----
bool fAnswerWait(robot_leg leg, MKmotor Uzel, posOfMotors & mot,  char cL,  char cR)
{

  //static long old_LeftLegCurrentSteps = mot.LeftLegCurrentSteps;
  //static long old_RightLegCurrentSteps = mot.RightLegCurrentSteps;
  //  int k = 0;
  char cmdL = 0x0, cmdR = 0x0;
  const short delay_nrf_pause = 10;
  unsigned long last_time = millis();
  long Data1;
  bool Leg;
  if (leg == left_leg)  Leg = 0;
  if (leg == right_leg) Leg = 1;

  // long oldFootData1L = mot.LeftFootCurrentSteps, oldFootData1R = mot.RightFootCurrentSteps;
  // long oldKneeData1L = mot.LeftLegCurrentSteps, oldKneeData1R = mot.RightLegCurrentSteps;

  if ((leg == left_leg) || (leg == right_leg))  while (fNoBlockAnsWait(Uzel, mot, Leg) != 0)
    {
      /*
          if (k == 1)
          {
            fErrorMes("k = 1");
            return general_error;
          }
      */
      if (last_time + return_time < millis())
      {
        if (Leg == 0) fErrorMes("LEFT-LEG-TIME-ERROR");
        else if (Leg == 1) fErrorMes("RIGHT-LEG-TIME-ERROR");
        else fErrorMes("LEG-TIME-ERROR");
        //       SerL.prepareMessage( cL, mot.LeftLegCurrentSteps); // втягиваем
        //       last_time =  millis();
        //         mot.LeftLegCurrentSteps = oldKneeData1L;
#ifdef STOP_ON_ERROR
        return general_error;
#endif
      }
      pr_telega.fDoezd(*motorLink);
    }
  else if (leg == all_legs)
  {
    if (Uzel == knee)
    {
      //	  fOtladkaMes("RghtNdLftKneeNswrWt\r\n");
      //     k = 0;
      while ((cmdR != 'A') || (cmdL != 'A'))
      {
        //        if (fBreak(right_leg, foot)) return general_error;
        //      if ((k % 100) == 0) //остаток от деления равен нулю?
        //		  fOtladkaMes(".");
        //        k++;
        delay(delay_nrf_pause);
        if (SerR.handle_serial() > 0)
        {
          SerR.getString1(cmdR, Data1);
          if (cmdR == 'A')
          {
            //         fOtladkaMes("RKnee: " + String(Data1R));
            if (mot.GetRightLegCurrentSteps() != Data1)
            {
              fErrorMes("R LEG MUST BE: ", mot.GetRightLegCurrentSteps());
              //    delay(2000);
            }
          }
        }
        if (SerL.handle_serial() > 0)
        {
          SerL.getString1(cmdL, Data1);
          if (cmdL == 'A')
          {
            //        fOtladkaMes("LKnee: " + String(Data1L));
            //           if (mot.LeftLegCurrentSteps != Data1L)
            //         {
            //            fErrorMes("MUST BE: " + String(mot.LeftLegCurrentSteps));
            //           delay(2000);
            //           mot.LeftLegCurrentSteps = Data1L;
            //         }
          }
        }
        // в режиме воспроизведения?
        if (last_time + return_time < millis())
        {
          if (cmdR != 'A')
          {
            fErrorMes("RIGHT KNEE TIME ERROR");
            //           mot.RightLegCurrentSteps = oldKneeData1R;
            SerR.prepareMessage( cR, mot.GetRightLegCurrentSteps()); // втягиваем
            last_time =  millis();
            cmdR = 0;
          }
          if (cmdL != 'A')
          {
            fErrorMes("LEFT KNEE TIME ERROR");
            //           mot.LeftLegCurrentSteps = oldKneeData1L;
            SerL.prepareMessage( cL, mot.GetLeftLegCurrentSteps()); // втягиваем
            last_time =  millis();
            cmdL = 0;
          }
          //   delay(2000);
#ifdef STOP_ON_ERROR

          //   return general_error;
#endif
          //    break;
        }
        //        if (digitalRead(ENDCAP_R) == EC_LOW) // правая стопа, правый упор телеги контролируется
        pr_telega.fDoezd(*motorLink);
      }
    }

    else if (Uzel == foot)
    {
      //	  fOtladkaMes("RghtNdLftFtNswrWt\r\n");
      //      k = 0;
      while ((cmdR != 'B') || (cmdL != 'B'))
      {
        //        if (fBreak(right_leg, foot)) return general_error;
        //        if ((k % 100) == 0) //остаток от деления равен нулю?
        //		  fOtladkaMes(".");
        //          k++;
        delay(delay_nrf_pause);
        if (SerR.handle_serial() > 0)
        {
          SerR.getString1(cmdR, Data1);
          if (cmdR == 'B')
          {
            //      fOtladkaMes("RFoot: " + String(Data1R));
            if (mot.GetRightFootCurrentSteps() != Data1)
            {
              fErrorMes("R FOOT MUST BE: ", mot.GetRightFootCurrentSteps());
              //      delay(2000);
              mot.SetRightFootCurrentSteps(Data1);
            }
          }
        }
        if (SerL.handle_serial() > 0)
        {
          SerL.getString1(cmdL, Data1);
          if (cmdL == 'B')
          {
            //           fOtladkaMes("LFoot: " + String(Data1L));
            if (mot.GetLeftFootCurrentSteps() != Data1)
            {
              fErrorMes("L FOOT MUST BE: ", mot.GetLeftFootCurrentSteps());
              //         delay(2000);
              mot.SetLeftFootCurrentSteps(Data1);
            }
          }
        }

        if (last_time + return_time < millis())
        {
          if (cmdR != 'B')
          {
            fErrorMes("RIGHT FOOT TIME ERROR");
            fErrorMes("RFoot: ", Data1);
            //        fErrorMes("MUST BE: " + String(mot.RightFootCurrentSteps));
            //         mot.RightFootCurrentSteps = oldFootData1R;
          }
          if (cmdL != 'B')
          {
            fErrorMes("LEFT FOOT TIME ERROR");
            //        fErrorMes("LFoot: " + String(Data1L));
            //        fErrorMes("MUST BE: " + String(mot.LeftFootCurrentSteps));
            //        mot.LeftFootCurrentSteps = oldFootData1L;
          }
          delay(2000);
#ifdef STOP_ON_ERROR
          cmdL = cmdR = 0;
          return general_error;
#endif
          break;
        }
        //        if (digitalRead(ENDCAP_R) == EC_LOW) // правая стопа, правый упор телеги контролируется
        pr_telega.fDoezd(*motorLink);
      }
    }
  }


  endTime = millis();

  // cmdL = cmdR = 0;
  return 0;
} // ===== fAnswerWait ===









// это для хотьбы..
// Функция поворачивает выбранную ногу до необходимого угла
// и доворачивает вторую ногу на такой же угол в обратную сторону
// leg - нога, на которой совершается поворот
// dir - в какую сторону идти (вперед или назад)
// возвращает 1 если была ошибка, 0 если все штатно
bool orient_steps(long stepAngle, robot_leg leg, step_dir dir, posOfMotors & mot)
{
  //  угол  = угол в градусах * шагов_на_оборот / 360грд + поворот для шага.
  // angleL - положение стопы относительно калиброванного 0 в тиках.
  // mot.rbOrient - угол направления движения в градусах
  //  fOtladkaMes("Pov");
  //  fOtladkaMes("rbOrient" + String(mot.GetOrient()));

  //  const unsigned short gradus = 1;
  long angleR = 0; // куда нужно повернуть правую
  long angleL = 0; // куда нужно повернуть левую
  // поворот на левой ноге
  //                            -5             -(-10) = +5
  if (leg == left_leg)
  {
    if (readyForRotLeftFoot(mot))   // стоим на левой ноге?
    {
      //     angleR =  mot.rbOrient * 160000L / 360L - stepAngle;
      //     angleL = -mot.rbOrient * 160000L / 360L + stepAngle;
      angleR =  mot.GetOrient() * angleToStep() - stepAngle;
      angleL = -mot.GetOrient() * angleToStep() + stepAngle;
      //   leed_leg = leftLeed;
      // в какую сторону идем,назад или вперед?
      if (dir == forward)  //
      {
        angleR =  mot.GetOrient() * angleToStep() + stepAngle;
        angleL = -mot.GetOrient() * angleToStep() - stepAngle;
        //      leed_leg = rightLeed;
      }

      // условие при котором перепутано направление
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      if (angleR == mot.GetRightFootCurrentSteps())
      {
        if (dir == unknown)
        {
          angleR =  mot.GetOrient() * angleToStep() + stepAngle;
          angleL = -mot.GetOrient() * angleToStep() - stepAngle;
          //       leed_leg = rightLeed;
          fOtladkaMes("VozmozhnoPereputanoNapravlenie");
        }
      }
      mot.SetRightFootCurrentSteps(angleR);
      mot.SetLeftFootCurrentSteps(angleL);

      if (mot.GetRightFootCurrentSteps() == mot.GetLeftFootCurrentSteps())
      {
        fOtladkaMes("LFCurntSteps", mot.GetLeftFootCurrentSteps());
        fOtladkaMes("RFCurntSteps", mot.GetRightFootCurrentSteps());
        fOtladkaMes("stepAngle", stepAngle);
        fOtladkaMes("GetOrient()", mot.GetOrient());
        mot.SendRightFootCurrentSteps();
        //        SerR.prepareMessage( 'c', mot.RightFootCurrentSteps);
        delay(pauseForSerial);
        mot.SendLeftFootCurrentSteps();
        //        SerL.prepareMessage( 'c', mot.LeftFootCurrentSteps);
        if (fAnswerWait(all_legs,  foot, mot)) return general_error; //ждем
      }
      else {
        mot.SendRightFootCurrentSteps();

        //      fOtladkaMes("TekUgol= " + String(mot.RightFootCurrentSteps));
        delay(pauseForSerial);
        // крутим левую стопу на положительный заданный угол + ориентация
        mot.SendLeftFootCurrentSteps();
        //      fOtladkaMes("TekUgol= " + String(mot.LeftFootCurrentSteps));
        // ждем отработку поворота на правой ноге
        delay(100);
        if (fAnswerWait(all_legs,  foot, mot)) return general_error; //ждем
      }
    }
    else
    {
      fErrorMes("<LEFT TURN TERM ERROR>");
      delay(5000);
    }
  }
  // поворот на правой ноге
  //                            - 5           - (-10) = +5 разница между ногами
  else if (leg == right_leg)   // стоим на правой ноге?
  {
    if (readyForRotRightFoot(mot))
    {
      angleR =  mot.GetOrient() * angleToStep() + stepAngle; // куда нужно повернуть правую
      angleL = -mot.GetOrient() * angleToStep() - stepAngle; // куда нужно повернуть левую
      //    leed_leg = rightLeed;

      if (dir == forward)
      {
        angleR =  mot.GetOrient() * angleToStep() - stepAngle; // куда нужно повернуть правую
        angleL = -mot.GetOrient() * angleToStep() + stepAngle; // куда нужно повернуть левую
        //      leed_leg = leftLeed;
      }

      if (angleR == mot.GetRightFootCurrentSteps())
      {
        if (dir == unknown)
        {
          angleR =  mot.GetOrient() * angleToStep() - stepAngle;
          angleL = -mot.GetOrient() * angleToStep() + stepAngle;
          //         leed_leg = leftLeed;
        }
      }

      mot.SetRightFootCurrentSteps(angleR);
      mot.SetLeftFootCurrentSteps(angleL);
      // заранее переводим левую стопу в исходное положение
      if (mot.GetRightFootCurrentSteps() == mot.GetLeftFootCurrentSteps())
      {
        fOtladkaMes("LFCurntStps", mot.GetLeftFootCurrentSteps());
        fOtladkaMes("RFCurntStps", mot.GetRightFootCurrentSteps());
        fOtladkaMes("stepAngle", stepAngle);
        fOtladkaMes("GetOrient()", mot.GetOrient());
        mot.SendRightFootCurrentSteps();
        delay(pauseForSerial);
        mot.SendLeftFootCurrentSteps();
        if (fAnswerWait(all_legs,  foot, mot)) return general_error; //ждем
      }
      else {
        mot.SendLeftFootCurrentSteps();
        //      fOtladkaMes("TekUgol= " + String(mot.LeftFootCurrentSteps));
        delay(pauseForSerial);
        // крутим правую стопу на положительный заданный угол + ориентация
        mot.SendRightFootCurrentSteps();
        if (fAnswerWait(all_legs,  foot, mot)) return general_error; //ждем
      }
    }
    else
    {
      fErrorMes("<RIGHT TURN TERM ERROR>");
      delay(5000);
    }
  }
  else fErrorMes("No foot to orient steps");
  return 0;
}












byte rotPlace1(posOfMotors & mot, unsigned short minStep, short newOrient, regimRaboty &mode)
{
  int turnTimes = abs(newOrient / minStep);
  int ostatok   = abs(newOrient % minStep);
  bool bDir;

  if (turnTimes == 0) {
    fErrorMes("Small angle");//fMinStp(mot, minStep, mode);
    return general_error;
  }
  bool bEW = fEastOrWestStandingCalc(mot);
  if (newOrient < 0)
  { bDir = 0;
    minStep = -abs(minStep);
    ostatok = -abs(ostatok);
  }
  else bDir = 1;

  fOtladkaMes("AbsUgolDo=", mot.GetOrient());
  fOtladkaMes("turnTimes=", turnTimes);
  fOtladkaMes("ostatok=", ostatok);
  fOtladkaMes("bEW=", bEW);
  fOtladkaMes("bDir=", bDir);

  //  startTime = millis();

  if (fTurnToNTimes(mot, minStep, mode, bEW, turnTimes, bDir)) return general_error;

  fOtladkaMes("AbsUgol=", mot.GetOrient());

  if (bDir == 0) fDoPovorotL(mot, mode, ostatok);
  else if (bDir == 1) fDoPovorotR(mot, mode, ostatok);

  fOtladkaMes("AbsUgolPosle=", mot.GetOrient());

  endTime = millis();  //второй этап это доехать просто оставшийся угол
  // проверка, что нога повернута в соответствии с ориентацией

  return 0;
}















void PlayFromEEPROM(bool zoom,  posOfMotors& mot)
{
#ifdef _EPROM1_
  if (stWork == StRec) return;
  fOtladkaMes("PlayStarts");
  //  byte sizeOfRFdata = 30;
  //  char RF_data[sizeOfRFdata];

  //  fOtladkaMes("NmbOfStrgs= " + String(readString(0)));
  char buff[incriment];
  // сперва читаем то, что нужно прочесть один раз
  // в принципе это не обязательно, можно будет потом убрать эту запись
  // В ней хорошо то, что экономится время и не выполняется лишнее чтение.
  /*  if ((unsigned)(readString(0).toInt()) >= (6 * incriment))
    {
      for (StrAddr = StartEEPROM; StrAddr < (StartEEPROM + 6 * incriment); StrAddr += incriment)
      {
        char data_out[11];
        buff = readString(StrAddr);
        //    fOtladkaMes("StrAdr= " + String(StrAddr));
        //    fOtladkaMes(buff);
        buff.toCharArray(data_out, buff.length() + 1);
        RF_messege_handle(data_out, drvPos);
        buff = "";
      }
    }
  */
  //  unsigned int str_number = (unsigned)readString(0).toInt();

  char tmp2[incriment];
  //  char *pBuff = buff;
  readString(0, tmp2);
  unsigned int str_number = (unsigned)atoi(tmp2);
  fOtladkaMes("StringsNumber ", str_number);  // ОТЛАДКА
  StartRecordTime = millis();
  do {
    for (StrAddr = StartEEPROM; StrAddr < str_number; StrAddr += incriment)
    {
      if (stWork != StPlay)
      {
        fErrorMes("stWork != StPlay");
        break;
      }
      char data_out[incriment];
      readString(StrAddr, buff);
	  fOtladkaMes("StrAdr= ");              // ОТЛАДКА
      fOtladkaMes((StrAddr));                 // ОТЛАДКА
      fOtladkaMes((buff));                    // ОТЛАДКА
      if (strlen(buff) > incriment) fErrorMes("WARN!FULBUF");
      strcpy(data_out, buff); // stoping copy at termonated nullcharecter and including it.
      RF_messege_handle(data_out, mot);
      strcpy(buff, "\0");
      zoom = LoopPlay;
    }  // for..
  } while (zoom);
#endif
  CheckUpMes("PlayStops");
}








bool readyForRotLeftFoot(posOfMotors& mot)
{
  if ((mot.GetRightLegCurrentSteps() - mot.GetLeftLegCurrentSteps()) <= -mot.GetStepsDepthInSteps()) return general_error;
  else
  {
#ifdef _OTLADKA_
#ifdef SERIAL_USB
    //  Serial1.print("LFTBLK\r\n");
    //	fOtladkaMes("RLCurntStps:"+String(mot.RightLegCurrentSteps));
    // fOtladkaMes("LLCurntStps:"+String(mot.LeftLegCurrentSteps));
    // fOtladkaMes("stpsDpthInStps:"+String(mot.stepsDepthInSteps));

#endif
#endif
    return 0;
  }
}






bool readyForRotRightFoot(posOfMotors& mot)
{
  if ((mot.GetRightLegCurrentSteps() - mot.GetLeftLegCurrentSteps()) >= mot.GetStepsDepthInSteps()) return general_error;
  else
  {
#ifdef _OTLADKA_
#ifdef SERIAL_USB

    //       Serial1.print("RFTBLK\r\n");
    //       fOtladkaMes("RLCurntStps:"+String(mot.RightLegCurrentSteps)+ " - ");
    //       fOtladkaMes("LLCurntStps:"+String(mot.LeftLegCurrentSteps) + " >= ");
    //       fOtladkaMes("stpsDpthInStps:"+String(mot.stepsDepthInSteps));

#endif
#endif
    return 0;
  }
}










bool fstandStill(posOfMotors& mot)
{
  if (mot.GetRightLegCurrentSteps() == mot.GetLeftLegCurrentSteps()) return general_error;
  else
  {
#ifdef _OTLADKA_
#ifdef SERIAL_USB
    //   Serial1.print("STANDING ON ONE LEG\r\n");
#endif
#endif
    return 0;
  }
}
//   leedLeg leedLgOgj, c какой ноги идти?
//  short angle)  на какой угол поворачивать ногу
//











// меняется алгоритм Leg_fn
// присесть или привстать, меняется текущий режим хотьбы
// mode это режим работы 0 - поменять текущее положение
// 1 - привстать, 2 - присесть.
byte seetUpDown(posOfMotors & mot,  const byte mode)
{
  static bool HalfPos = 1;
  if (!mode)
  {
    if (mot.GetLeftLegCurrentSteps() == 0) HalfPos = 1;
    else HalfPos = 0;
  }
  else if (mode == 1) HalfPos = 0;
  if (mode == 2) HalfPos = 1;
  if (HalfPos)
  {
    mot.MoveLeftLegCurrentSteps(-stepsPerLegRot * fullRotationLimit / 2, BTANYTb_CH);
    delay(pauseForSerial);

    mot.MoveRightLegCurrentSteps(-stepsPerLegRot * fullRotationLimit / 2, BTANYTb_CH);
    mot.SetCurrentZero(mot.GetLeftLegCurrentSteps());
    delay(pauseForSerial);
    if (fAnswerWait(all_legs, knee, mot)) return general_error;
  }
  else
  {
    mot.MoveLeftLegCurrentSteps(0, BbITANYTb_CH);
    // вытягиваем
    delay(pauseForSerial);
    mot.MoveRightLegCurrentSteps(0, BbITANYTb_CH);
    // вытягиваем
    mot.SetCurrentZero(mot.GetLeftLegCurrentSteps());
    delay(pauseForSerial);
    if (fAnswerWait(all_legs, knee, mot)) return general_error;
  }
  return 0;
}












// перемещение телеги в центр
void telega_to_center(posOfMotors& mot, bool shouldRec)
{
  if (fstandStill(mot))
  {
    /*    if (stWork == StRec)
        {
          Actions = telCenter;
          fSendState(stWork, Actions, 0);
        }
    */
//    fAddInActionInRecordMode(wait);
    pr_telega.RotateStpsOnly(motorLink[0],  0L);
    if(shouldRec) 
		fAddInActionInRecordMode(telCenter);
//  эта строчка здесь не нужна потому что она дублирует  предыдущую функцию
//    endTime = millis();

  }
}











void telega_left(posOfMotors& mot)
{
  if (fstandStill(mot))
    pr_telega.RotateStpsOnly(motorLink[0],  pr_telega.DriveLeft()); // ехать влево
}












void telega_right(posOfMotors& mot)
{
  if (fstandStill(mot))
    pr_telega.RotateStpsOnly(motorLink[0],  pr_telega.DriveRight()); // ехать вправо
}












void topLeft(posOfMotors & mot)
{
  fOtladkaMes("Vidvinut-levuyu");

 // fAddInActionInRecordMode(wait);

  if (fstandStill(mot))
  {
    moveMassRight(mot);
    LegUpL = vtianut;
  }
  else {
    if (readyForRotLeftFoot(mot)) return; // если стоим на левой ноге, то выходим
    LegUpL = vytianut;
  }
  LegUpL == vtianut ? LegUpL = vytianut : LegUpL = vtianut;
  if (LegUpL == vytianut) {
    mot.MoveLeftLegCurrentSteps(mot.GetCurrentZero() - mot.GetStepsDepthInSteps(), BTANYTb_CH);
    //    mot.SetLeftLegCurrentSteps(mot.GetCurrentZero() - mot.GetStepsDepthInSteps());
    //    SerL.prepareMessage( 'h', mot.GetLeftLegCurrentSteps()); // вытягиваем
    if (fAnswerWait(left_leg, knee, mot)) return;
  }
  if (LegUpL == vtianut)
  {
    if ((mot.GetLeftLegCurrentSteps() + mot.GetStepsDepthInSteps()) >= 0)
      mot.SetLeftLegCurrentSteps(0L);
    else mot.SetLeftLegCurrentSteps(mot.GetCurrentZero());
    mot.SendLeftLegCurrentSteps(BbITANYTb_CH);
    //    SerL.prepareMessage( 'i', mot.GetLeftLegCurrentSteps()); // втягиваем
    if (fAnswerWait(left_leg, knee, mot)) return;
  }
  fAddInActionInRecordMode(lUp);
  return;
}









void topRight(posOfMotors & mot)
{
  fOtladkaMes("Vidvinut-pravuyu");
 // fAddInActionInRecordMode(wait);
  // телега справа, если смотреть спереди?
  if (fstandStill(mot)) {
    moveMassLeft(mot);
    LegUpR = vtianut;
  }
  else {
    if (readyForRotRightFoot(mot)) return; // если стоим на правой ноге, то выходим
    LegUpR = vytianut;
  }
  LegUpR == vtianut ? LegUpR = vytianut : LegUpR = vtianut;
  if (LegUpR == vytianut)
  {
    mot.MoveRightLegCurrentSteps(mot.GetCurrentZero() - mot.GetStepsDepthInSteps(), BTANYTb_CH);
    //    mot.RightLegCurrentSteps = mot.GetCurrentZero() - mot.GetStepsDepthInSteps() ;
    //    SerR.prepareMessage( 'h', mot.RightLegCurrentSteps); // вытягиваем
    if (fAnswerWait(right_leg, knee, mot)) return;
  }
  if (LegUpR == vtianut)
  {
    if ((mot.GetRightLegCurrentSteps() + mot.GetStepsDepthInSteps()) >= 0)
      mot.SetRightLegCurrentSteps(0L);
    else mot.SetRightLegCurrentSteps(mot.GetCurrentZero());
    mot.SendRightLegCurrentSteps(BbITANYTb_CH);
    //    SerR.prepareMessage( 'i', mot.GetRightLegCurrentSteps()); // втягиваем
    if (fAnswerWait(right_leg, knee, mot)) return;
  }
  fAddInActionInRecordMode(rUp);
  return;
}











// вращение стопой
// может вращать в разные стороны по очереди или по отдельности
void foot_rotation(const robot_leg leg, const byte direction, short angle, short times, posOfMotors & mot)
{
  // posOfMotors mot; // для совместимости
  long steps = angleToStep(angle);

  long angleL = -mot.GetOrient() * angleToStep();
  long angleR =  mot.GetOrient() * angleToStep();

  //if(angle < 0) direction = 1;
  if (direction == 1) steps = -steps; // направление по часовой стрелке?

  if (leg == left_leg)
  {
    if (readyForRotRightFoot(mot) ) // стоим на правой ноге?
    {
      if (direction == 0) // вращать в разные стороны по очереди
      {
        while (times > 0)
        {
          mot.MoveLeftFootCurrentSteps(angleL - steps);
          //          SerL.prepareMessage( 'c', angleL - steps);
          if (fAnswerWait(left_leg,  foot, mot)) return; //ждем
          //          SerL.prepareMessage( 'c', angleL + steps);
          mot.MoveLeftFootCurrentSteps(angleL + steps);
          if (fAnswerWait(left_leg,  foot, mot)) return; //ждем
          times--;
        }
      } else
      {

        mot.MoveLeftFootCurrentSteps(angleL + steps);
        //        SerL.prepareMessage( 'c', angleL + steps);
        if (fAnswerWait(leg,  foot, mot)) return; //ждем
        SerL.prepareMessage( 'j', angleL);
        if (fAnswerWait(leg,  foot, mot, 'j', 'T')) return; //ждем
      }
    }
    else
    {
      fErrorMes("CHANGE FOOT");
      delay(10000);
    }
  }
  if (leg == right_leg)
  {
    if (readyForRotLeftFoot(mot) ) // стоим на левой ноге?
    {
      if (direction == 0) // вращать в разные стороны по очереди
      {
        while (times > 0)
        {
          mot.MoveRightFootCurrentSteps(angleR - steps);
          //          SerR.prepareMessage( 'c', angleR - steps);
          if (fAnswerWait(right_leg,  foot, mot)) return; //ждем
          //          SerR.prepareMessage( 'c', angleR + steps);
          mot.MoveRightFootCurrentSteps(angleR + steps);
          if (fAnswerWait(right_leg,  foot, mot)) return; //ждем
          times--;
        }
      }
      else // направление по часовой стрелке?
      {
        mot.MoveRightFootCurrentSteps(angleR + steps);
        //    SerR.prepareMessage( 'c', angleR + steps);
        if (fAnswerWait(leg,  foot, mot)) return; //ждем
        SerR.prepareMessage( 'j', angleR);
        if (fAnswerWait(leg,  foot, mot, 'T', 'j')) return; //ждем
      }
    }
    else
    {
      fErrorMes("CHANGE FOOT");
      delay(10000);
    }
  }
}














// поочередное поднятие ног с вращением стопы поднятой ноги то в одну сторону
// то в другую сторону
void demo1(posOfMotors& mot)
{
  //  regimRaboty oldmode = mode;
  //  posOfMotors mot; // для совместимости
  // rot_dir dir;
  regimRaboty mode = energySaving;

  seetUpDown(mot, 1); // привстать

  if (fstandStill(mot))
  {
    pr_telega.fDoezd(*motorLink);
    if (moveMassLeft(mot)) goto skip; // ехать влево
    // поднимаем(втягиваем) правую ногу.
    if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) goto skip;
    foot_rotation(right_leg, 0, 30, 2, mot);
    // опускаем правую ногу
    if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) goto skip;
    // телега едет вправо
    if (moveMassRight(mot)) goto skip ;
    // поднимаем(втягиваем) левую ногу.
    if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) goto skip;
    foot_rotation(left_leg, 0, 30, 2, mot);
    // опускаем  ногу
    if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) goto skip;
  }
skip:
  //  if (oldmode == fast)
  // {
  //   seetUpDown(mot, 2); // присесть
  //   mode = fast;
  // }
  stWork = StWork;
}















// проходы вперед назад по 4 шага с остановкой и приседаниями. Во время приседаний телега
// становится в центр
bool demo2(posOfMotors & mot)
{
  regimRaboty mode = energySaving;
  seetUpDown(mot, 1);
  long stepAngle = angleToStep();
  stWork = StPlay;
  while (1)
  {
    maxStepsCount = 4;
    if (walkForward(stepAngle, mode, mot)) break;

    telega_to_center(mot,0);

    //   <<<<<<<<<<<<<<< Сели, встали >>>>>>>>>>>>>>
    seetUpDown(mot);
    delay(2000);
    seetUpDown(mot);

    maxStepsCount = 4;
    if (walkBackward(stepAngle, mode, mot)) break;

    seetUpDown(mot);
    delay(2000);
    seetUpDown(mot);

  }
  stWork = StWork;
  return 0;
}

















void demo3(posOfMotors & mot)
{
  regimRaboty mode = fast;
  long stepAngle = angleToStep();
  stWork = StPlay;
  // 4 шага вперед
  maxStepsCount = 4;
  walkForward(stepAngle, mode, mot);
  // поворот влево
  //  oldOrient = mot.rbOrient;
  rotPlace1(mot, 30, 90,  mode);
  Actions = turnL;

  // 4 шага вперед
  walkForward(stepAngle, mode,  mot);
  // поворот влево
  //  oldOrient = mot.rbOrient;
  rotPlace1(mot, 30, 90,  mode);
  Actions = turnL;

  // 4 шага вперед
  walkForward(stepAngle, mode,  mot);
  // поворот влево
  //  oldOrient = mot.rbOrient;
  rotPlace1(mot, 30, 90,  mode);
  Actions = turnL;

  // 4 шага вперед
  walkForward(stepAngle, mode, mot);
  // поворот влево
  //  oldOrient = mot.rbOrient;
  rotPlace1(mot, 30, 90,  mode);
  Actions = turnL;

  stWork = StWork;

  maxStepsCount = 0;
}





// Функция запоминает предыдущее значение времени
// и передает его в качестве параметра по ссылке
byte RecordingTime(long &prevTime, const unsigned long &treshold)
{
  static long time = 0;
  prevTime = time;
  if (millis() - StartRecordTime < treshold)
  {
    return 0;
  } else {
    time = millis() - StartRecordTime;
    //   Serial1.println("prevtime="+ (String)time);
    return general_error;
  }
}
// Функция возвращает разницу во времени с начала действия
//
unsigned long RecordingTime(void)
{
  return millis() - StartRecordTime;
}














// Качаемся из стороны в сторону
// если режим равен нулю, то качаемся
// если режим равен 1, то возвращаемся в исходное положение
byte fShake(posOfMotors & mot, regimRaboty &mode, const byte mode1)
{
  telega_to_center(mot,0);
  //Если стоим на вытянутых, в режиме экономичном
  //  то чтобы перекатиться с ноги на ноги нужно опуститься.
  static regimRaboty old_reg;
  if ((mode == energySaving) && (mode1 == 0) && (mot.GetRightLegCurrentSteps() == 0))
  {
    // присесть
    seetUpDown(mot, 2);
    // чтобы  больше не заходить в этот блок
    old_reg = mode;
    change_mode(mode, 0);// = fast;
  }
  if (mode1 == 1)
  {
    mode = old_reg;
  }
  static bool HPos;
  if (!mode1) HPos == 1 ? HPos = 0 : HPos = 1;
  else if (mode1 == 1)
  {
    mot.SetLeftLegCurrentSteps(mot.GetCurrentZero());
    mot.SetRightLegCurrentSteps(mot.GetCurrentZero());
    if (HPos)
    {
      SerL.prepareMessage( 'i', mot.GetLeftLegCurrentSteps());  // втягиваем
      delay(pauseForSerial);
      SerR.prepareMessage( 'h', mot.GetRightLegCurrentSteps()); // вытягиваем
      delay(pauseForSerial);
      if (fAnswerWait(all_legs, knee, mot)) return general_error;
    }
    else
    {
      SerL.prepareMessage( 'h', mot.GetLeftLegCurrentSteps());  // вытягиваем
      delay(pauseForSerial);
      SerR.prepareMessage( 'i', mot.GetRightLegCurrentSteps()); // втягиваем
      delay(pauseForSerial);
      if (fAnswerWait(all_legs, knee, mot)) return general_error;
    }
    if ((mode == energySaving) && (mode1 == 1))
    {
      seetUpDown(mot, 1);
    }

    return 0;
  };
  //наклоны
  long teta = 1500;
  if (HPos)
  {
    //    mot.MoveLeftLegCurrentSteps(-stepsPerLegRot * fullRotationLimit / 2 - teta, BTANYTb_CH);
    mot.SetLeftLegCurrentSteps(-stepsPerLegRot * fullRotationLimit / 2 - teta);
    SerL.prepareMessage( 'h', mot.GetLeftLegCurrentSteps()); // вытягиваем
    delay(pauseForSerial);
    // mot.MoveRightLegCurrentSteps(-stepsPerLegRot * fullRotationLimit / 2 + teta, BbITANYTb_CH);
    //    mot.RightLegCurrentSteps = -stepsPerLegRot * fullRotationLimit / 2 + teta;
    mot.SetRightLegCurrentSteps(-stepsPerLegRot * fullRotationLimit / 2 + teta);
    SerR.prepareMessage( 'i', mot.GetRightLegCurrentSteps()); // втягиваем
    long k = mot.GetLeftLegCurrentSteps();
    mot.SetCurrentZero(k);
    delay(pauseForSerial);
    if (fAnswerWait(all_legs, knee, mot)) return general_error;
  }
  else
  {

    // mot.MoveLeftLegCurrentSteps(-stepsPerLegRot * fullRotationLimit / 2 + teta, BbITANYTb_CH);
    mot.SetLeftLegCurrentSteps(-stepsPerLegRot * fullRotationLimit / 2 + teta);
    SerL.prepareMessage( 'i', mot.GetLeftLegCurrentSteps()); // втягиваем
    delay(pauseForSerial);
    //  mot.MoveRightLegCurrentSteps(-stepsPerLegRot * fullRotationLimit / 2 - teta, BTANYTb_CH);
    mot.SetRightLegCurrentSteps(-stepsPerLegRot * fullRotationLimit / 2 - teta);
    SerR.prepareMessage( 'h', mot.GetRightLegCurrentSteps()); // вытягиваем
    long k = mot.GetLeftLegCurrentSteps();
    mot.SetCurrentZero(k);
    delay(pauseForSerial);
    if (fAnswerWait(all_legs, knee, mot)) return general_error;
  }
  return 0;
}





/*
  // перекатываться спомощью стоп
  void fRoll(posOfMotors & mot)
  {

  long oldStepAngle = stepAngle;
  stepAngle = angleToStep(90);
  orient_steps(stepAngle,right_leg, forward, mot);
  stepAngle = oldStepAngle;

  }

*/













// Если включена запись, то функция используется для записи команды в память.
void fAddInActionInRecordMode(actions Action, long param)
{
  static bool pauseStart = false; // indicates if button pause activated
  if (stWork == StRec)
  {
	// 
    if(pauseStart&&isMoveCommands(Action)) 
	{
	  pauseStart = false;
	  endTime = millis();
	  fSendState(stWork, wait, RecordingTime());
	}
	
    if(withPauses == 1)
    {
	  fSendState(stWork, wait, recordTimer);
    }
	else
	{
		if(isMoveCommands(Action)) startTime = millis();
	    if(bPause) 
		{
		    pauseStart = true; 
		}
	}
    Actions = Action;
    fSendState(stWork, Actions, param);
  }
}

// здесь не только движимые команды
// для которых нужно сохранять время но и режимы 
// потому что режимы без времени могут срабатывать преждевременно
// сразу после предыдущей команды
/*
bool isMoveCmd(char cmd)
{
	return (cmd == cmdSelect::goBackCmd||
	cmd == cmdSelect::goForwardCmd||
	cmd == cmdSelect::rotLefttCmd||
	cmd == cmdSelect::rotRightCmd||
	cmd == cmdSelect::standStillCmd||
	cmd == cmdSelect::telegaToCenterCmd||
	cmd == cmdSelect::shakeCmd||
	cmd == cmdSelect::afterShakeCmd||
	cmd == cmdSelect::rotLefttFfCmd||
	cmd == cmdSelect::rotRightFfCmd||
	cmd == cmdSelect::rotLefttBkCmd||
	cmd == cmdSelect::rotRightBkCmd||
	cmd == cmdSelect::liftLeftCmd||
	cmd == cmdSelect::liftRightCmd||
	cmd == cmdSelect::goUpstearsCmd||
	cmd == cmdSelect::goDnStearsCmd||
	cmd == cmdSelect::fastModeCmd);
}
*/
// закомментировал потому что все команнды нужны во времени


bool isMoveCommands(const actions& Action)
{
  return (Action == walkFf||
  Action == walkBk||
  Action == turnL||
  Action == turnR||
  Action == standStil||
  Action == telCenter||
  Action == shakeOn||
  Action == shakeOff||
  Action == turnLfst||
  Action == turnRfst||
  Action == turnLbst||
  Action == turnRbst||
  Action == lUp||
  Action == rUp||
//  Action == shakeRot||
  Action == goSlow||
  Action == goFast||
  Action == walkUpSt||
  Action == walkDnSt||
  Action == stDn);
}



//  отклонение текущего положения вправо или влево от направления движения
//  Если возвращает 1, то отклонение вправо, если смотреть по ходу движения сзади
//  иначе 0.
byte fEastOrWestStandingCalc(posOfMotors& mot)
{
  // Если смотрим влево, тогда считаем, что левая нога впереди
  // (которая правая, если смотреть сзади)
  if (mot.GetRightFootCurrentSteps() > mot.GetOrient() * angleToStep())  //убрал *2
  {
    //    fOtladkaMes("RightLegFF");
    return general_error;
  }
  // Если смотрим вправо, тогда считаем, что правая нога впереди
  if (mot.GetRightFootCurrentSteps() < mot.GetOrient() * angleToStep())  //убрал *2
  {
    //    fOtladkaMes("LeftLegFF");

    return 0;
  }
  //  fOtladkaMes("LeftLeg == RigthLeg");
  return 2;
}











bool fStepsCounter(posOfMotors & mot, unsigned long &hlfStepsCnt)
{
  hlfStepsCnt++;
  if (fBreak(not_leg, knee, mot))
  {
 //   fSendState(stWork, Actions, hlfStepsCnt);
	 // поменял на эту функцию, так как при записи не зависывалось время паузы.
     fAddInActionInRecordMode(Actions, hlfStepsCnt);

     return general_error;
  }
  //  if (maxStepsCount < 0) return general_error;
  if ((stWork == StPlay) && (hlfStepsCnt >= maxStepsCount))
  {
    //       fSendState(stWork, Actions, radio, mot);
    fOtladkaMes("PlMdMxStpsRched");
    return general_error;
  }
  return 0;
}





/*
  // pos - в миллиметрах
  // возвр. 1 если ошибка
  bool fLegUpToPos(posOfMotors & mot, long pos)
  {
  if (pos < (- stepsPerLegRot * fullRotationLimit))
  {
    fErrorMes("OutOfRange");
    return general_error;
  }
  mot.RightLegCurrentSteps = - stepsPerLegRot * fullRotationLimit;
  SerR.prepareMessage( 'h', mot.RightLegCurrentSteps); // вытягиваем
  if (fAnswerWait(right_leg, knee, mot)) return general_error;

  mot.RightLegCurrentSteps = 0L;
  SerR.prepareMessage( 'i', mot.RightLegCurrentSteps); // втягиваем
  if (fAnswerWait(right_leg, knee, mot)) return general_error;
  return 0;
  }
*/


//Поворот вперед на правой ноге если смотреть сзади

byte rotateRightFF(short stepAngle, const regimRaboty &mode, posOfMotors & mot)
{
  fOtladkaMes(">Vpered");

  //  long old_rotSpeed = rotSpeed;

  if (fstandStill(mot) || readyForRotLeftFoot(mot))
  {
    const long rotSpeed = 5000;
    mot.Send_Rot_Speed(rotSpeed);
    /*
        SerL.prepareMessage( 'S', rotSpeed);
        delay(pauseForSerial);
        SerR.prepareMessage( 'S', rotSpeed);
    */
    delay(pauseForSerial);

    // защита от топтания на месте
    if (fChkOrientStpsAvailable(stepAngle, left_leg, forward, mot))	return 2;
    // перевозим тележку влево
    if (moveMassLeft(mot)) return general_error;
    if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
    mot.OrientInc(stepAngle);
    if (orient_steps(0 , left_leg, forward, mot) && (stWork == StWork)) return general_error;
    if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;
    //    rotSpeed=old_rotSpeed;
    // считанная с еепром скорость
    mot.Send_Rot_Speed();
    /*
        SerL.prepareMessage( 'S', rotSpeed);
        delay(pauseForSerial);
        SerR.prepareMessage( 'S', rotSpeed);
        delay(pauseForSerial);
    */
    return 0;
  } else fErrorMes("OnTwoLegs");
  return general_error;
}



byte rotateRightBK(short stepAngle, const regimRaboty &mode, posOfMotors & mot)
{
  fOtladkaMes(">Nazad");
  // long old_rotSpeed = rotSpeed;

  if (fstandStill(mot) || readyForRotRightFoot(mot))
  {
    const long rotSpeed = 5000;
    mot.Send_Rot_Speed(rotSpeed);
    delay(pauseForSerial);
    if (fChkOrientStpsAvailable(stepAngle, right_leg, backward, mot)) return 2;
    // перевозим тележку влево
    if (moveMassRight(mot)) return general_error;
    if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
    mot.OrientInc(stepAngle);
    if (orient_steps(0, right_leg, backward, mot) && (stWork == StWork)) return general_error;
    if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;
    //  rotSpeed=old_rotSpeed;

    mot.Send_Rot_Speed();
    delay(pauseForSerial);

    return 0;
  } else fErrorMes("OnTwoLegs");
  return general_error;
}



byte rotateLeftFF(short stepAngle, const regimRaboty &mode, posOfMotors & mot)
{
  fOtladkaMes("<Vpered");

  //  long old_rotSpeed = rotSpeed;

  if (fstandStill(mot) || readyForRotRightFoot(mot))
  {
    const long rotSpeed = 5000;
    mot.Send_Rot_Speed(rotSpeed);
    delay(pauseForSerial);
    if (fChkOrientStpsAvailable(stepAngle, right_leg, forward, mot)) return 2;
    // перевозим тележку влево
    if (moveMassRight(mot)) return general_error;
    if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
    mot.OrientInc(stepAngle);
    if (orient_steps(0, right_leg, forward, mot) && (stWork == StWork)) return general_error;
    if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;
    //    rotSpeed=old_rotSpeed;
    mot.Send_Rot_Speed();
    delay(pauseForSerial);

    return 0;
  } else fErrorMes("OnTwoLegs");
  return general_error;
}


byte rotateLeftBK(short stepAngle, regimRaboty &mode, posOfMotors & mot)
{
  fOtladkaMes("<Nazad");
  //  long old_rotSpeed = rotSpeed;
  if (fstandStill(mot) || readyForRotLeftFoot(mot))
  {
    const long rotSpeed = 5000;
    mot.Send_Rot_Speed(rotSpeed);
    delay(pauseForSerial);
    if (fChkOrientStpsAvailable(stepAngle, left_leg, backward, mot)) return 2;
    // перевозим тележку влево
    if (moveMassLeft(mot)) return general_error;
    if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return general_error;
    mot.OrientInc(stepAngle);
    if (orient_steps(0, left_leg, backward, mot) && (stWork == StWork)) return general_error;
    if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) return general_error;
    //    rotSpeed=old_rotSpeed;
    mot.Send_Rot_Speed();
    delay(pauseForSerial);

    return 0;
  } else fErrorMes("OnTwoLegs");
  return general_error;
}


// stepAngle - 	в микрошагах привода
// возвращает 0 - если поворот возможен, 1 - если нет.
bool fChkOrientStpsAvailable(long stepAngle, robot_leg leg, step_dir dir, posOfMotors & mot)
{
  //  fOtladkaMes("ChkPvrt");
  //  const unsigned short gradus = 1;
  long angleR = 0; // куда нужно повернуть правую
  long angleL = 0; // куда нужно повернуть левую
  // поворот на левой ноге
  //                            -5             -(-10) = +5
  if (leg == left_leg)
  {
    if (readyForRotLeftFoot(mot))   // стоим на левой ноге?
    {
      angleR =  mot.GetOrient() * angleToStep() * 2 - stepAngle;
      angleL = -mot.GetOrient() * angleToStep() * 2 + stepAngle;
      //  leed_leg = leftLeed;
      // в какую сторону идем,назад или вперед?
      if (dir == forward)  //
      {
        angleR =  mot.GetOrient() * angleToStep() * 2 + stepAngle;
        angleL = -mot.GetOrient() * angleToStep() * 2 - stepAngle;
        //     leed_leg = rightLeed;
      }

      // условие при котором перепутано направление
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      if (angleL == mot.GetLeftFootCurrentSteps())
      {
        fOtladkaMes("PovorotNePoluchitsia");
        return general_error;
      }
      return 0;
    }
  } else if (leg == right_leg)   // стоим на правой ноге?
  {
    if (readyForRotLeftFoot(mot))
    {
      angleR =  mot.GetOrient() * angleToStep() * 2 + stepAngle; // куда нужно повернуть правую
      angleL = -mot.GetOrient() * angleToStep() * 2 - stepAngle; // куда нужно повернуть левую
      //    leed_leg = rightLeed;

      if (dir == forward)
      {
        angleR =  mot.GetOrient() * angleToStep() * 2 - stepAngle; // куда нужно повернуть правую
        angleL = -mot.GetOrient() * angleToStep() * 2 + stepAngle; // куда нужно повернуть левую
        //       leed_leg = leftLeed;
      }

      if (angleR == mot.GetRightFootCurrentSteps())
      {
        fOtladkaMes("PovorotNePoluchitsia");
        return general_error;
      }
      return 0;
    }
  }
  return 0;
}






/*
bool fShakeHandWithRotation(posOfMotors & mot, short stepAngle)
{
  const long midle_pos =  stepsPerLegRot * fullRotationLimit *  2  / 3;
  const short upDnMove = (stepsPerLegRot * fullRotationLimit / 10) - 5;
  seetUpDown(mot, 1); // привстаем
  // {
  regimRaboty mode = energySaving;
  if (moveMassRight(mot)) return general_error;
  // сохраняем текущее  значение высоты подъема ноги
  const long old_step_depth = mot.GetStepsDepthInSteps();
  // поднимаем на половину высоты
  if (Leg_fn(left_leg, mode, vtianut, mot, midle_pos)) return general_error;
  // меняем ориентацию, чтобы повернуть на 360 градусов
  mot.OrientInc(stepAngle);
  // Чтоб махать рукой, нужно исходить из средней позиции ноги
  //  mot.CurrentZero = mot.LeftLegCurrentSteps;
  long angleR = 0; // куда нужно повернуть правую
  long angleL = 0; // куда нужно повернуть


  // задаем скорость поворота
  unsigned long old_rotSpeed = rotSpeed;

  rotSpeed = 1000;
  mot.Send_Rot_Speed(rotSpeed);
  //  SerR.prepareMessage( 'S', rotSpeed);
  delay(pauseForSerial);


  unsigned short old_vtagSpeed  = mot.GetVtagSpeed();
  unsigned short old_vytagSpeed = mot.GetVytagSpeed();
  unsigned short old_vtagAccel  = mot.GetVtagAccel();
  unsigned short old_vytagAccel = mot.GetVytagAccel();
  unsigned short vtagSpeed  = 9000;
  unsigned short vytagSpeed = 9000;
  unsigned short vtagAccel = 11000;
  unsigned short vytagAccel = 11000;


  // Крутимся
  if (readyForRotRightFoot(mot))
  {
    angleR =  mot.GetOrient() * angleToStep(); // куда нужно повернуть правую
    angleL = -mot.GetOrient() * angleToStep(); // куда нужно повернуть левую

    mot.SetRightFootCurrentSteps(angleR);
    mot.SetLeftFootCurrentSteps(angleL);



    fOtladkaMes("RightFootCurrentSteps", mot.GetRightFootCurrentSteps());
    // начинаем крутиться на правой ноге
    mot.SendRightFootCurrentSteps();

    delay(pauseForSerial);
    // левая нога должна думать что повернулась
    SerL.prepareMessage( 'j', angleL);
    delay(pauseForSerial);
    fAnswerWait(left_leg, foot, mot, 'j', 'T');
  }
  // задаем скорость и ускорение махания рукой



  mot.Set_BTAHYTb_Accel(vtagAccel);
  //  delay(pauseForSerial);
  mot.Set_BbITAHYTb_Accel(vytagAccel);
  //  delay(pauseForSerial);
  mot.Set_BTAHYTb_Speed(vtagSpeed);
  mot.Set_BbITAHYTb_Speed(vytagSpeed);
  //  SerL.prepareMessage( 'P', vtagSpeed);
  //  delay(pauseForSerial);
  //  SerL.prepareMessage( 'Q', vytagSpeed);
  //  delay(pauseForSerial);



  // махаем рукой пока крутимся
  while (fNoBlockAnswerWait(right_leg, foot))
  {
    if ((-midle_pos - upDnMove) < -stepsPerLegRot * fullRotationLimit) break;
    if (fBreak(right_leg, foot, mot)) break;   //left
    mot.MoveLeftLegCurrentSteps(- midle_pos - upDnMove, BTANYTb_CH);

    if (fAnswerWait(left_leg, knee, mot)) break;

    mot.MoveLeftLegCurrentSteps(- midle_pos + upDnMove, BbITANYTb_CH);

    if (fAnswerWait(left_leg, knee, mot)) break;
  }
  rotSpeed = old_rotSpeed;
  mot.Send_Rot_Speed();
  //  SerR.prepareMessage( 'S', rotSpeed);
  delay(pauseForSerial);

  vtagSpeed = old_vtagSpeed;
  vytagSpeed = old_vytagSpeed;
  vtagAccel = old_vtagAccel;
  vytagAccel = old_vytagAccel;

  mot.Set_BTAHYTb_Accel(vtagAccel);
  //  delay(pauseForSerial);
  mot.Set_BbITAHYTb_Accel(vytagAccel);
  //  delay(pauseForSerial);
  mot.Set_BTAHYTb_Speed(vtagSpeed);
  mot.Set_BbITAHYTb_Speed(vytagSpeed);
  //  SerL.prepareMessage( 'P', vtagSpeed);
  //  delay(pauseForSerial);
  //  SerL.prepareMessage( 'Q', vytagSpeed);
  delay(pauseForSerial);
  mot.SetStepsDepthInSteps(old_step_depth);
  StepsManage(mot, mode);
  return 0;
}
*/

// LeftOrRight  - левый-0 правый-1
byte fNoBlockAnsWait(MKmotor &Uzel, posOfMotors & mot, bool LeftOrRight)
{
  long Data1, motData = 0;
  char letter = 'G';

  switch (Uzel)
  {
    case knee:
      letter = 'A';
      motData = LeftOrRight == 0 ? mot.GetLeftLegCurrentSteps() :
                mot.GetRightLegCurrentSteps();
      // if (LeftOrRight == 0) motData = mot.LeftLegCurrentSteps;
      // else motData = mot.RightLegCurrentSteps;
      break;

    case foot:
      letter = 'B';
      motData = LeftOrRight == 0 ? mot.GetLeftFootCurrentSteps() :
                mot.GetRightFootCurrentSteps();
      // if (LeftOrRight == 0) motData = mot.LeftFootCurrentSteps;
      // else motData = mot.RightFootCurrentSteps;
      break;

    case kal:
      letter = 'C';
      break;

    case zero:
      letter = 'G';
      break;

    default:
      fErrorMes("UnknownLeg");
  }

#ifdef _OTLADKA_
  //  static int k = 0;
  //  if ((k % 300) == 0)
  //  { //остаток от деления равен нулю?
  //    Serial1.print(".");
  //    k++;
  //  }
#endif
  static char cmdX = 0x0;
  if (cmdX != letter)
  { // ждем ответ от колена
    //        if (fBreak(left_leg, knee))  return general_error;
    //    delay(delay_nrf_pause);
    if (LeftOrRight == 0) {
      if (SerL.handle_serial() > 0)
        SerL.getString1(cmdX, Data1);
    } // читаем что принято
    else if (SerR.handle_serial() > 0) SerR.getString1(cmdX, Data1);
    if (cmdX == letter)
    {
      // проверяем положение колена
      //     fOtladkaMes("LKnee:" + String(Data1L));
      if ((motData != Data1))
      {
        if (letter == 'A') {
          if (LeftOrRight == 0) Serial1.print("LEFT "); else Serial1.print("RIGHT ");
          fErrorMes("LEG MUST BE:", motData);
        }

        if (letter == 'B') {
          if (LeftOrRight != 0) Serial1.print("RIGHT "); else Serial1.print("LEFT ");
          fErrorMes("FOOT MUST BE:", motData);
        }
        //  delay(2000);
      }
      cmdX = 0x0;
      return 0;
    }
    else if (cmdX != 0x0)
    {
      fErrorMes("LftAswrKneeIs:", cmdX);
      cmdX = 0x0;
      return 2;
    }
  }
  return 8;
}


// ФУНКЦИЯ ВОЗВРАЩАЕТ:
// 0 - когда получен ответ
// 1 - когда ждем ответа
// 2 - когда есть ошибки
byte fNoBlockAnswerWait(robot_leg leg, MKmotor Uzel)
{
  long Data1L, Data1R;
  char letter = 'G';

  letter =
    Uzel == knee ?
    'A' :
    Uzel == foot ?
    'B' : letter;

  static char cmdX = 0x0;

  //	  fOtladkaMes("LftKneeAnswrWt..");
  if (cmdX != letter)
  { // ждем ответ от колена
    //        if (fBreak(left_leg, knee))  return general_error;
    //    delay(delay_nrf_pause);
    if (leg == left_leg)
    {
      if (SerL.handle_serial() > 0) // что-то принято?
      {
        SerL.getString1(cmdX, Data1L); // читаем что принято
        if (cmdX == letter)
        {
          // проверяем положение колена
          //     fOtladkaMes("LKnee:" + String(Data1L));
          //         if (mot.LeftLegCurrentSteps != Data1L)
          //         {
          //          fErrorMes("MUST BE:" + String(mot.LeftLegCurrentSteps));
          //           delay(2000);
          //              mot.LeftLegCurrentSteps = Data1L;
          //        }
          cmdX = 0x0;
          return 0;
        }
        else
        {
          //          String errStr = "LftAswrKneeIs:" + String(cmdX);
          fErrorMes("LftAswrKneeIs:");
          cmdX = 0x0;
          return 2;
        }
      }
    }
    if (leg == right_leg)
    {
      if (SerR.handle_serial() > 0)
      {
        SerR.getString1(cmdX, Data1R);
        if (cmdX == letter)
        {
          //         fOtladkaMes("RKnee:" + String(Data1R));
          //        if (mot.RightLegCurrentSteps != Data1R)
          //        {
          //         fErrorMes("MUSTBE:" + String(mot.RightLegCurrentSteps));
          //         delay(2000);
          //              mot.RightLegCurrentSteps = Data1R;
          //       }
          cmdX = 0x0;
          return 0;
        }
        else
        {
          //         String errStr = "RGHT-NSWR-KNEE-IS:" + String(Data1R);
          fErrorMes("RGHT-NSWR-KNEE-IS:");
          cmdX = 0x0;
          return 2;
        }
      }
    }
    pr_telega.fDoezd(*motorLink);
  }
  return general_error;
}


// Функция поворота на заданный угол вправо
// stepAngel - угол поворота, который меняет rbOrient
// leg = 0 - повернуться на правой ноге, если смотреть спереди
// leg = 1 - повернуться на левой ноге, если смотреть спереди
byte fTurnWithStepsR(posOfMotors & mot, short stepAngle, regimRaboty &mode, bool leg)
{
  fOtladkaMes("TurnR");

  return
    leg == 0 ?   // поворот на правой ноге(которая левая) назад
    rotateRightBK(stepAngle, mode, mot) :
    leg == 1 ?       // поворот на левой ноге(которая правая) вперед
    rotateRightFF(stepAngle, mode, mot) : 0;
}

// Функция поворота на заданный угол влево
// stepAngel - угол поворота, который меняет rbOrient
//leg = 1 - повернуться на правой ноге, если смотреть спереди
//leg = 0 - повернуться на левой ноге, если смотреть спереди
byte fTurnWithStepsL(posOfMotors & mot, short stepAngle, regimRaboty &mode, bool leg)
{
  fOtladkaMes("TurnL");

  leg == 0 ?   // поворот на левой ноге(которая правая) назад
  rotateLeftBK(stepAngle, mode, mot) :
  leg == 1 ?  // поворот на правой ноге(которая левая) вперед
  rotateLeftFF(stepAngle, mode, mot) : 0;
  return 0;
}


// FirstLeg - нага спереди 0 - левая 1 - правая
// если LeftOrRight = 0 значит влево иначе вправо
bool fTurnToNTimes(posOfMotors & mot, short stepAngle, regimRaboty &mode, bool FirstLeg,
                   int nTimes, bool LeftOrRight)
{


  if (LeftOrRight == 0) //влeво
  {
    for (int i = 0; i < nTimes; ++i)
    {
      // остаток от деления равен 0 (четное число) первая нога
      // повернуться вначале на левой ноге
      if (i % 2) switch (fTurnWithStepsL(mot, stepAngle, mode, FirstLeg))
        {
          case 0:
            //          return 0;
            //         fOtladkaMes("turn0L");
            break;
          case 1:
            fOtladkaMes("turn1L");
            return general_error;
          case 2:
            fOtladkaMes("turn2L");
            if (fTurnWithStepsL(mot, stepAngle, mode, !FirstLeg)) return general_error;
            break;
        }
      // потом на правой
      else switch (fTurnWithStepsL(mot, stepAngle, mode, !FirstLeg))
        {
          case 0:
            //         fOtladkaMes("turn0L");
            break;
          case 1:
            fOtladkaMes("turn1L");
            return general_error;
          case 2:
            fOtladkaMes("turn2L");
            if (fTurnWithStepsL(mot, stepAngle, mode, FirstLeg)) return general_error;
            break;
        }
    }
  } else if (LeftOrRight == 1) // вправо
  {
    for (int i = 0; i < nTimes; ++i)
    {
      // остаток от деления равен 0 (четное число) первая нога
      // повернуться вначале на левой ноге
      if (i % 2) switch (fTurnWithStepsR(mot, stepAngle, mode, !FirstLeg))
        {
          case 0:
            //		  return 0;
            fOtladkaMes("turn0R");
            break;
          case 1:
            fOtladkaMes("turn1R");
            return general_error;
          case 2:
            fOtladkaMes("turn2R");
            if (fTurnWithStepsL(mot, stepAngle, mode, FirstLeg)) return general_error;
            break;
        }
      // потом на правой
      else switch (fTurnWithStepsR(mot, stepAngle, mode,  FirstLeg))
        {
          case 0:
            fOtladkaMes("turn0R");
            break;
          case 1:
            fOtladkaMes("turn1R");
            return general_error;
          case 2:
            fOtladkaMes("turn2R");
            if (fTurnWithStepsL(mot, stepAngle, mode, FirstLeg)) return general_error;
            break;
        }
    }
  }
  return 0;
}




//

// поворот до нужного угла с котролем какой ногой ходил последний раз
// ostatok - насколько нужно довернуть относительно текущего положения в градусах
// bDir - поворачиваемся влево или вправо
byte fDoPovorotL(posOfMotors & mot, regimRaboty &mode, const int ostatok)
{

  bool bEW = fEastOrWestStandingCalc(mot);
  // int gradus = 1;

  // если есть остаток и ориентация ноги соответсвует ориентации робота
  // телега справа если смотреть сзади
  if (ostatok && (mot.GetOrient() * 2 * angleToStep() == mot.GetRightFootCurrentSteps()))
  {
    switch (pr_telega.dir_flg)
    {
      case left:
        if (rotateLeftFF(ostatok, mode, mot)) return general_error;
        break;
      case right:
        if (rotateLeftBK(ostatok, mode, mot))  return general_error;
        break;
      case middle:
        if (bEW == 0) if (rotateLeftFF(ostatok, mode, mot))  return general_error;
        if (bEW == 1) if (rotateLeftBK(ostatok, mode, mot))  return general_error;
        break;
      default:
        fErrorMes("unknown");
    }
    return 0;
  }
  fOtladkaMes("BezOst");
  return 2;
}






// последний шаг для поворота
byte fDoPovorotR(posOfMotors & mot, regimRaboty &mode, const int ostatok)
{
  bool bEW = fEastOrWestStandingCalc(mot);
  //  int gradus = 1;

  if (ostatok && (mot.GetOrient() * 2 * angleToStep() == mot.GetRightFootCurrentSteps()))
  {
    switch (pr_telega.dir_flg)
    {
      case left:
        if (rotateRightBK(ostatok, mode, mot))  return general_error;
        break;
      case right:
        if (rotateRightFF(ostatok, mode, mot))  return general_error;
        break;
      case middle:
        if (bEW == 0) if (rotateRightFF(ostatok, mode, mot))  return general_error;
        if (bEW == 1) if (rotateRightBK(ostatok, mode, mot))  return general_error;
        break;
      default:
        fErrorMes("unknown");

    }
    return 0;
  }
  return 2;
}


// перевезти тележку влево
byte moveMassLeft(posOfMotors & mot)
{
  if (mot.GetRightLegCurrentSteps() <= mot.GetLeftLegCurrentSteps())
  {
    if (pr_telega.RotateStpsOnly(*motorLink,  pr_telega.DriveLeft())) return general_error;
  } else
  {
    fErrorMes("Can'tMoveMassToLeft");
    return 2;
  }
  return 0;
}



// перевезти тележку вправо
byte moveMassRight(posOfMotors & mot)
{
  if (mot.GetRightLegCurrentSteps() >= mot.GetLeftLegCurrentSteps())
  {
    if (pr_telega.RotateStpsOnly(*motorLink,  pr_telega.DriveRight())) return general_error;
  } else
  {
    fErrorMes("Can'tMoveMassToRight");
    return 2;
  }
  return 0;
}







// steps_height в миллиметрах
byte goUpstears(posOfMotors & mot)
{
  ledder_var ledderVar;
  ledderVar.StepDepth = stepDephCalc(ledderVar.steps_height);
  ledderVar.DeltaStep = stepDephCalc(ledderVar.zapas_na_povorot);
  /*
    if ((ledderVar.steps_cnt < 0) || (ledderVar.steps_cnt > 5))
    {
      fErrorMes("StepsCountError");
      delay(1000);
      return 2;
    }
  */


  seetUpDown(mot, 1);  // привстаем
  // делаем шаги вверх по ступенке 3 раза

  if (upStep(mot, ledderVar)) fErrorMes("CantStepUp");
  seetUpDown(mot, 1);  // привстаем

  return 0;
  /*
    // допустим стоим впритык к лестнице, тогда первый шаг 90 градусов
    // поворот вправо вперед
    // поднимаем ногу на высоту ступени + запас в 30 мм
    // поворачиваемся на 90 гдрадусов
    // опускаем ногу на ступеньку на 30 мм
  */
}


// одно движение ногой вверх для подъема по ступенькам

byte upStep(posOfMotors & mot, ledder_var& var)
{
  //  zapas_na_povorot = 6L;
  // 1. Проверяем стоим ли на обеих ногах
  //  short angle = 90 - 15;
  //  long StepDepth = stepDephCalc(BbIcoTa_CTYnEHbKU);
  //  long DeltaStep = stepDephCalc(zapas_na_povorot);

  if (fstandStill(mot))
  {
    unsigned short old_tS = mot.GetVtagSpeed();
    unsigned short old_yS = mot.GetVytagSpeed();
    unsigned short old_rA = mot.GetRotAccel();

    mot.Send_BTAHYTb_Speed(6700);

    StepUpWhenBothStepsTogether(mot, var);

    mot.Send_BbITAHYTb_Speed(2500);
    mot.Send_BTAHYTb_Speed(3500);
    mot.Send_Rot_Accel(2500);
    //	long old_stepDepth = mot.GetStepsDepthInSteps();
    //	mot.SetStepsDepthInSteps(0);
    if (stepUpOneStepAlg1(mot, var, left_leg)) return general_error;
    if (stepUpOneStepAlg1(mot, var, right_leg)) return general_error;
    //      stepUpOneStepAlg1(mot, dir, angle, StepDepth, DeltaStep);
    //	}
    // делаем последний шаг чтобы встать обеими ногами на ступеньки
    if (stepUpOneStepAlg2(mot, var, left_leg)) return general_error;
    StepsManage(mot, energySaving);
    //	mot.SetStepsDepthInSteps(old_stepDepth);
    // 1.2. Если стоим на противоположной ноге, то
    // 1.2.1 проверяем, поднята ли одна нога выше другой на величину ступеньки
    mot.Send_BbITAHYTb_Speed(old_yS);
    mot.Send_BTAHYTb_Speed(old_tS);
    mot.Send_Rot_Accel(old_rA);

    // делаем последний шаг чтобы встать обеими ногами на ступеньки
    var.angle = 0;
    stepUpOneStepAlg1(mot, var, right_leg);
    StepsManage(mot, energySaving);
  }

  // 3. Если стояли на обоих ногах то поворачиваемся на 90 градусов
  // иначе на 180.
  return 0;
}

// сделать шаг вверх по лестнице
byte stepUpOneStepAlg1(posOfMotors & mot, ledder_var& var,  robot_leg legUp)
{
  if (fstandStill(mot))
  {
    fErrorMes("StpUpError");
    return general_error;
  }

  long raznica_v_polozhenii = fRaznost_v_polozhenii(mot);
  /*
  	    robot_leg legUp = raznica_v_polozhenii<0?
  	                                          left_leg:
  			          raznica_v_polozhenii>0?
  			                                  right_leg:legUp;


  */

  // Если разность в положении равна высоте ступеньки
  if (abs(raznica_v_polozhenii) == var.StepDepth)
  {
    //   calcLegForStep(raznica_v_polozhenii, legRot, legUp, legDn);
    //  1.2.2 перевозим тележку
    //  если нужно поднять левую ногу, то телегу двиаем вправо
    if (legUp == left_leg)
    {
      //	  if (angle == 0)
      if (LegsMoveTo(mot, 0, BbITANYTb_CH, (-var.StepDepth - var.DeltaStep), BTANYTb_CH)) return general_error;
      if (orient_steps(angleToStep(var.angle), left_leg, forward, mot)) return general_error;
      if (var.angle == (90 - 15))
      {
        if (OpustytPravuyNoguNa(vytianut, mot, var.DeltaStep)) return general_error;
      }
      if (pr_telega.RotateStpsOnly(*motorLink, pr_telega.DriveRight())) return general_error;
    }
    //  если нужно поднять правую ногу, то телегу двиаем влево
    if (legUp == right_leg)
    {
      if (LegsMoveTo(mot, (-var.StepDepth - var.DeltaStep), BTANYTb_CH, 0, BbITANYTb_CH)) return general_error;

      if (orient_steps(angleToStep(var.angle), right_leg, forward, mot)) return general_error;

      if (var.angle == (90 - 15))
      {
        //	     if (LegXXXMoveTo(mot, -BbIcoTa_CTYnEHbKU, BTANYTb_CH, left_leg)) return general_error;
        if (OpustytLevuyNoguNa(vytianut, mot, var.DeltaStep)) return general_error;
      }
      if (pr_telega.RotateStpsOnly(*motorLink,  pr_telega.DriveLeft())) return general_error;
    }
  }
  else {
    //    fOtladkaMes(static_cast<String>(raznica_v_polozhenii) + " = " + static_cast<String>(stepDephCalc(var.StepDepth)));
    delay(5000);
  }
  return 0;
}


// делаем последний шаг чтобы встать обеими ногами на ступеньки
byte stepUpOneStepAlg2(posOfMotors & mot, ledder_var& var, robot_leg legUp)
{
  if (fstandStill(mot))
  {
    fErrorMes("StpUpError");
    return general_error;
  }
  long oldStepDepth = mot.GetStepsDepthInSteps();
  long raznica_v_polozhenii = fRaznost_v_polozhenii(mot);

  /*
  	    robot_leg legUp = raznica_v_polozhenii<0?
  	                                          left_leg:
  			          raznica_v_polozhenii>0?
  			                                  right_leg:legUp;
  */

  // Если разность в положении равна высоте ступеньки
  if (abs(raznica_v_polozhenii) == var.StepDepth)
  {
    //   calcLegForStep(raznica_v_polozhenii, legRot, legUp, legDn);
    //  1.2.2 перевозим тележку
    //  если нужно поднять левую ногу, то телегу двиаем вправо
    mot.SetStepsDepthInSteps(var.DeltaStep);
    if (legUp == left_leg)
    {
      if (LegsMoveTo(mot, 0, BbITANYTb_CH, (-var.DeltaStep), BTANYTb_CH)) return general_error;
      if (orient_steps(angleToStep(180), left_leg, backward, mot)) return general_error;
      //      if (pr_telega.RotateStpsOnly(*motorLink,  pr_telega.DriveRight())) return general_error;
    }
    //  если нужно поднять правую ногу, то телегу двиаем влево
    if (legUp == right_leg)
    {
      if (LegsMoveTo(mot, (-var.DeltaStep), BTANYTb_CH, 0, BbITANYTb_CH)) return general_error;
      if (orient_steps(angleToStep(180), right_leg, backward, mot)) return general_error;
      //      if (pr_telega.RotateStpsOnly(*motorLink,  pr_telega.DriveLeft())) return general_error;
    }
    mot.SetStepsDepthInSteps(oldStepDepth);
  }
  else {
    //   fOtladkaMes(static_cast<String>(raznica_v_polozhenii) + " = " + static_cast<String>(var.StepDepth));
    delay(5000);
  }
  return 0;
}


// Поднять левую ногу (если смотреть спереди) на ступеньку когда две ноги на земле

byte StepUpWhenBothStepsTogether(posOfMotors & mot, ledder_var& var)  // BbIcoTa CTYnEHbKU, mm
{

  //  —————T
  //  |	 |
  //  –  |
  //	 |
  //     -
  // передвинуть телегу вправо(если смотреть)
  if (moveMassRight(mot)) return general_error;

  if (Leg_pfn(left_leg, energySaving, vtianut, mot, var.StepDepth + var.DeltaStep)) return general_error;
  /*
  	|---------T)
  	V
  */

  if (orient_steps(angleToStep(var.angle) , right_leg, forward, mot)) return general_error;
  // опускаем ногу на 30 мм
  if (OpustytLevuyNoguNa(vytianut, mot, var.DeltaStep)) return general_error;

  if (pr_telega.RotateStpsOnly(*motorLink,  pr_telega.DriveLeft())) return general_error;
  return 0;
}






// какая нога выше, какая ниже
long fRaznost_v_polozhenii(posOfMotors & mot)
{
  long raznica_v_polozhenii = mot.GetLeftLegCurrentSteps() - mot.GetRightLegCurrentSteps();
  //  Serial1.println("Raznitsa:" + String(raznica_v_polozhenii));
  return raznica_v_polozhenii;
}




byte goDnstears(posOfMotors & mot, step_dir dir, byte steps_height, long zapas_na_povorot, byte steps_count)
{
  /*
    if ((steps_height < 0) || (steps_height > (8 * 25)))
    {
      fErrorMes("DepthError");
      delay(1000);
      return general_error;
    }
  */
  seetUpDown(mot, 1);  // привстаем

  moveMassLeft(mot);
  // втянем правую
  mot.MoveRightLegCurrentSteps(-stepsPerLegRot * 15L / lengthPerLegRot, BTANYTb_CH);
  if (fAnswerWait(right_leg, knee, mot, 'T', BTANYTb_CH)) return general_error;
  // повернем стопу до концевика
  delay(pauseForSerial);
  SerR.prepareMessage('g');
  if (fAnswerWait(right_leg, zero, mot)) return general_error;
  // повернем правую стопу на 22 градуса
  foot_rotation(right_leg, 1, 22, 1, mot);
  // опускаем
  mot.MoveRightLegCurrentSteps(0, BbITANYTb_CH);
  if (fAnswerWait(right_leg, knee, mot, 'T', BbITANYTb_CH)) return general_error;

  delay(pauseForSerial);
  moveMassRight(mot);

  // тоже самое для левой ноги
  mot.MoveLeftLegCurrentSteps(-stepsPerLegRot * 15L / lengthPerLegRot, BTANYTb_CH);
  if (fAnswerWait(left_leg, knee, mot, 'h', 'T')) return general_error;
  delay(pauseForSerial);
  SerL.prepareMessage('g');
  if (fAnswerWait(left_leg, zero, mot)) return general_error;
  foot_rotation(left_leg, 1, 22, 1, mot);
  mot.MoveLeftLegCurrentSteps(0, BbITANYTb_CH);
  if (fAnswerWait(left_leg, knee, mot, 'T', BbITANYTb_CH)) return general_error;



  // делаем шаги вверх по ступенке 3 раза
  if (dnStep(mot, dir, steps_height, zapas_na_povorot, steps_count)) {
    fErrorMes("CantStepDn");
    delay(4000);
  }
  //  seetUpDown(mot, 1);  // привстаем

  return 0;
  /*
    // допустим стоим впритык к лестнице, тогда первый шаг 90 градусов
    // поворот вправо вперед
    // поднимаем ногу на высоту ступени + запас в 30 мм
    // поворачиваемся на 90 гдрадусов
    // опускаем ногу на ступеньку на 30 мм
  */
}


// одно движение ногой вверх для подъема по ступенькам
byte dnStep(posOfMotors & mot, step_dir dir,        // FF\BK
            byte BbIcoTa_CTYnEHbKU, long zapas_na_povorot, byte stpsCnt)   // BbIcoTa CTYnEHbKU, mm
{
  //  long zapas_na_povorot = 5L;
  short angle = 90 - 15;
  long StepDepth = stepDephCalc(BbIcoTa_CTYnEHbKU);
  long DeltaStep = stepDephCalc(zapas_na_povorot);

  long old_step_depth = mot.GetStepsDepthInSteps();

  if (fstandStill(mot))
  {
    mot.SetStepsDepthInSteps(0);
    if (StepDnWhenBothStepsTogether(mot, dir, angle, StepDepth, DeltaStep)) return general_error;
    if (stepDnOneStep(mot, dir, StepDepth, DeltaStep, left_leg)) return general_error;
    if (stepDnOneStep(mot, dir, StepDepth, DeltaStep, right_leg)) return general_error;
    if (stepDnOneStepAlg2(mot, dir, StepDepth, DeltaStep, left_leg)) return general_error;
    mot.SetStepsDepthInSteps(old_step_depth);
    // 1.2. Если стоим на противоположной ноге, то
    // 1.2.1 проверяем, поднята ли одна нога выше другой на величину ступеньки
  } else {
    for (int i = 0; i < stpsCnt - 1; ++i)
    {
      //      stepDnOneStep(mot, dir, angle,  StepDepth, DeltaStep);
    }
    // делаем последний шаг чтобы встать обеими ногами на ступеньки
    //    stepDnOneStep(mot, dir, 0,  StepDepth, DeltaStep);
    //   StepsManage(mot, energySaving);
  }

  // 3. Если стояли на обоих ногах то поворачиваемся на 90 градусов
  // иначе на 180.
  return 0;
}

// Поднять левую ногу (если смотреть спереди) на ступеньку когда две ноги на земле

byte StepDnWhenBothStepsTogether(posOfMotors & mot, step_dir dir,
                                 short angle,
                                 long StepDepth,  // BbIcoTa CTYnEHbKU, mm
                                 long DeltaStep)
{

  //  —————T
  //  |	 |
  //  –  |
  //	 |
  //     -

  //  if (moveMassRight(mot)) return general_error;
  if (moveMassLeft(mot)) return general_error;
  // приподнять правую ногу для проноса над ступенькой
  if (Leg_pfn(right_leg, energySaving, vtianut, mot, DeltaStep)) return general_error;
  //  if (LegLeftMoveTo(mot, -DeltaStep, BTANYTb_CH)) return general_error;
  /*
  	|---------T)
  	V
  */
  // повернуться на левой ноге
  if (orient_steps(angleToStep(angle) , left_leg, dir, mot)) return general_error;
  // поставить ногу на ступеньку
  //  if (LegsMoveTo(mot, 0, BbITANYTb_CH, -StepDepth, BTANYTb_CH)) return general_error;
  if (LegsMoveTo(mot,  -StepDepth, BTANYTb_CH, 0, BbITANYTb_CH)) return general_error;
  // левую ногу полностью вытягиваем, правую втягиваем
  if (pr_telega.RotateStpsOnly(*motorLink,  pr_telega.DriveRight())) return general_error;
  return 0;
}



byte stepDnOneStep(posOfMotors & mot, step_dir dir,
                   long BbIcoTa_CTYnEHbKU,
                   long leg_up_for_rotate, robot_leg legUp)
{
  if (fstandStill(mot))
  {
    fErrorMes("StpDnError");
    return general_error;
  }
  long raznost_v_polozhenii = fRaznost_v_polozhenii(mot);

  // Если разность в положении равна высоте ступеньки
  if (abs(raznost_v_polozhenii) == BbIcoTa_CTYnEHbKU)
  {
    // robot_leg legDn, legRot;    //  calcLegForStep(raznost_v_polozhenii, legRot, legUp,  legDn);
    /*
        robot_leg legUp = raznost_v_polozhenii > 0 ?
                          right_leg :
                          raznost_v_polozhenii < 0 ?
                          left_leg : legUp;
    */
    //  1.2.2 перевозим тележку
    //  если нужно поднять левую ногу, то телегу двиаем вправо
    if (legUp == left_leg)
    {
      if (pr_telega.RotateStpsOnly(*motorLink,  pr_telega.DriveRight())) return general_error;
      if (ZadatPolozhenieLevoy(mot, -(BbIcoTa_CTYnEHbKU + leg_up_for_rotate), BTANYTb_CH)) return general_error;
      if (orient_steps(angleToStep(90 - 15), right_leg, dir, mot)) return general_error;
      if (LegsMoveTo(mot, 0, BbITANYTb_CH, -BbIcoTa_CTYnEHbKU, BTANYTb_CH)) return general_error;
      if (pr_telega.RotateStpsOnly(*motorLink,  pr_telega.DriveLeft()))  return general_error;
    }
    if (legUp == right_leg)
    {
      if (pr_telega.RotateStpsOnly(*motorLink,  pr_telega.DriveLeft())) return general_error;
      if (ZadatPolozheniePravoy(mot, -(BbIcoTa_CTYnEHbKU + leg_up_for_rotate), BTANYTb_CH)) return general_error;
      if (orient_steps(angleToStep(90 - 15), left_leg, dir, mot)) return general_error;
      if (LegsMoveTo(mot, -BbIcoTa_CTYnEHbKU, BTANYTb_CH, 0, BbITANYTb_CH)) return general_error;
      if (pr_telega.RotateStpsOnly(*motorLink,  pr_telega.DriveRight())) return general_error;
    }
  }
  // else Serial1.println(static_cast<String>(raznost_v_polozhenii) + " = " + static_cast<String>(stepDephCalc(BbIcoTa_CTYnEHbKU)));

  return 0;
}


// поседний полушаг
byte stepDnOneStepAlg2(posOfMotors & mot, step_dir dir, long BbIcoTa_CTYnEHbKU, long leg_up_for_rotate, robot_leg legUp)
{
  if (fstandStill(mot))
  {
    fErrorMes("StpDnError");
    return general_error;
  }
  long raznost_v_polozhenii = fRaznost_v_polozhenii(mot);

  // Если разность в положении равна высоте ступеньки
  if (abs(raznost_v_polozhenii) == BbIcoTa_CTYnEHbKU)
  {

    //  1.2.2 перевозим тележку
    //  если нужно поднять левую ногу, то телегу двиаем вправо
    if (legUp == left_leg)
    {
      if (pr_telega.RotateStpsOnly(*motorLink,  pr_telega.DriveRight())) return general_error;
      if (ZadatPolozhenieLevoy(mot, -(BbIcoTa_CTYnEHbKU + leg_up_for_rotate), BTANYTb_CH)) return general_error;
      //	  mot.SetStepsDepthInSteps();
      if (orient_steps(angleToStep(0), right_leg, dir, mot)) return general_error;
      //      if (LegsMoveTo(mot, 0, BbITANYTb_CH, -BbIcoTa_CTYnEHbKU, BTANYTb_CH)) return general_error;
      if (ZadatPolozhenieLevoy(mot, 0, BbITANYTb_CH)) return general_error;
      if (pr_telega.RotateStpsOnly(*motorLink,  pr_telega.DriveLeft()))  return general_error;
    }
    if (legUp == right_leg)
    {
      if (pr_telega.RotateStpsOnly(*motorLink,  pr_telega.DriveLeft())) return general_error;
      if (ZadatPolozheniePravoy(mot, -(BbIcoTa_CTYnEHbKU + leg_up_for_rotate), BTANYTb_CH)) return general_error;
      if (orient_steps(angleToStep(0), left_leg, dir, mot)) return general_error;
      //      if (LegsMoveTo(mot, -BbIcoTa_CTYnEHbKU, BTANYTb_CH, 0, BbITANYTb_CH)) return general_error;
      if (ZadatPolozheniePravoy(mot, 0, BbITANYTb_CH)) return general_error;
      if (pr_telega.RotateStpsOnly(*motorLink,  pr_telega.DriveRight())) return general_error;
    }
  }
  // else Serial1.println(static_cast<String>(raznost_v_polozhenii) + " = " + static_cast<String>(stepDephCalc(BbIcoTa_CTYnEHbKU)));

  return 0;
}


void dataToUpload()
{
  char buff[15];
  char pause = 10;

  readString(VtagSpeedAddr, buff);
  CheckUpMes("L_VtagSpd");
  buff[0] = '\0';
  delay(pause);

  CheckUpMes(buff);
  buff[0] = '\0';
  //  buff = "";
  delay(pause);

  readString(VytagSpeedAddr, buff);
  CheckUpMes(buff);
  buff[0] = '\0';
  //  buff = "";
  delay(pause);

  readString(VtagAccelAddr, buff);
  CheckUpMes(buff);
  buff[0] = '\0';
  delay(pause);

  readString(VytagAccelAddr, buff);
  CheckUpMes(buff);
  delay(pause);

  readString(FootRotSpeedAddr, buff);
  CheckUpMes(buff);
  delay(pause);

  readString(FootRotAccelAddr, buff);
  CheckUpMes(buff);
  delay(pause);
  /*
    buff = String(minRbtStep);
    CheckUpMes(buff);
    buff = "";
    delay(pause);
  */
  readString(TelegaSpeedAddr, buff);
  CheckUpMes(buff);
  //  buff = "";
  delay(pause);

  readString(TelegaAccelAddr, buff);
  CheckUpMes(buff);
  //  buff = "";
  delay(pause);
}



// Изменения режима хотьбы быстрый или энергоэффективный
// b = 0 - изменить текущий режим работы на противолополжный
// b = 1 - изменить на режим энергосбережения
// b = 2 - изменить на режим быстрый
void change_mode(regimRaboty &mode, byte b)
{
  if (b == 0)
  {
    if (mode == fast)
    {
//      fOtladkaMes("EnergySaveMode");
      mode = energySaving;
      // для обратной связи
      CheckUpMes("menergySaving");
//      fAddInActionInRecordMode(goSlow);
    }
    else
    {
//      fOtladkaMes("FastMode");
      mode = fast;
      // для обратной связи
      CheckUpMes("mFastMode");
    }
    fAddInActionInRecordMode(goFast);
  }
  else if (b == 1)
  {
	  mode = energySaving;
      CheckUpMes("menergySaving");
      fAddInActionInRecordMode(goSlow);
  }
  else if (b == 2)
  {
	  mode = fast;
      CheckUpMes("mFastMode");
      fAddInActionInRecordMode(goFast);
  }
}

bool composeNumCmdCode(char cmd[], char* num)
{
  if((num[0]<'0')||(num[0]>'9')){ fErrorMes("Composer:CmdCodeErr"); return 1;}
  if((num[1]<'0')||(num[1]>'9')){ fErrorMes("Composer:CmdCodeErr"); return 1;}
  cmd[1] = num[0];
  cmd[2] = num[1];
  return 0;
}


long setWalkAngleCmd(char* num, unsigned short angle)
{
  if(!composeNumCmdCode(currStpAngl, num)) return angleToStep(angle);
  return 0;
}
