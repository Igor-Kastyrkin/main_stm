//

/* глюки
  1. После нажатие кнопки опускания ноги, начинается перекатывание тележки 3 раза
  2. При смене направления движения, если тележка переехала, то перед отправкой телеги обратно,
  происходит подъем и опускание ноги
  3. Кнопка влево на самом деле поворачивает робота вправо и наоборот
  4. После команды поворота на право или на лево, иногда не с первого шага он начинает перекатывать телегу туда сюда.
  5. После нажатия назад, он делает шаг вперед, потом топчится на месте с перекатыванием телеги, и только потом делает шаг назад
  6. Если остановить телегу до того, как он ее перевел, потом он делает лишние перекатывания телеги.
  7. Если телега слева, если смотреть сзади и остановить движение на двух ногах с перекатыванием телеги после остановки, то следующее движение тоже
  назад, телега опять перекатывается и делает много лишних движений
  8. Не срабатывает стоп, котогда идет назад, телега перекатывается влево, если смотреть сзади.
  9. Если идти назад и телега находится слева, то если нажимать стоп каждый раз, после перекатывания телеги, то он делает как на видео.
  После каждой остановки, при нажатии назад, стоит на месте.

  10. Телега слева, я нажимаю вперед, она едет вправо, поднимает ногу, я нажимаю стоп потом опять вперед, она опускает левую ногу - стоп - вперед - телега влево - вперед - телега вправо и тд



  Из видео:
  1. После опускания ноги, нажал назад. Он сделал цикл из перемещений тележки
  2. потом вперед и пошел вперед потом опять опустил и стал перемещать туда сюда.

  "/
  // Задачи:
  /*
  1. Наладить обмен между ногами при включении.
  2. Сделать калибровку по времени
  3. Проверить есть ли 1ком подтяжек на +3 и землю.
  8. После того, как шли вперед, начинаем идте назад, происходят много лишних движений
  13.Добавить функцию answerwait для двух ног одновременно
  14.Сообщения в ПО во время движения
  15.Тестировать рс485 на разницу в миливольтах с подтяжками на холостом и с уровнями
  16.Ввести в луп обнуление оприентировки rbOrient
  17.Заряжать АКБ 14,3 Вольтами или 43 вольтами 3 аккума или 41 В
  18.После нажатия конца воспроизведения, кнопка стоп тоже должна нажиматься сама
  19.В функции стартплей и других местах подумать как переделать типы в стринг и отправлять
   порцию данных с обязательным символом конца строки \r\n
  20.Продумать чтобы точно при записи комментарии не могли вылезти, а в других случаях пусть будут.
  ---------------------------------------------------------
  25.11.19
  1. Приделал калибровку
  2. Убрал проверку готовности ног
  3. Добавлены отладочные сообщения
  4. Вернул подтверждение готовности ног
  5. В экономном режиме добавлен флаг втягивания
  6. Добавил сохранение из fbreak данных положения ног и убрал сохранение из fAnswerWait
  27.11.19
  1. Переписал класс UART для безошибочноый работы
  2. Отключаемый светодиод
  3. Возможность отключить одну левую ногу
  28.11.19
  1. Убрал почти все отладочные сообщения в теле доступа к нрф
  2. Изменил чтение из НРФ. Вместо rf_data исправил на &RF_data
  3. Увеличил интервал delay_nrf_pause с 50 до 200 мс
  4. Добавил выход по времени из ожидания ответа
  5. Сдалал защиту от случайного срабатывания DigitalRead на телеге
  29.11.19
  1. Убрал break вообще из тележки
  2. Исправил учет направления движения тележки
  3. Добавил в функцию возможность менять направление хотьбы
  4. Добавил функциональности кнопки назад и вперед
  5. Работают все кнопки в приложении.
  6. Исправил неверное определение положения на какой либо ноге
  2.12.19
  1. Добился работоспособности для хождения.
  2. Добавил скорости и ускорения
  3. Сделал возможность ходить на полусогнутых ногах
  4. Увеличил диапазон подъема ноги
  6.12.19
  1. В break сделал отправку количества пройденных шагов
  2. В loop начал делать разные режимы work, play, rec
  19.12.19
  1. Если телега справа то идти начинаю с левой ноги
  2. Теперь вначале идет число, а потом команда в случае записи
  или воспроизвденеия.
  20.12.19
  1. Усовершенствовал алгоритм записи в файл
  2. Ввод скорости и ускорения поворота ноги с клавиатуры.
  21.12.19
  1. Исправил зависание, когда останавливаю его на одной ноге
  2. Устранил задержку при калибровке.
    9.12.19
  1. При смене направления хождения на противоположный не делает лишних движений
  2. Высокие/низкие уровни концевиков задаются отдельно через дефайн
  10.12.19
  1. Добавил класс privod
  20.1.20
  1. РХ пины подтягиваю к +3 волт
  2. Теперь обработка ожидания ответа может происходить одновременно для двух ног.
  23.1.20
  1. В модуле НРФ добавил конец строки, и добавил убирание лишних нулей в посылке в консольном приложении
  чтобы сообщение с НРФ читалось корректно
  2. Убрал все предупреждения
  3. Избавился от некорых глобальных переменных
  4. Подчистил закоментированные строки
  3.3.20
  1. Исправил StWork = play. Убрал это условие в самой функции и перенес его
  непосредственно перед вызовом этой функции в лууп.
  6.3.20
  1. Добавил отдельную функцию загрузки данных в МК
  2. Исправил заполнение массива загруженными данными только до последнего принятого символа.


  При  отправке драйвер виндоуз может глючить и отправлять по частям команду
  А при приеме он всегда принимает по частям, какую часть и по сколько примет угадать сложно.

  Обратить внимание как выполеняется чтение из флешки,
  как выводит координату полученную от ноги особенно когда она зависла
  почему-то вылязиют лишние пробелы
  как передаются углы больше 90 градусов
  сделать чтобы при старте чтобы обнулялся текущий угол а углы измерять прямо в приложении



telega
PlayModeMaxStepsReached_
<<_
maxStepsCount=5_
<<_
backward_
RIGHT FOOT BLOCKED
LeftLegFF
1666_
>_
-1666
*/

//  примечание: вправо это назад для обеих ног.

#include <stdlib.h>

#define _STEPPER_
#include <AccelStepper.h>
#include "UART.h"
#include "proc.h"

#include "privod.h"

//#define _OTLADKA_

#define _EPROM1_

#ifdef _EPROM1_
#include <Wire.h>
#define disk1 0x50
#include "Eeprom.h"
// #define _TEL_EEPROM_

#endif


#include <LiquidCrystal_I2C_STM32.h>



#define _CHKDATA_         // проверка обмена с ногами
#define _BUILTIN_LED_ON_  // отключение светодиода
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
long ledPinTimer = 0;
bool ledPinActive = 0;
#endif
#define _TIHOHOD_

short m_stp = 2;


leedLeg leed_leg = noLeed;

long LeftLegCurrentSteps   = 0;
long LeftFootCurrentSteps  = 0;
long RightLegCurrentSteps  = 0;
long RightFootCurrentSteps = 0;
long CurrentZero = 0;
byte stepDepth = 15; // мм
const short stepsPerLegRot    = 800;//3200; // количество шагов на оборот
const byte lengthPerLegRot   = 4; // за один оборот винта нога перемещается на 8мм
const long fullRotationLimit = 22;
long stepsDepthInSteps = (long)stepsPerLegRot * (long)stepDepth / (long)lengthPerLegRot;  //12800

long rbOrient  = 0;  // угол направления движения в градусах
long oldOrient = 0;

//long stepAngle = calc_angle(10, 2); //1111;//555.5555556;//2222.2222;// 10 град, 2 микрошаг

LiquidCrystal_I2C_STM32 lcd(0x3F, 16, 2);

posOfMotors drvPos;

const byte Motor_number = 1;

bool HalfPos = 1;

bool LoopPlay = 0;

bool withPauses = 1;

byte minRbtStep = 10;

long recordTimer = 0;
long StartRecordTime = 0;
long playTimer = 0;

#ifdef _EPROM1_
const unsigned long StartEEPROM = 10;
unsigned long StrAddr = StartEEPROM;
const byte incriment = 10;

#ifdef _TEL_EEPROM_
const unsigned long StartTelEEPROM = 510;
unsigned long StrTelAddr = StartTelEEPROM;
const byte TelIncriment = 30;
#endif


// Используется при записи как значения по умолчанию
String currSpeed = "S2";
String currAccel = "A2";
String currStpDpth = "K29";
String currStpAngl = "K22";
String currRbStps  = "d65";
String currMinStepRot = "m" + String(minRbtStep);

#endif
//byte nul_data = 3, one_data = 7, two_data = 11, three_data = 15;//, four_data = 19, six_data = 27;

const unsigned long Com1Rate = 115200L;
const unsigned long Com2Rate = 115200L;




rot_dir directAngle;
robot_leg rLeg;
robot_leg old_leg = rLeg;
regimRaboty mode = fast;
leg_dir LegUpL = vtianut, LegUpR = vtianut;
MKmotor uzel;
//dirflg dir_flg = middle;
//step_dir StpDir = forward;
StadyWork stWork = StWork;
actions Actions  = standStil;
actions oldActions = Actions;

unsigned short maxStepsCount = 0;

unsigned long parkingTimer;

char **work_data;
short fLength = 0;

//RF24 radio1(CE_PIN, CS_PIN);
//RF24& radio = radio1;
#ifdef _NRF24_
RF24 radio(CE_PIN, CS_PIN);
#endif

#ifdef _STEPPER_
AccelStepper* motorLink[Motor_number];
#endif

const int baudRateCom1 = 9600;

void setup()
{
#ifdef _EPROM1_
  Wire.begin();
#endif
  lcd.begin();
  lcd.backlight();
  lcd.print("Loading...");
 // char cmdL = 0x0, cmdR = 0x0;
  pinMode(ENDCAP, INPUT_PULLUP);
  delay(50);
  pinMode(ENDCAP_R, INPUT_PULLUP);
  delay(50); // +100
#ifdef _BUILTIN_LED_ON_
  pinMode(LED_BUILTIN, OUTPUT);
#endif
  //  pinMode(CS_PIN, OUTPUT);
  //  digitalWrite(CS_PIN, LOW);
  delay(200); // +200
  motorLink[0] = new AccelStepper(AccelStepper::DRIVER, StepPin, DirPin);

  delay(3400); // +3400

  pr_telega.privod_init(ENDCAP, ENDCAP_R);

// по результатам испытаний немного подразогнали
// скорость была 9200, ускорение 10000

  float telegaSpeed = 10000.0;
  float telegaAccel = 10500.0; //8000

  pr_telega.setTelegaSpec(telegaSpeed, telegaAccel);
  motorLink[0]->setMinPulseWidth(20);

  Serial1.begin(baudRateCom1);

  SerL.setComRate(Com1Rate, RTS_L);
  //  pinMode(SerL_RX_PIN, INPUT_PULLUP);
  delay(100);
  SerR.setComRate(Com2Rate, RTS_R);
  //  pinMode(SerR_RX_PIN, INPUT_PULLUP);
  delay(100);
#ifdef SERIAL_USB
  Serial2.begin(Com1Rate);
  delay(100);
  Serial3.begin(Com2Rate);
#endif
  Serial1.println("Loading...");
  delay(10);
#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, HIGH);
#endif

#ifdef _CHKDATA_
 // long waitTimer;
#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, LOW);
#endif
  delay(6000); // +4000
  // очистка буфера от возможного мусора
  while ((SerL.handle_serial() < 0) ||
         (SerR.handle_serial() < 0)) {};
  while (Serial1.available()) {
    Serial1.read();
    delay(1);
  };
  delay(10);
  //  SerL.prepareMessage('s');
  //  delay(10);
  //  SerR.prepareMessage('s');
  // waitTimer = millis();
  /*
    while ((cmdL != 's') || (cmdR != 's'))
    {
      delay(10);
      if (SerL.handle_serial() > 0)
        SerL.getString0(cmdL);

      if (SerR.handle_serial() > 0)
        SerR.getString0(cmdR);

      if (millis() - waitTimer > 3000)
      {
        if (cmdL != 's')
        {
          fErrorMes("Left timeout");

          delay(2000);
        }
        if (cmdR != 's')
        {
          fErrorMes("Right timeout");

          delay(2000);
        }
        break;
      }
    }
  */
#endif
#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, HIGH);
#endif

  fCalibration(SerL, SerR, drvPos);
  fErrorMes("Starting-working");
  /*
    #ifdef SERIAL_USB
    Serial1.print("Starting working\r\n");
    #else
    Serial.print("Starting working\r\n");
    #endif
  */
}  /*-------------------// END SETUP //---------------------*/

// --ГЛАВНЫЙ ЦИКЛ ПРОГРАММЫ!!!--
void loop()
{
//  byte sizeOfRFdata = 60;
  //  char buff[sizeOfRFdata - 1];
  bool RadioAval = 0;
  char RF_data[60] = {'~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '\0'};
  byte serialAmount = 0;
  char tmp;
#ifdef _BUILTIN_LED_ON_
  if (((millis() - ledPinTimer) > 500) && ledPinActive)
  {
    ledPinActive = 0;
    digitalWrite(LED_BUILTIN, HIGH);
  } // через заданное время отключаем зеленый светодиод
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

    while (Serial1.available() && (serialAmount < 30))
    {
      RF_data[serialAmount] = Serial1.read();
      serialAmount++;
      delayMicroseconds((10000000 / baudRateCom1) + 500);  //23.7.20 было +50мкс
    }
    RF_data[serialAmount] = '\0';
    while (Serial1.available())
    {
      Serial1.read();
      delay(2);
      RadioAval = 0;
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
        PlayFromEEPROM(LoopPlay);
        stWork == StWork ? stWork = StPlay : stWork = StWork; //10.7.2020
        RadioAval = 0;
        //     ledPinActive = 0;
      }
    }
    //   delay(100);
  }

  if (RadioAval == 1)
  {
    fOtladkaMes(">>");
    RF_messege_handle(RF_data, drvPos);
  } // radio.available
  if(stWork != StPlay)
	  while ((SerL.handle_serial() < 0) || (SerR.handle_serial() < 0)) {};

  //  StepsManage();
} // ------- LOOP --------

byte RF_messege_handle(char *RF_data, posOfMotors & mot)
{
  static long stepAngle = calc_angle(30, m_stp); // +++++
  short m_stp = 2;
  //  fOtladkaMes("Radio_in: "); //    for (unsigned int i = 0; i < sizeOfRFdata; i++) Serial.print((char)RF_data[i]);
  // delay(9);
  if ((RF_data[0] == 'W') && (RF_data[1] == 'a') && (RF_data[2] == 'k') && (RF_data[3] == 'e'))
  {
    fErrorMes("Chek-CS-PIN-on-JDY-40");
    return 0;
  }
  char cmdR = RF_data[0];

  byte D_Len = strlen(RF_data);
  if (D_Len >= 30)
  {
    fOtladkaMes("ShouldBeMesError" + String(D_Len));
    return 0;
  }
  char cpyData[D_Len + 1];
  for (int f = 0; f < (D_Len - 1) ; f++)
  {
    cpyData[f] = RF_data[f + 1];
  }

  for (int f = 0; f < (D_Len - 1) ; f++)
  {
    RF_data[f] = cpyData[f];
  }

  RF_data[D_Len - 1] = 0;
  //  fOtladkaMes(RF_data);

  short x = atoi(&RF_data[0]);
  long x_l = atol(&RF_data[0]);
  /*
    #ifdef SERIAL_USB
    Serial1.print(RF_data[0]);
    Serial1.print(x);
    Serial1.print("\r\n");
    #else
    Serial.print(RF_data[0]);
    Serial.print(x);
    Serial.print("\r\n");
    #endif
  */
  // char tmp2[] = "Cen\n";

  switch (cmdR)
  {
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // не хватает команды при принятии которой, выполнялось бы заданное количество шагов!!!!!!!!!!!!!!!!!!!
    case 'K':
      switch (x)
      {
        case 10: // stop
          fOtladkaMes("Stop");
          break;
        case 13:
          //		  String OtlMes = "Starting mooving backward"
          fOtladkaMes("backward");
          //		  fOtladkaMes("maxStepsCount = ");
          //		  fOtladkaMes(maxStepsCount);
          //		  fOtladkaMes("\r\n");
          walkBackward(stepAngle,mot);
          break;
        case 14: //forward шагаем
          fOtladkaMes("forward");
          //		  fOtladkaMes("maxStepsCount = ");
          //		  fOtladkaMes(maxStepsCount);
          //		  fOtladkaMes("\r\n");
          walkForward(stepAngle,mot);
          break;
        case 19: // telega vpravo
          telega_right();
          break;
        case 17: // telega vlevo
          telega_left();
          break;
        case 18: // telega v centr
          telega_to_center();
          break;
        case 15:  // выдвинуть левую (если смотреть спереди робота против его хода)
          topLeft(mot);
          break;
        case 16:  // выдвинуть правую (если смотреть спереди робота против его хода)
          topRight(mot);
          break;
        case 39: // калибровка
          fCalibration(SerL, SerR, mot);
          break;
        case 20:
          fOtladkaMes("10 Degree");
          currStpAngl[1] = '2'; currStpAngl[2] = '0';
          stepAngle = calc_angle(10, m_stp); //4444L; // 10 град
          break;
        case 21:
          fOtladkaMes("20 Degree");
          currStpAngl[1] = '2'; currStpAngl[2] = '1';
          stepAngle = calc_angle(20, m_stp);
          break;
        case 22:
          fOtladkaMes("30 Degree");
          currStpAngl[1] = '2'; currStpAngl[2] = '2';
          stepAngle = calc_angle(30, m_stp);
          break;
        case 23:
          fOtladkaMes("45 Degree");
          currStpAngl[1] = '2'; currStpAngl[2] = '3';
          stepAngle = calc_angle(45, m_stp);  // 45 град
          break;
        case 24:
          fOtladkaMes("60 Degree");
          currStpAngl[1] = '2'; currStpAngl[2] = '4';
          stepAngle = calc_angle(60, m_stp); // 60 град
          break;
        case 25:
          fOtladkaMes("70 Degree");
          currStpAngl[1] = '2'; currStpAngl[2] = '5';
          stepAngle = calc_angle(70, m_stp); // 70 град
          break;
        case 26:
          fOtladkaMes("80 Degree");
          currStpAngl[1] = '2'; currStpAngl[2] = '6';
          stepAngle = calc_angle(80, m_stp);
          break;
        case 27:
          fOtladkaMes("90 Degree");
          currStpAngl[1] = '2'; currStpAngl[2] = '7';
          stepAngle = calc_angle(90, m_stp); // 90 град
          break;
        case 28: // 6000
          fOtladkaMes("20 mm");
          currStpDpth[0] = 'K'; currStpDpth[1] = '2'; currStpDpth[2] = '8';
          stepsDepthInSteps = stepsPerLegRot * 10L / lengthPerLegRot;
          break;
        case 29: // 12000
          fOtladkaMes("30 mm");
          currStpDpth[0] = 'K'; currStpDpth[1] = '2'; currStpDpth[2] = '9';
          stepsDepthInSteps = stepsPerLegRot * 15L / lengthPerLegRot;
          break;
        case 30: // 18000
          fOtladkaMes("50 mm");
          currStpDpth[0] = 'K'; currStpDpth[1] = '3'; currStpDpth[2] = '0';
          stepsDepthInSteps = stepsPerLegRot * 25L / lengthPerLegRot;
          break;
        case 31: // 24000
          fOtladkaMes("80 mm");
          currStpDpth[0] = 'K'; currStpDpth[1] = '3'; currStpDpth[2] = '1';
          stepsDepthInSteps = stepsPerLegRot * 40L / lengthPerLegRot;
          break;
        case 32:
          fOtladkaMes("120 mm");
          currStpDpth[0] = 'K'; currStpDpth[1] = '3'; currStpDpth[2] = '2';
          stepsDepthInSteps = stepsPerLegRot * 60L / lengthPerLegRot;
          break;
        case 33: //36000
          fOtladkaMes("160 mm");
          currStpDpth[0] = 'K'; currStpDpth[1] = '3'; currStpDpth[2] = '3';
          stepsDepthInSteps = stepsPerLegRot * 80L / lengthPerLegRot;
          break;
        case 36:
          fOtladkaMes("180 mm");
          currStpDpth[0] = 'K'; currStpDpth[1] = '3'; currStpDpth[2] = '6';
          stepsDepthInSteps = stepsPerLegRot * 90L / lengthPerLegRot;
          break;
        case 37:
          fOtladkaMes("220 mm");
          currStpDpth[0] = 'K'; currStpDpth[1] = '3'; currStpDpth[2] = '7';
          stepsDepthInSteps = stepsPerLegRot * 110L / lengthPerLegRot;
          break;
        case 38:
          fOtladkaMes("360 mm");
          currStpDpth[0] = 'K'; currStpDpth[1] = '3'; currStpDpth[2] = '8';
          stepsDepthInSteps = stepsPerLegRot * 180L / lengthPerLegRot;
          break;
  	    case 34:
		  if(stWork == StPlay)
		  {
             mode = energySaving; 
		  }
		  break;
        case 35:
		  if(stWork == StPlay)
		  {
             mode = fast; 
		  }else{
            if (mode == fast)
            {
              fOtladkaMes("EnergySaveMode");
              mode = energySaving;
	  		fAddInActionInRecordMode(goSlow);
            }
            else
            {
              fOtladkaMes("FastMode");
              mode = fast;
		      fAddInActionInRecordMode(goFast);
            }
		  }
          break;
        case 11: // поворот влево
          //          change_orient(leftA, mot,minRbtStep);

          fStepFf(leed_leg, -minRbtStep);
          break;
        case 12: // поворот вправо (это назад идти)
          //          change_orient(rightA, mot,minRbtStep);

          fStepFf(leed_leg, minRbtStep);
          break;
        case 40:
          seetUpDown(mot);
          break;
        case 41: //   вытягиваем и втягиваем правую чтобы разработать
          while (1)
          {
            if (fBreak(right_leg, foot, mot)) break;
            RightLegCurrentSteps = - stepsPerLegRot * fullRotationLimit;
            SerR.prepareMessage( 'h', RightLegCurrentSteps); // вытягиваем
            if (fAnswerWait(right_leg, knee, mot)) break;

            RightLegCurrentSteps = 0L;
            SerR.prepareMessage( 'i', RightLegCurrentSteps); // втягиваем
            if (fAnswerWait(right_leg, knee, mot)) break;
          }
          break;
        case 42: //запись
          if (stWork == StPlay)
          {
            fErrorMes("ErrorRecInPlayMode");
            return 1;
          }
          stWork == StWork ? stWork = StRec : stWork = StWork;
          // записать в память скорости и ускорения
#ifdef _EPROM1_
          if (stWork != StRec)
          {
            fOtladkaMes("Record Off");
            recordTimer = 0;
          }
          if (stWork == StRec)
          {
            StartRecordTime = millis();
            fOtladkaMes("Rec");
            //           fOtladkaMes("StrAdr1 = " + String(StrAddr));
            StrAddr = StartEEPROM;
            writeString(currSpeed);
            writeString(currAccel);
            writeString(currStpDpth);
            writeString(currStpAngl);
   //         writeString(currRbStps);
            writeString(currMinStepRot);
            //      fOtladkaMes(readString(StrAddr - incriment));
          }
          else writeString(String(StrAddr), 0);
#endif
          break;
        //      case 43: //воспроизведение
        //        if (stWork == StRec)
        //	  {
        //	    fErrorMes("ErrorPlayInRecMode");
        //	    return;
        //	  }
        // stWork == StWork ? stWork = StPlay : stWork = StWork;
        //	  fOtladkaMes("stWork = ");
        //	  fOtladkaMes(stWork);
        //	  fOtladkaMes("\r\n");
        //         break;
        case 44:
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
        case 45:
          fOtladkaMes("StandStill");
          StepsManage(mot);
          break;
        case 46:
          //покачаться
          break;
        case 50: // обработка команды загрузки данных из ПК
   //       fDload(); // функция загрузки
          break;
        case 51:
          demo1();
          break;
        case 52:
          demo2(mot);
          break;
        case 53:
          demo3(mot);
          break;
        case 54: // режим записи с запоминанием времени
          withPauses == 1 ? withPauses = 0 : withPauses = 1;
          break;
        // качаем один раз
        case 60:
          fShake(mot, 0);
		  fAddInActionInRecordMode(shakeOn);
          break;
        case 61: // переход в исходное после качания
          fShake(mot, 1);
  		  fAddInActionInRecordMode(shakeOff);
          break;
        case 62: // переход в исходное после качания
          fRoll(mot);
          break;
		case 63:  // быстрый поворот влево
		  break;
		case 64:  // быстрый поворот вправо
		  break;
		
      }
      break;
    // s, L, R, S, A, Y, W, d, m, D, J, K
    case 's':
      // Если не воспроизведение тогда выходим
      if (stWork != StPlay) break;
      // сохраняем параметр х
	  if(x<0) break;
      maxStepsCount = x;
      fOtladkaMes("maxStepsCount=" + String(maxStepsCount));
      break;
    case 'R': // влево с дошаганием
      oldOrient = rbOrient;
      fAddInActionInRecordMode(wait);
      rotPlace(mot, minRbtStep, x);
      // При смене ориентации не стоит отправлять об этом сообщение в ПК,
      // т.к. подтверждение этой операции не требуется
      delay(10);
      fOtladkaMes(String(rbOrient));
      delay(10);
      fAddInActionInRecordMode(turnL);

      break;
    case 'L': // вправо с дошаганием
      oldOrient = rbOrient;
      fAddInActionInRecordMode(wait);
      rotPlace(mot, minRbtStep, -x);
#ifndef _NRF24_
      delay(10);
      fOtladkaMes(String(rbOrient));
      delay(10);
#endif
      // При смене ориентации не стоит отправлять об этом сообщение в ПК,
      // т.к. подтверждение этой операции не требуется
      fAddInActionInRecordMode(turnR);
      break;
    case 'S': // задаем скорость для колен
      fOtladkaMes("SpeedLegs=" + String(x));
      //	    currSpeed[0] = 'S';
      if (x == 0)
      {
        currSpeed[1] = '0';
        SerL.prepareMessage( 'P', 23000 / 4); // скорость втягивания
        delay(15);
        SerR.prepareMessage( 'P', 23000 / 4);
        delay(15);
        SerL.prepareMessage( 'Q', 18000 / 4); // скорость вытягивания
        delay(15);
        SerR.prepareMessage( 'Q', 18000 / 4);
        delay(15);
        //     fOtladkaMes("0");
      }
      else if (x == 1)
      {
        currSpeed[1] = '1';
        SerL.prepareMessage( 'P', 20000 / 4);
        delay(15);
        SerR.prepareMessage( 'P', 20000 / 4);
        delay(15);
        SerL.prepareMessage( 'Q', 15000 / 4);
        delay(15);
        SerR.prepareMessage( 'Q', 15000 / 4);
        delay(15);
      }
      else if (x == 2)
      {
        currSpeed[1] = '2';
        SerL.prepareMessage( 'P', 18000 / 4);
        delay(15);
        SerR.prepareMessage( 'P', 18000 / 4);
        delay(15);
        SerL.prepareMessage( 'Q', 14000 / 4);
        delay(15);
        SerR.prepareMessage( 'Q', 14000 / 4);
        delay(15);
      }
      else if (x == 3)
      {
        currSpeed[1] = '3';
        SerL.prepareMessage( 'P', 16000 / 4);
        delay(15);
        SerR.prepareMessage( 'P', 16000 / 4);
        delay(15);
        SerL.prepareMessage( 'Q', 12000 / 4);
        delay(15);
        SerR.prepareMessage( 'Q', 12000 / 4);
        delay(15);
      }

      break;
    case 'A': // задаем ускорение для колен
      fOtladkaMes("AccelLegs=" + String(x));
      if (x == 0)
      {
        currAccel[1] = '0';
        SerL.prepareMessage( 'V', 25000 / 4);
        delay(15);
        SerR.prepareMessage( 'V', 25000 / 4);
        delay(15);
        SerL.prepareMessage( 'W', 35000 / 4);
        delay(15);
        SerR.prepareMessage( 'W', 35000 / 4);
        delay(15);
      }
      else if (x == 1)
      {
        currAccel[1] = '1';
        SerL.prepareMessage( 'V', 35000 / 4);
        delay(15);
        SerR.prepareMessage( 'V', 35000 / 4);
        delay(15);
        SerL.prepareMessage( 'W', 45000 / 4);
        delay(15);
        SerR.prepareMessage( 'W', 45000 / 4);
        delay(15);
      }
      else if (x == 2)
      {
        currAccel[1] = '2';
        SerL.prepareMessage( 'V', 20000 / 4);
        delay(15);
        SerR.prepareMessage( 'V', 20000 / 4);
        delay(15);
        SerL.prepareMessage( 'W', 20000 / 4);
        delay(15);
        SerR.prepareMessage( 'W', 20000 / 4);
        delay(15);
      }
      else if (x == 3)
      {
        currAccel[1] = '3';
        SerL.prepareMessage( 'V', 60000 / 4);
        delay(15);
        SerR.prepareMessage( 'V', 60000 / 4);
        delay(15);
        SerL.prepareMessage( 'W', 45000 / 4);
        delay(15);
        SerR.prepareMessage( 'W', 45000 / 4);
        delay(15);
      }
      break;

    case 'Y': // Скорости и ускорения стопы
      fOtladkaMes("SpeedFoots=" + String(x));
      SerL.prepareMessage( 'S', x);
      delay(15);
      SerR.prepareMessage( 'S', x);
      delay(15);
      break;

    case 'W':
      fOtladkaMes("AccelFoots=" + String(x));
      SerL.prepareMessage( 'X', x);
      delay(15);
      SerR.prepareMessage( 'X', x);
      delay(15);
      break;

    case 'd':
      currRbStps = "d" + String(x);
      pr_telega.SetMaxDist(x);
      break;
    case 'm': // маленький шажок робота
      minRbtStep = x;
      currMinStepRot = "m" + String(x);
      fOtladkaMes("MinStp=" + String(x));
      break;
    case 'D': //
	  if(x_l < 0) break;
      do {} while (RecordingTime() < x_l);
      break;
    case 'J': // поворот влево
	
	  break;
	case 'K': // поворот вправо
	  break;

    default:
      fErrorMes("unknownCmd:" + String(cmdR));
      fErrorMes(String(RF_data));
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








byte walkForward(long stepAngle, posOfMotors & mot)
{
  /*  if (stWork == StRec)
    {
      Actions = wait;
      fSendState(stWork, Actions, 0);
    }*/
  fAddInActionInRecordMode(wait);
  unsigned long halfLengthStepsCount = 0;
  Actions = walkFf;
  pr_telega.fDoezd(*motorLink, telega);
  if (readyForRotRightFoot() ) // стоим на правой ноге?
  {
	oldActions = walkFf;
    while (1)
    {
      // поворачиваемся вперед на правой ноге на нужный угол
      if (orient_steps(stepAngle,right_leg, forward, mot) && (stWork == StWork)) return 1;
      /*      halfLengthStepsCount++;
            if (fBreak(not_leg, foot, mot))
            { // отправляем в ПК информацию о количестве пройденных шагов, направлении движения
              fSendState(stWork, Actions, halfLengthStepsCount);
              return 1;
            }
            if ((stWork == StPlay) && (halfLengthStepsCount >= maxStepsCount))
            {
              //      fSendState(stWork, Actions, radio, mot);
              fOtladkaMes("Reached");
              return 0;
            }*/
      // опускаем левую ногу (будем стоять на обоих)
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      if (fStepsCounter(mot, halfLengthStepsCount)) return 1;
      // перевозим тележку влево
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) return 1;
      // если стоп в обычном режиме работы, то остановится
      if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
      // поднимаем(втягиваем) правую ногу.
      if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
      // поворачиваемся на левой ноге
      if (orient_steps(stepAngle,left_leg, forward, mot) && (stWork == StWork)) return 1;
      /*      halfLengthStepsCount++;
            if (fBreak(not_leg, foot, mot))
            {
              fSendState(stWork, Actions, halfLengthStepsCount);
              return 1;
            }
            if ((stWork == StPlay) && (halfLengthStepsCount >= maxStepsCount))
            { // пройденно нужное количество шагов, поэтому выходим
              //      fSendState(stWork, Actions, radio, mot);
              fOtladkaMes("Reached");
              return 0;
            } */
      // опускаем правую ногу
      if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      if (fStepsCounter(mot, halfLengthStepsCount)) return 1;
      // перевозим тележку вправо
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1; // ехать вправо
      if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
      // поднимаем левую ногу
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
      // слушаем ББ на случай команды стоп выходим из цикла
      //                if (fBreak(left_leg, knee)) break;
    }
  }// подумать о погрешности между обеими ногами и как ее убрать.

  else if (fstandStill()) // стоим на обеих ногах?
  { // если левая впереди, то телега справа и надо идти вперед, то много лишних действий.
    // если левая сзади и надо идти назад == много лишних действий.
    // для того чтобы небыло лишних движений при смене направления движения на противоположное

    if (oldActions == walkBk)
    {
      oldActions = walkFf;
       /*           if (pr_telega.dir_flg  ==  right) {
              if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) return 1; // ехать влево
            } else if (pr_telega.dir_flg  ==  left)
              if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1; // ехать вправо
      */
      /*
            if (old_leg == left_leg) // последнее опускание левой ноги на опору?
            {
      		// правая нога впереди? (смотрим влево?)
      		if(fEastOrWestStandingCalc() == 0)
              // если смотреть спереди, то телега ехать вправо
              if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1;
              if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
              // поднимаем левую ногу
              if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
              // слушаем ББ на случай команды стоп выходим из цикла
              //                if (fBreak(left_leg, knee)) break;
              // поворачиваемся вперед на правой ноге на нужный угол
              if (orient_steps(right_leg, forward, mot) && (stWork == StWork)) return 1;
              halfLengthStepsCount++;
              if (fBreak(not_leg, foot, mot))
              {
                fSendState(stWork, Actions, halfLengthStepsCount);
                return 1;
              }
              if ((stWork == StPlay) && (halfLengthStepsCount >= maxStepsCount))
              {
                //       fSendState(stWork, Actions, radio, mot);
                fOtladkaMes("Reached");
                return 0;
              }
              // опускаем левую ногу            (будем стоять на обоих)
              if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
            }
      */
    }

    // левая нога впереди? (смотрим вправо?)
    if (fEastOrWestStandingCalc() == 1)
    {
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1;
      if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
      // поднимаем левую ногу
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
      // слушаем ББ на случай команды стоп выходим из цикла
      //                if (fBreak(left_leg, knee)) break;
      // поворачиваемся вперед на правой ноге на нужный угол
      if (orient_steps(stepAngle,right_leg, forward, mot) && (stWork == StWork)) return 1;
      /*      halfLengthStepsCount++;
            if (fBreak(not_leg, foot, mot))
            {
              fSendState(stWork, Actions, halfLengthStepsCount);
              return 1;
            }
            if ((stWork == StPlay) && (halfLengthStepsCount >= maxStepsCount))
            {
              //       fSendState(stWork, Actions, radio, mot);
              fOtladkaMes("Reached");
              return 0;
            } */
      // опускаем левую ногу            (будем стоять на обоих)
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      if (fStepsCounter(mot, halfLengthStepsCount)) return 1;
    }


    while (1)
    {
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) return 1; // ехать влево
      if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
      // поднимаем(втягиваем) правую ногу.
      if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
      //                if (fBreak(right_leg, knee)) break;
      // поворачиваемся на левой ноге
      if (orient_steps(stepAngle,left_leg, forward, mot) && (stWork == StWork)) return 1;
      halfLengthStepsCount++;
      /*      if (fBreak(not_leg, foot, mot))
            {
              fSendState(stWork, Actions, halfLengthStepsCount);
              return 1;
            }
            if ((stWork == StPlay) && (halfLengthStepsCount >= maxStepsCount))
            {
              //     fSendState(stWork, Actions, radio, mot);
              fOtladkaMes("Reached");
              return 0;
            }*/
      // опускаем правую ногу             (выставляем в 0)
      if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      //                if (fBreak(right_leg, knee)) break;
      if (fStepsCounter(mot, halfLengthStepsCount)) return 1;
      // перевозим тележку вправо
      if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1; // ехать вправо
      // поднимаем левую ногу
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
      // слушаем ББ на случай команды стоп выходим из цикла
      //                if (fBreak(left_leg, knee)) break;
      // поворачиваемся вперед на правой ноге на нужный угол
      if (orient_steps(stepAngle,right_leg, forward, mot) && (stWork == StWork)) return 1;
      /*      halfLengthStepsCount++;
            if (fBreak(not_leg, foot, mot))
            {
              fSendState(stWork, Actions, halfLengthStepsCount);
              return 1;
            }
            if ((stWork == StPlay) && (halfLengthStepsCount >= maxStepsCount))
            {
              //       fSendState(stWork, Actions, radio, mot);
              fOtladkaMes("Reached");
              return 0;
            }*/

      // опускаем левую ногу            (будем стоять на обоих)
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      if (fStepsCounter(mot, halfLengthStepsCount)) return 1;
      //      if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
      //                if (fBreak(left_leg, knee)) break;
    }
  }
  //              стоим на левой ноге
  else if (readyForRotLeftFoot())
  {
	oldActions = walkFf;
    while (1)
    {
      // поворачиваемся на левой ноге
      if (orient_steps(stepAngle,left_leg, forward, mot) && (stWork == StWork)) return 1;
      /*      halfLengthStepsCount++;
            if (fBreak(not_leg, foot, mot))
            {
              fSendState(stWork, Actions, halfLengthStepsCount);
              return 1;
            }
            if ((stWork == StPlay) && (halfLengthStepsCount >= maxStepsCount))
            {
              //        fSendState(stWork, Actions, radio, mot);
              fOtladkaMes("Reached");
              return 0;
            }*/
      // опускаем правую ногу
      if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      //                if (fBreak(right_leg, knee)) break;
      if (fStepsCounter(mot, halfLengthStepsCount)) return 1;
      // перевозим тележку вправо
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1; // ехать вправо
      if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
      // поднимаем левую ногу
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
      // слушаем ББ на случай команды стоп выходим из цикла
      //                if (fBreak(left_leg, knee)) break;
      // поворачиваемся вперед на правой ноге на нужный угол
      if (orient_steps(stepAngle,right_leg, forward, mot) && (stWork == StWork)) return 1;
      /*      halfLengthStepsCount++;
            if (fBreak(not_leg, foot, mot))
            {
              fSendState(stWork, Actions, halfLengthStepsCount);
              return 1;
            }
            if ((stWork == StPlay) && (halfLengthStepsCount >= maxStepsCount))
            {
              //      fSendState(stWork, Actions, radio, mot);
              fOtladkaMes("Reached");
              return 0;
            }*/
      // опускаем левую ногу (будем стоять на обоих)
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      //                if (fBreak(left_leg, knee)) break;
      if (fStepsCounter(mot, halfLengthStepsCount)) return 1;
      // перевозим тележку влево
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) return 1; // ехать влево
      if (fBreak(not_leg, knee,  mot) && (stWork == StWork)) return 1;
      // поднимаем(втягиваем) правую ногу.
      if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
      //                if (fBreak(right_leg, knee)) break;
    }
  }
  return 0;
  //        if(RightFootCurrentSteps !=0)  // корпус повернут на правой ноге?
  /*
    if( rbOrient < oldOrient) // смена ориентации против ЧС?
    {
    step_count = 0;

    1.нога поднята на достаточную высоту?
    2.это первый шаг?
    3.делаем доворот на угол = ориентация + угол шага
    а вторую поднятую ногу доворачиваем в нужное положение
    4.это не первый шаг?
    5.поворачиваемся на следующей ноге на угол = ориентация + угол шага.

    }
    if(rbOrient > oldOrient) // смена ориентации по ЧС?
    {
    step_count = 0;
    }
    if(rbOrient == oldOrient) // нет смены ориентации
    {
    if(step_count == 0)
    {
    }
    }
  */
}



byte walkBackward(long stepAngle, posOfMotors & mot)
{
  /*
    if (((RightLegCurrentSteps - LeftLegCurrentSteps) < stepsDepthInSteps) && (RightLegCurrentSteps - LeftLegCurrentSteps > 0)) // стоим на правой ноге?
    {
    // поднимаем левую ногу
    if (Leg_fn(left_leg, mode, vtianut)) break;
    } // стоим на левой ноге?
    if (((RightLegCurrentSteps - LeftLegCurrentSteps) < -stepsDepthInSteps) && (RightLegCurrentSteps - LeftLegCurrentSteps < 0))
    { // втягиваем правую ногу
    if (Leg_fn(right_leg, mode, vtianut)) break;
    }


    if (stWork == StRec)
    {
    Actions = wait;
    fSendState(stWork, Actions, 0);
    }
  */
  fAddInActionInRecordMode(wait);
  Actions = walkBk;
  unsigned long halfLengthStepsCount = 0;
  pr_telega.fDoezd(*motorLink, telega);
  //#ifdef _OTLADKA_
  //  Serial.println("Stoim na pravoy noge");
  //  Serial.print("RightLegCurrentSteps - LeftLegCurrentSteps = ");
  //  Serial.println(RightLegCurrentSteps - LeftLegCurrentSteps);
  //  Serial.print("stepsDepthInSteps = ");
  //  Serial.println(stepsDepthInSteps);
  //  Serial.println("RightLegCurrentSteps - LeftLegCurrentSteps >= stepsDepthInSteps");
  //#endif
  if (readyForRotRightFoot() ) // стоим на правой ноге?
  {
	oldActions = walkBk;
    while (1)
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    { //!!! добавить условие записи шагов чтобы break не останавливал цикл шагания в затруднительных местах !!!
      //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      // поворачиваемся назад на правой ноге на нужный угол
      if (orient_steps(stepAngle, right_leg, backward, mot) && (stWork == StWork)) return 1;
      // опускаем левую ногу (будем стоять на обоих)
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      /*      halfLengthStepsCount++;
            // слушаем ББ на случай команды стоп выходим из цикла
            if (fBreak(not_leg, foot, mot))
            {
              fSendState(stWork, Actions, halfLengthStepsCount);
              return 1;
            }
            // выход из шагания если закончилось число шагов + вызов функции, которая отправит данные
            // для ПК о выполнении операции
            if ((stWork == StPlay) && (halfLengthStepsCount >= maxStepsCount))
            {
              //    fSendState(stWork, Actions, radio, mot);
              fOtladkaMes("PlayModeMaxStepsReached");
              return 0;
            }*/
      //                if (fBreak(left_leg, knee)) break;
      if (fStepsCounter(mot, halfLengthStepsCount)) return 1;
      // перевозим тележку влево
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) return 1; // ехать влево
      if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
      // поднимаем(втягиваем) правую ногу.
      if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
      //                if (fBreak(left_leg, foot)) break;
      // поворачиваемся на левой ноге
      if (orient_steps(stepAngle, left_leg, backward, mot) && (stWork == StWork)) return 1;
      // опускаем правую ногу
      if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      /*      halfLengthStepsCount++;
            if (fBreak(not_leg, foot, mot))
            {
              fSendState(stWork, Actions, halfLengthStepsCount);
              return 1;
            }
            if ((stWork == StPlay) && (halfLengthStepsCount >= maxStepsCount))
            {
              //     fSendState(stWork, Actions, radio, mot);
              fOtladkaMes("PlayModeMaxStepsReached");
              return 0;
            } */
      if (fStepsCounter(mot, halfLengthStepsCount)) return 1;
      //                if (fBreak(right_leg, knee)) break;
      // перевозим тележку вправо
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1; // ехать вправо
      if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
      // поднимаем левую ногу
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
    }
  }// подумать о погрешности между обеими ногами и как ее убрать.
  else if (fstandStill()) // стоим на обеих ногах?
  {
    // для того чтобы небыло лишних движений при смене направления движения на противоположное

    if (oldActions == walkFf) // шел вперед?
    {
      oldActions = walkBk;
      /*            if (pr_telega.dir_flg  ==  right)  {
              if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) return 1; // ехать влево
            } else if (pr_telega.dir_flg  ==  left)
              if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1; // ехать вправо
      */
      //      if (old_leg == left_leg) // последнее опускание левой ноги на опору?
      // левая нога впереди? (смотрим вправо?)
      if (fEastOrWestStandingCalc() == 0)
      {
        if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1; // ехать вправо
        if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
        // поднимаем левую ногу
        if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
        // слушаем ББ на случай команды стоп выходим из цикла
        //                if (fBreak(left_leg, knee)) break;
        // поворачиваемся вперед на правой ноге на нужный угол
        if (orient_steps(stepAngle, right_leg, backward, mot) && (stWork == StWork)) return 1;
        // опускаем левую ногу            (будем стоять на обоих)
        if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
        /*        halfLengthStepsCount++;
                if (fBreak(not_leg, foot, mot))
                {
                  fSendState(stWork, Actions, halfLengthStepsCount);
                  return 1;
                }
                if ((stWork == StPlay) && (halfLengthStepsCount >= maxStepsCount))
                {
                  //       fSendState(stWork, Actions, radio, mot);
                  fOtladkaMes("Reached");
                  return 0;
                }*/
        if (fStepsCounter(mot, halfLengthStepsCount)) return 1;
      }
    }

    /*
        // тележка справа? тогда шагаем с левой ноги
        if (pr_telega.dir_flg  ==  right)
        {
          if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
          // слушаем ББ на случай команды стоп выходим из цикла
          //           if (fBreak(left_leg, knee)) break;
          // поворачиваемся вперед на правой ноге на нужный угол
          if (orient_steps(right_leg, backward, mot) && (stWork == StWork)) return 1;
          halfLengthStepsCount++;
          if (fBreak(not_leg, foot, mot))
          {
            fSendState(stWork, Actions, halfLengthStepsCount);
            return 1;
          }
          if ((stWork == StPlay) && (halfLengthStepsCount >= maxStepsCount))
          {
            //       fSendState(stWork, Actions, radio, mot);
            fOtladkaMes("PlayModeMaxStepsReached");
            return 0;
          }
          // опускаем левую ногу (будем стоять на обоих)
          if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
          //         if (fBreak(left_leg, knee)) break;
        }

    */
    while (1)
    {
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) return 1; // ехать влево
      if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
      // поднимаем(втягиваем) правую ногу.
      if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
      //        if (fBreak(right_leg, knee)) break;
      // поворачиваемся на левой ноге
      if (orient_steps(stepAngle, left_leg, backward, mot) && (stWork == StWork)) return 1;
      // опускаем правую ногу
      if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      /*      halfLengthStepsCount++;
            if (fBreak(not_leg, foot, mot))
            {
              fSendState(stWork, Actions, halfLengthStepsCount);
              return 1;
            }
            if ((stWork == StPlay) && (halfLengthStepsCount >= maxStepsCount))
            {
              //       fSendState(stWork, Actions, radio, mot);
              fOtladkaMes("PlayModeMaxStepsReached");
              return 0;
            }*/
      if (fStepsCounter(mot, halfLengthStepsCount)) return 1;
      //               if (fBreak(right_leg, knee)) break;
      // перевозим тележку вправо
      //      if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1; // ехать вправо
      if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
      // поднимаем левую ногу
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
      // слушаем ББ на случай команды стоп выходим из цикла
      //           if (fBreak(left_leg, knee)) break;
      // поворачиваемся вперед на правой ноге на нужный угол
      if (orient_steps(stepAngle, right_leg, backward, mot) && (stWork == StWork)) return 1;
      /*      halfLengthStepsCount++;
            if (fBreak(not_leg, foot, mot))
            {
              fSendState(stWork, Actions, halfLengthStepsCount);
              return 1;
            }
            if ((stWork == StPlay) && (halfLengthStepsCount >= maxStepsCount))
            {
              //       fSendState(stWork, Actions, radio, mot);
              fOtladkaMes("PlayModeMaxStepsReached");
              return 0;
            } */
      // опускаем левую ногу (будем стоять на обоих)
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      if (fStepsCounter(mot, halfLengthStepsCount)) return 1;
      //     if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
    }
  }
  //              стоим на левой ноге
  else if (readyForRotLeftFoot())
  {
	oldActions = walkBk;
    while (1)
    {
      // поворачиваемся на левой ноге
      if (orient_steps(stepAngle, left_leg, backward, mot) && (stWork == StWork)) return 1;
      /*      halfLengthStepsCount++;
            if (fBreak(not_leg, foot, mot))
            {
              fSendState(stWork, Actions, halfLengthStepsCount);
              return 1;
            }
            if ((stWork == StPlay) && (halfLengthStepsCount >= maxStepsCount))
            {
              //      fSendState(stWork, Actions, radio, mot);
              fOtladkaMes("PlayModeMaxStepsReached");
              return 0;
            }
      */
      // опускаем правую ногу
      if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      if (fStepsCounter(mot, halfLengthStepsCount)) return 1;
      // перевозим тележку вправо
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1; // ехать вправо
      if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
      // поднимаем левую ногу
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
      // слушаем ББ на случай команды стоп выходим из цикла
      // поворачиваемся вперед на правой ноге на нужный угол
      if (orient_steps(stepAngle, right_leg, backward, mot) && (stWork == StWork)) return 1;
      /*      halfLengthStepsCount++;
            if (fBreak(not_leg, foot, mot))
            {
              fSendState(stWork, Actions, halfLengthStepsCount);
              return 1;
            }
            if ((stWork == StPlay) && (halfLengthStepsCount >= maxStepsCount))
            {
              //        fSendState(stWork, Actions, radio, mot);
              fOtladkaMes("PlayModeMaxStepsReached");
              return 0;
            }*/
      // опускаем левую ногу (будем стоять на обоих)
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      if (fStepsCounter(mot, halfLengthStepsCount)) return 1;
      // перевозим тележку влево
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) return 1; // ехать влево
      if (fBreak(not_leg, knee, mot) && (stWork == StWork)) return 1;
      // поднимаем(втягиваем) правую ногу.
      if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
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
  err = pr_telega.RotateStps4Step(*motorLink, telega, -60L);
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
void fCalibration(UART_Serial & lSer, UART_Serial & rSer, posOfMotors & mot)
{
  char cmdL = 0x0, cmdR = 0x0;
  fOtladkaMes("Starting-calibration");
#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, LOW);
#endif
  if (Cal_telega()) return; // переносим центр на правую ногу
  // Калибровка обоих ног
  delay(50);
  lSer.prepareMessage('K'); // вначале вытягиваем левую ногу
  delay(10);
  rSer.prepareMessage('K'); // потом правую
  // ждем пока не получим ответ от двух
  // концевиков ног о выполненной калибровке
  long kal_time = millis();
  bool in_cycle = 1;
  bool AnswerL = 0;
  bool AnswerR = 0;
  while (in_cycle)
  {
    if (fBreak(not_leg, knee, mot)) break;
    if (lSer.handle_serial() > 0) // чего-то принято от левой?
    {
      lSer.getString0(cmdL); // смотрим чего пришло
      if (cmdL != 'C')
      {
        fErrorMes("LeftLegWrongAnswer");
        lSer.prepareMessage( 'K');
        //       delay(10);
      }
      else AnswerL = 1;
    }

    if (rSer.handle_serial() > 0) // чего-то принято от левой?
    {
      rSer.getString0(cmdR); // смотрим чего пришло
      if (cmdR != 'C')
      {
        fErrorMes("RightLegWrongAnswer");
        rSer.prepareMessage( 'K');
        //       delay(10);
      }
      else AnswerR = 1;
    }
    if (AnswerL && AnswerR)
    {
      LeftLegCurrentSteps = 0;
      RightLegCurrentSteps = 0;
      CurrentZero = 0;
      in_cycle = 0;
      cmdL = cmdR = 0;
      rbOrient  = 0;
      oldOrient = 0;
      break;
    }
    if ((millis() - kal_time) > 20000) // если через 20 секунд не вышли из цикла
    { break;
      /*
        if (fBreak(not_leg, knee)) break;
        if (SerL.handle_serial())
        SerL.getString0(cmdL);
        delay(10);
        if (SerR.handle_serial())
        SerR.getString0(cmdR);
        delay(10);
        // смотрим какая нога не среагировала на опускание, повторяем команду
        if (cmdL != 'C')   SerL.prepareMessage( 'K');
        delay(10);
        if (cmdR != 'C')   SerR.prepareMessage( 'K');
        delay(300);
        #ifdef _OTLADKA_
        Serial.println("CalibrationRepeat");
        #endif
      */
    }
  }     // in cycle


  SerL.prepareMessage( 'P', 14000 / 4);
  delay(15);
  SerR.prepareMessage( 'P', 14000 / 4);
  delay(15);

  // втянем правую
  RightLegCurrentSteps = -stepsPerLegRot * 15L / lengthPerLegRot;
  rSer.prepareMessage( 'h', RightLegCurrentSteps);
  if (fAnswerWait(right_leg, knee, mot)) return;
  // повернем стопу до концевика
  rSer.prepareMessage('g');
  delay(50);
  if (fAnswerWait(right_leg, zero, mot)) return;
  LeftLegCurrentSteps = -stepsPerLegRot * 15L / lengthPerLegRot;
  lSer.prepareMessage( 'h', LeftLegCurrentSteps);
  if (fAnswerWait(left_leg, knee, mot)) return;

  // двигаем тележку влево
  pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight());
  // втягиваем на 30 мм
  // еще втянем левую
  LeftLegCurrentSteps = -stepsPerLegRot * 30L / lengthPerLegRot;
  lSer.prepareMessage( 'h', LeftLegCurrentSteps);
  if (fAnswerWait(left_leg, knee, mot)) return;
  // повернем стопу до концевика
  lSer.prepareMessage('g');
  delay(50);
  if (fAnswerWait(left_leg, zero, mot));
// устанавливаем левую ногу вровень с правой

  RightLegCurrentSteps = -stepsPerLegRot * 30L / lengthPerLegRot;
  rSer.prepareMessage( 'h', RightLegCurrentSteps);
  if (fAnswerWait(right_leg, knee, mot)) return;


  // двигаемся в исходное положение
  delay(100);
  LeftLegCurrentSteps = 0;
  RightLegCurrentSteps = 0;
//  LeftLegCurrentSteps = -stepsPerLegRot * fullRotationLimit / 2;
//  RightLegCurrentSteps = -stepsPerLegRot * fullRotationLimit / 2;
  lSer.prepareMessage( 'i', LeftLegCurrentSteps); // втягиваем
  rSer.prepareMessage( 'i', RightLegCurrentSteps); // втягиваем
  CurrentZero = LeftLegCurrentSteps;
  delay(10);
  if (fAnswerWait(all_legs, knee, mot)) return;
  fOtladkaMes("CalOk");


  SerL.prepareMessage( 'P', 18000 / 4);
  delay(15);
  SerR.prepareMessage( 'P', 18000 / 4);
  delay(15);
// после калибровки установить режим с поднятыми ногами
  fOtladkaMes("EnergySaveMode");
  mode = energySaving;
// установить угол шагания 30 градусов
  fOtladkaMes("30 Degree");
  currStpAngl[1] = '2'; currStpAngl[2] = '2';
//  stepAngle = calc_angle(30, m_stp);


  //  delay(10);
  //  if (fAnswerWait(right_leg, knee, mot)) return;
  //  delay(10);
#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, HIGH);
#endif
}













// --------- ОСТАНОВКА ИЛИ ОБРАБОТКА ПОВОРОТА ----------------
bool fBreak(robot_leg leg, MKmotor Uzel, posOfMotors & mot)
{
  fOtladkaMes("InBreak");
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
    if (RF_data[0] == 'R') // команда повернуть влево
    {
      rbOrient -= x;
      //  rotPlace(mot, minRbtStep, x);
      Actions = turnLfst;
//      fSendState(stWork,  Actions, x);
      fAddInActionInRecordMode(Actions);
    }
    if (RF_data[0] == 'L') // команда повернуть вправо
    {
      rotPlace(mot, minRbtStep, -x);	
  //    rbOrient += x;
      Actions = turnRfst;
//      fSendState(stWork,  Actions, x);
      fAddInActionInRecordMode(Actions);
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
          //	  fOtladkaMes("LftKneeNswrWt...");
          // ожидание ответа от ноги
          while (cmdL != 'A')
          { // ждем ответ от колена
            if ((k % 100) == 0) //остаток от деления равен нулю?
              fOtladkaMes(".");
              k++;
            delay(delay_nrf_pause);
            if (SerL.handle_serial() > 0) // что-то принято?
            {
              SerL.getString1(cmdL, Data1L);
              if (cmdL == 'A')
              {
                LeftLegCurrentSteps = Data1L;
                //		fOtladkaMes("ok\r\n");
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
              fOtladkaMes(".");
              k++;
            delay(delay_nrf_pause);
            if (SerL.handle_serial() > 0)
            {
              SerL.getString1(cmdL, Data1L);
              if (cmdL == 'B')
              {
                if (LeftFootCurrentSteps != Data1L)
                  fOtladkaMes("LftFtStpsErr");
              } else
              {
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
          //	  fOtladkaMes("RghtKneeNswrWt...");
          k = 0;
          while (cmdR != 'A')
          { // ждем ответ от колена
            if ((k % 100) == 0) //остаток от деления равен нулю?
              fOtladkaMes(".");
              k++;
            delay(delay_nrf_pause);
            if (SerR.handle_serial() > 0) // что-то принято?
            {
              SerR.getString1(cmdR, Data1R);
              if (cmdR == 'A')
              {
                RightLegCurrentSteps = Data1R;
                //			fOtladkaMes("ok\r\n");
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
          //	  fOtladkaMes("RghtFtAnswrWt...");
          k = 0;
          while (cmdR != 'B')
          {
            if ((k % 100) == 0) //остаток от деления равен нулю?
              fOtladkaMes(".");
              k++;
            delay(delay_nrf_pause);
            if (SerR.handle_serial() > 0)
            {
              SerR.getString1(cmdR, Data1R);
              if (cmdR == 'B')
              {
                if (RightFootCurrentSteps != Data1R)
                  fOtladkaMes("RghtFtStpsErr");
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
    pr_telega.fDoezd(*motorLink, telega);
    if (out == 1)
    {
      String OM = "BreakOK=" + String(out);
      fOtladkaMes(OM);
    }
  }
  //  RF_data[0] = '0';
  cmdL = cmdR = 0;
  fOtladkaMes("Break_ok");
  return out;
} // ==== fBreak =======





// опускаем поднятую ногу
void StepsManage(posOfMotors &mot)
{
  //  fOtladkaMes("StepManage");
  if (LeftLegCurrentSteps != RightLegCurrentSteps)
  {
    /*
        if (stWork == StRec)
        {
          Actions = wait;
          fSendState(stWork, Actions, 0);
        }
    */
    fAddInActionInRecordMode(wait);
    //    if ((millis() - parkingTimer) >= waitTimeForParkingLeg)
    {
      if (LeftLegCurrentSteps < RightLegCurrentSteps) //стоим на правой ноге?
      {
        // опускаем левую ногу (будем стоять на обоих)
        Leg_fn(left_leg, mode, vytianut, mot);
        if (fBreak(left_leg, knee, mot)) return;
      }
      if (RightLegCurrentSteps < LeftLegCurrentSteps) //стоим на левой ноге?
      {
        // опускаем правую ногу (будем стоять на обоих)
        Leg_fn(right_leg, mode, vytianut, mot);
        if (fBreak(right_leg, knee, mot)) return;
      }
    }
    /*    if (stWork == StRec)
        {
          Actions = standStil;
          fSendState(stWork, Actions, 0);
        }
    */
    fAddInActionInRecordMode(standStil);
  }
}








// Отправка текущего состояния для записи и воспроизведения
void fSendState(StadyWork WorkSt, actions Action, unsigned short param)
{
  // при записи, после того как ПК отправил команду повернуть на МК
  // он записывает в файл свою команду и ждет от МК сообщение о пройденном
  // количестве шагов для записи их в файл. После обнуляем кол. шагов
  // В режиме чтения нужно просто отправить количество пройденных шагов
  // чтобы ПК знал, что нужно выполнять новое действие.
  //  fOtladkaMes("InfSendState\r\n");
  String Stp  = "";
  String Str1 = "";
  String Str2 = "";
  //  char   Str3[50];
  String out = "";
  if ((WorkSt == StRec))
  {
    switch (Action)
    {
      case walkFf:
        Stp = "K14"; // малая s означает количество пройденных шагов
        //        delay(20);
        Str1 = "s";
        Str2 = String(param); // число шагов конвертируем в строку
        //       ltoa(halfLengthStepsCount, Str2, 10);
        Str1.concat(Str2);  // добавляем к 's' число шагов и сохраняем в Str1
        //    Str1 += Str2;
        //    strcat(Str1, Str2);

#ifdef _EPROM1_
        //     Str1.toCharArray(Str3, Str1.length() + 1); // конвертируем из String в
        writeString(Str1);
//        fOtladkaMes(readString(StrAddr - incriment));
        //		Stp.toCharArray(Str3, Stp.length() + 1);
        writeString(Stp);
//        fOtladkaMes(readString(StrAddr - incriment));
        //	fOtladkaMes("StringAdr2 = " + String(StrAddr));

#endif
        param = 0;
        break;
      case walkBk:
        Stp = "K13"; // малая s означает количество пройденных шагов
        Str1 = "s";
        Str2 = String(param); // число шагов конвертируем в строку
        //        Str1 += Str2;  // добавляем к 's' число шагов и сохраняем в Str1
        Str1.concat(Str2);
#ifdef _EPROM1_
        //    Str1.toCharArray(Str3, Str1.length() + 1); // конвертируем из String в
        writeString(Str1);
//        fOtladkaMes(readString(StrAddr - incriment));
        //	Stp.toCharArray(Str3, Stp.length() + 1);
        writeString(Stp);
        //        fOtladkaMes(readString(StrAddr - incriment));
#endif
        break;
      //    case constCmd:
      case turnL:
#ifdef _EPROM1_
        Str1 = "L";
        Str2 = String(param); // конвертируем в строку
        Str1.concat(Str2);
        writeString(Str1);
        //        fOtladkaMes(readString(StrAddr - incriment));

#endif
        break;
      case turnR:
#ifdef _EPROM1_
        Str1 = "R";
        Str2 = String(param); // конвертируем в строку
        Str1.concat(Str2);
        writeString(Str1);
        //       fOtladkaMes(readString(StrAddr - incriment));
#endif
      case wait:
#ifdef _EPROM1_
        Str1 = "D";
        Str2 = String(RecordingTime()); // записть текущего времени в память
        Str1.concat(Str2);
        if (withPauses) writeString(Str1);
        //       fOtladkaMes(readString(StrAddr - incriment));
#endif
        break;
      case standStil:
        Stp = "K45"; // малая s означает количество пройденных шагов
#ifdef _EPROM1_
        writeString(Stp);
        //        fOtladkaMes(readString(StrAddr - incriment));
#endif

        break;
      case telCenter:
        Stp = "K18"; // малая s означает количество пройденных шагов
#ifdef _EPROM1_
        writeString(Stp);
        //        fOtladkaMes(readString(StrAddr - incriment));
#endif
        break;
	  case goSlow:
        Stp = "K34"; // малая s означает количество пройденных шагов
#ifdef _EPROM1_
        writeString(Stp);
        //        fOtladkaMes(readString(StrAddr - incriment));
#endif
        break;
	  case goFast:
        Stp = "K35"; // малая s означает количество пройденных шагов
#ifdef _EPROM1_
        writeString(Stp);
        //        fOtladkaMes(readString(StrAddr - incriment));
#endif
        break;
	  case shakeOn:
	    Stp = "K60";
        writeString(Stp);
		break;
	  case shakeOff:
	    Stp = "K61";
        writeString(Stp);
		break;
	  case turnLfst:
	    break;
	  case turnRfst:
	    break;
    }
  }
  if ((WorkSt == StPlay))
  {
    Str1 = "K43";
  }
}











// --------- ПОДНЯТИЕ ИЛИ ОПУСКАНИЕ ЛЮБОЙ НОГИ ---------------
//Вытягиваем/втягиваем ногу относительно текущего положения.
bool Leg_fn(robot_leg leg, regimRaboty regim, leg_dir dir, posOfMotors & mot)
{
  fOtladkaMes("Leg_up/dn");
  old_leg = leg;
  bool out = 0;
  if (dir == vytianut)
  {
    if (leg == left_leg) // левая нога
    {
      if (regim == fast) // БЫСТРЫЙ РЕЖИМ
      {
        // вытягивается медленнее, поэтому его выше
        LeftLegCurrentSteps = CurrentZero;// + (stepsDepthInSteps - stepsDepthInSteps / 2.0);
        SerL.prepareMessage( 'i', CurrentZero); // вытягиваем
        delay(10);
        RightLegCurrentSteps = CurrentZero;// - stepsDepthInSteps / 2.0;
        SerR.prepareMessage( 'h', CurrentZero); // втягиваем
        delay(1);
        //       delay(200);
#ifndef _TIHOHOD_
        if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) out = 1;
        pr_telega.fDoezd(*motorLink, telega);
#endif
        if (fAnswerWait(all_legs,  knee, mot)) return 1; //ждем
        // не хватает алгоритма приведения переменной вытянуто в соответствие!!!!!!!
      }
      else
      { // energysaving
        LeftLegCurrentSteps = CurrentZero;
        SerL.prepareMessage( 'i', CurrentZero); // вытягиваем
        delay(1);
        if (fAnswerWait(left_leg, knee, mot)) return 1; //ждем
        LegUpL = vytianut;
      }
    }
    else if (leg == right_leg)
    { // правая нога
      if (regim == fast) // БЫСТРЫЙ РЕЖИМ
      {
        RightLegCurrentSteps = CurrentZero;// + (stepsDepthInSteps - stepsDepthInSteps / 2.0);
        SerR.prepareMessage( 'i', CurrentZero); // вытягиваем
        delay(10);
        LeftLegCurrentSteps = CurrentZero;// - stepsDepthInSteps / 2.0;
        SerL.prepareMessage( 'h', CurrentZero); // втягиваем
        delay(1);
#ifndef _TIHOHOD_
        //      delay(200);
        if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) out = 1;
        pr_telega.fDoezd(*motorLink, telega);
#endif
        if (fAnswerWait(all_legs,  knee, mot)) return 1; //ждем
        // не хватает алгоритма приведения переменной вытянуто в соответствие!!!!!!!
      }
      else
      { // energysaving
        RightLegCurrentSteps = CurrentZero;
        SerR.prepareMessage( 'i', CurrentZero); // вытягиваем
        delay(1);
        if (fAnswerWait(right_leg, knee, mot)) return 1; //ждем
        LegUpR = vytianut;
      }
    }
  }
  if (dir == vtianut)
  {
    if (leg == left_leg) // левая нога
    {
      if (regim == fast) // БЫСТРЫЙ РЕЖИМ
      { //         текущее положение
        RightLegCurrentSteps = CurrentZero + (stepsDepthInSteps - stepsDepthInSteps / 2);
        SerR.prepareMessage( 'i', CurrentZero + (stepsDepthInSteps - stepsDepthInSteps / 2)); // вытягиваем
        delay(10);
        LeftLegCurrentSteps = CurrentZero - stepsDepthInSteps / 2;
        SerL.prepareMessage( 'h', CurrentZero - stepsDepthInSteps / 2); // втягиваем
        delay(1);
        if (fAnswerWait(all_legs,  knee, mot)) return 1; //ждем
        // не хватает алгоритма приведения переменной вытянуто в соответствие!!!!!!!
      }
      else
      { // energysaving
        LeftLegCurrentSteps = CurrentZero - stepsDepthInSteps;
        SerL.prepareMessage( 'h', CurrentZero - stepsDepthInSteps); // втягиваем
        delay(1);
        if (fAnswerWait(left_leg, knee, mot)) return 1; //ждем
        LegUpL = vtianut;
      }
    }
    else
    { // правая нога
      if (regim == fast) // БЫСТРЫЙ РЕЖИМ
      {
        LeftLegCurrentSteps = CurrentZero + (stepsDepthInSteps - stepsDepthInSteps / 2);
        SerL.prepareMessage( 'i', CurrentZero + (stepsDepthInSteps - stepsDepthInSteps / 2)); // вытягиваем
        delay(10);
        RightLegCurrentSteps = CurrentZero - stepsDepthInSteps / 2;
        SerR.prepareMessage( 'h', CurrentZero - stepsDepthInSteps / 2); // втягиваем
        delay(1);
        if (fAnswerWait(all_legs,  knee, mot)) return 1; //ждем
        // не хватает алгоритма приведения переменной вытянуто в соответствие!!!!!!!
      }
      else
      { // energysaving
        RightLegCurrentSteps = CurrentZero - stepsDepthInSteps;
        SerR.prepareMessage( 'h', CurrentZero - stepsDepthInSteps); // втягиваем
        delay(1);
        if (fAnswerWait(right_leg, knee, mot)) return 1; //ждем
        LegUpR = vtianut;
      }
    }
  }

  parkingTimer = millis(); // отсчет для совпадения ног
  return out;
} // ========= Leg_fn ==========

// 0=левый 1=правый.
/*
  short readAnswerletter(const bool L_Or_R, const char letter)
  {
  short returnValue;
  int k = 0;
  char cmdL = 0x0, cmdR = 0x0;
  const short delay_nrf_pause = 10;
  unsigned long last_time = millis();
  unsigned long return_time = 60000;  // отсрочка 60 секунд
  long Data1L, Data1R;
  long oldFootData1L = LeftFootCurrentSteps, oldFootData1R = RightFootCurrentSteps;
  long oldKneeData1L = LeftLegCurrentSteps, oldKneeData1R = RightLegCurrentSteps;
  if(L_Or_R == 0)
  {
    while (cmdL != letter)
    { // ждем ответ от колена
    //        if (fBreak(left_leg, knee))  return 1;
      if ((k % 100) == 0) //остаток от деления равен нулю?
      //		  fOtladkaMes(".");
         k++;
      delay(delay_nrf_pause);
      if (SerL.handle_serial() > 0) // что-то принято?
      {
        SerL.getString1(cmdL, Data1L); // читаем что принято
        if (cmdL == letter)
        {
          // проверяем положение колена
          //     fOtladkaMes("LKnee:" + String(Data1L));
          if (LeftLegCurrentSteps != Data1L)
          {
  //           fErrorMes("MUST BE:" + String(LeftLegCurrentSteps));
            delay(2000);
            LeftLegCurrentSteps = Data1L;
			returnValue = 1;
          }
        }
        else
        {
  //         String errStr = "LftAswrKneeIs:" + String(cmdL);
  //         fErrorMes(errStr);
  //        delay(2000);
		  returnValue = 2;
        }
      }
      if (last_time + return_time < millis())
      {
 		returnValue = 3;
  //       fErrorMes("LEFT-KNEE-TIME-ERROR");
        LeftLegCurrentSteps = oldKneeData1L;
  #ifdef STOP_ON_ERROR
        cmdL = cmdR = 0;
        return returnValue;
  #endif
        break;
      }
      pr_telega.fDoezd(*motorLink, telega);
    }
  }
  return returnValue;
  }
*/













//  ------ ОЖИДАНИЕ ОТВЕТА ОТ ПРИВОДА НОГ -----
bool fAnswerWait(robot_leg leg, MKmotor Uzel, posOfMotors & mot)
{

  int k = 0;
  char cmdL = 0x0, cmdR = 0x0;
  const short delay_nrf_pause = 10;
  unsigned long last_time = millis();
  unsigned long return_time = 60000;  // отсрочка 60 секунд
  long Data1L, Data1R;
  long oldFootData1L = LeftFootCurrentSteps, oldFootData1R = RightFootCurrentSteps;
  long oldKneeData1L = LeftLegCurrentSteps, oldKneeData1R = RightLegCurrentSteps;

  if (leg == left_leg)
  {
    if (Uzel == knee)
    {
      //	  fOtladkaMes("LftKneeAnswrWt...");
      while (cmdL != 'A')
      { // ждем ответ от колена
        //        if (fBreak(left_leg, knee))  return 1;
        if ((k % 100) == 0) //остаток от деления равен нулю?
          //		  fOtladkaMes(".");
          k++;
        delay(delay_nrf_pause);
        if (SerL.handle_serial() > 0) // что-то принято?
        {
          SerL.getString1(cmdL, Data1L); // читаем что принято
          if (cmdL == 'A')
          {
            // проверяем положение колена
            //     fOtladkaMes("LKnee:" + String(Data1L));
            if (LeftLegCurrentSteps != Data1L)
            {
              fErrorMes("MUST BE:" + String(LeftLegCurrentSteps));
              delay(2000);
              LeftLegCurrentSteps = Data1L;
            }
          }
          else
          {
            String errStr = "LftAswrKneeIs:" + String(cmdL);
            fErrorMes(errStr);
          }
        }
        if (last_time + return_time < millis())
        {
          fErrorMes("LEFT-KNEE-TIME-ERROR");
          LeftLegCurrentSteps = oldKneeData1L;
#ifdef STOP_ON_ERROR
          cmdL = cmdR = 0;
          return 1;
#endif
          break;
        }
        pr_telega.fDoezd(*motorLink, telega);
      }
    }
    if (Uzel == foot)
    {
      //	  fOtladkaMes("LftFtNswrWt\r\n");
      k = 0;
      while (cmdL != 'B')
      {
        //        if (fBreak(left_leg, foot)) return 1;
        if ((k % 100) == 0)   //остаток от деления равен нулю?
          //	  fOtladkaMes(".");
          k++;
        delay(delay_nrf_pause);
        if (SerL.handle_serial() > 0)
        {
          SerL.getString1(cmdL, Data1L);
          if (cmdL == 'B')
          {
            //      fOtladkaMes("LFoot:" + String(Data1L));
            if (LeftFootCurrentSteps != Data1L)
            {
              fErrorMes("MUST-BE:" + String(LeftFootCurrentSteps));
              delay(2000);
              LeftFootCurrentSteps = Data1L;
            }
            //              fErrorMes("LftFtStpsErr" + String(Data1L));
          }
          else
          {
            String errStr = "LftNswrFtIs:" + String(Data1L);
            fOtladkaMes(errStr);
          }
        }
        // в режиме воспроизведения?
        if ((stWork == StPlay) && (oldOrient != rbOrient))
        { // сравниваем и если узнали что ориентация поменялась,
          oldOrient = rbOrient;
          // отправляем подтверждение и угол на который повернулись
          //          fSendState(stWork, Actions, radio, mot);
        }

        if (last_time + return_time < millis())
        {
          fErrorMes("LEFT-FOOT-TIME-ERROR");
          LeftFootCurrentSteps = oldFootData1L;
#ifdef STOP_ON_ERROR
          cmdL = cmdR = 0;
          return 1;
#endif
          break;
        }
        //        if (digitalRead(ENDCAP) == EC_LOW) // левая стопа левый упор телеги
        pr_telega.fDoezd(*motorLink, telega);
      }
    }
    if (Uzel == kal)
    {
      //	  fOtladkaMes("Clbrtn nswr wt\r\n");
      k = 0;
      while (cmdL != 'C')
      {
        //        if (fBreak(left_leg, knee)) return 1;    //!!!!!!!!!!!!!!!!
        if ((k % 100) == 0) //остаток от деления равен нулю?
          //		  fOtladkaMes(".");
          k++;
        delay(delay_nrf_pause);
        if (SerL.handle_serial() > 0)
        {
          SerL.getString0(cmdL);
          if (cmdL != 'C')
          {
            String errStr = "Answer-calibration-is:" + String(cmdL);
            fErrorMes(errStr);
          }
        }
        if (last_time + return_time < millis())
        {
          fErrorMes("CALIBRATION-TIME-ERROR");
#ifdef STOP_ON_ERROR
          cmdL = cmdR = 0;
          return 1;
#endif
          break;
        }
        //       if (digitalRead(ENDCAP) == EC_HIGH)
        //       fDoezd(telega, 20000L, 15000L);
      }
    }
    if (Uzel == zero)
    {
      while (cmdL != 'G')
      {
        if (SerL.handle_serial() > 0)
        {
          SerL.getString0(cmdL);
          if (cmdL != 'G')
          {
            String errStr = "Answer-zero-is:" + String(cmdL);
            fErrorMes(errStr);
          }
        }
        if (last_time + return_time < millis())
        {
          fErrorMes("zero-TIME-ERROR");
#ifdef STOP_ON_ERROR
          cmdL = cmdR = 0;
          return 1;
#endif
          break;
        }
      }
    }
  }
  if (leg == right_leg)
  {
    if (Uzel == knee)
    {
      //	  fOtladkaMes("RghtKneeNswrWt...");
      k = 0;
      while (cmdR != 'A') {
        //        if (fBreak(right_leg, knee))  return 1;
        if ((k % 100) == 0) //остаток от деления равен нулю?
          //		  fOtladkaMes(".");
          k++;
        delay(delay_nrf_pause);
        if (SerR.handle_serial() > 0)
        {
          SerR.getString1(cmdR, Data1R);
          if (cmdR == 'A')
          {
            //         fOtladkaMes("RKnee:" + String(Data1R));
            if (RightLegCurrentSteps != Data1R)
            {
              fErrorMes("MUSTBE:" + String(RightLegCurrentSteps));
              delay(2000);
              RightLegCurrentSteps = Data1R;
            }
          }
          else
          {
            String errStr = "RGHT-NSWR-KNEE-IS:" + String(Data1L);
            fErrorMes(errStr);
          }
        }
        if (last_time + return_time < millis())
        {
          fErrorMes("RIGHT-KNEE-TIME-ERROR");
          RightLegCurrentSteps = oldKneeData1R;
#ifdef STOP_ON_ERROR
          cmdL = cmdR = 0;
          return 1;
#endif
          break;
        }
        pr_telega.fDoezd(*motorLink, telega);
      }
    }
    if (Uzel == foot)
    {
      //	  fOtladkaMes("RghtFtNswrWt...");
      k = 0;
      while (cmdR != 'B')
      {
        //        if (fBreak(right_leg, foot)) return 1;
        if ((k % 100) == 0) //остаток от деления равен нулю?
          //		  fOtladkaMes(".");
          k++;
        delay(delay_nrf_pause);
        if (SerR.handle_serial() > 0)
        {
          SerR.getString1(cmdR, Data1R);
          if (cmdR == 'B')
          {
            //       fOtladkaMes("RFoot: " + String(Data1R));
            if (RightFootCurrentSteps != Data1R)
            {
              fErrorMes("MUST BE: " + String(RightFootCurrentSteps));
              delay(2000);
              RightFootCurrentSteps = Data1R;
            }
            //              fErrorMes("RgthFtStpsErr");
          } else
          {
            String errStr = "RGHTNswrFtIs" + String(cmdR);
            fErrorMes(errStr);
          }
        }
        // в режиме воспроизведения?
        if ((stWork == StPlay) && (oldOrient != rbOrient))
        { // сравниваем и если узнали что ориентация поменялась,
          oldOrient = rbOrient;
          // отправляем подтверждение и угол на который повернулись
          //          fSendState(stWork, Actions, radio, mot);
        }
        if (last_time + return_time < millis())
        {
          fErrorMes("RIGHT-FOOT-TIME-ERROR");
          RightFootCurrentSteps = oldFootData1R;
#ifdef STOP_ON_ERROR
          cmdL = cmdR = 0;
          return 1;
#endif
          break;
        }
        if (digitalRead(ENDCAP_R) == EC_LOW) // правая стопа, правый упор телеги контролируется
          pr_telega.fDoezd(*motorLink, telega);
      }
    }
    if (Uzel == kal)
    {
      //	  fOtladkaMes("Clbrtn nswr wt\r\n");
      k = 0;
      while (cmdR != 'C')
      {
        //        if (fBreak(right_leg, knee)) return 1;
        if ((k % 100) == 0) //остаток от деления равен нулю?
          //		  fOtladkaMes(".");
          k++;
        delay(delay_nrf_pause);
        if (SerR.handle_serial() > 0)

          SerR.getString0(cmdR);
        if (last_time + return_time < millis())
        {
          fErrorMes("ERROR-CALIBRATION");
#ifdef STOP_ON_ERROR
          cmdL = cmdR = 0;
          return 1;
#endif
          break;
        }
        if (digitalRead(ENDCAP) == EC_LOW)
          pr_telega.fDoezd(*motorLink, telega);
      }
    }

    if (Uzel == zero)
    {
      while (cmdR != 'G')
      {
        if (SerR.handle_serial() > 0)
        {
          SerR.getString0(cmdR);
          if (cmdR != 'G')
          {
            String errStr = "Answer-zero-is:" + String(cmdR);
            fErrorMes(errStr);
          }
        }
        if (last_time + return_time < millis())
        {
          fErrorMes("zero-TIME-ERROR");
#ifdef STOP_ON_ERROR
          cmdL = cmdR = 0;
          return 1;
#endif
          break;
        }
      }
    }
  }

  if (leg == all_legs)
  {
    if (Uzel == knee)
    {
      //	  fOtladkaMes("RghtNdLftKneeNswrWt\r\n");
      k = 0;
      while ((cmdR != 'A') || (cmdL != 'A'))
      {
        //        if (fBreak(right_leg, foot)) return 1;
        if ((k % 100) == 0) //остаток от деления равен нулю?
          //		  fOtladkaMes(".");
          k++;
        delay(delay_nrf_pause);
        if (SerR.handle_serial() > 0)
        {
          SerR.getString1(cmdR, Data1R);
          if (cmdR == 'A')
          {
            //         fOtladkaMes("RKnee: " + String(Data1R));
            if (RightLegCurrentSteps != Data1R)
            {
              fErrorMes("MUST BE: " + String(RightLegCurrentSteps));
              delay(2000);
              RightLegCurrentSteps = Data1R;
            }
          }
        }
        if (SerL.handle_serial() > 0)
        {
          SerL.getString1(cmdL, Data1L);
          if (cmdL == 'A')
          {
            //        fOtladkaMes("LKnee: " + String(Data1L));
            if (LeftLegCurrentSteps != Data1L)
            {
              fErrorMes("MUST BE: " + String(LeftLegCurrentSteps));
              delay(2000);
              LeftLegCurrentSteps = Data1L;
            }
          }
        }
        // в режиме воспроизведения?
        if (last_time + return_time < millis())
        {
          if (cmdR != 'A')
          {
            fErrorMes("RIGHT KNEE TIME ERROR");
            RightLegCurrentSteps = oldKneeData1R;
          }
          if (cmdL != 'A')
          {
            fErrorMes("LEFT KNEE TIME ERROR");
            LeftLegCurrentSteps = oldKneeData1L;
          }
          delay(2000);
#ifdef STOP_ON_ERROR
          cmdL = cmdR = 0;
          return 1;
#endif
          break;
        }
        //        if (digitalRead(ENDCAP_R) == EC_LOW) // правая стопа, правый упор телеги контролируется
        pr_telega.fDoezd(*motorLink, telega);
      }
    }

    if (Uzel == foot)
    {
      //	  fOtladkaMes("RghtNdLftFtNswrWt\r\n");
      k = 0;
      while ((cmdR != 'B') || (cmdL != 'B'))
      {
        //        if (fBreak(right_leg, foot)) return 1;
        if ((k % 100) == 0) //остаток от деления равен нулю?
          //		  fOtladkaMes(".");
          k++;
        delay(delay_nrf_pause);
        if (SerR.handle_serial() > 0)
        {
          SerR.getString1(cmdR, Data1R);
          if (cmdR == 'B')
          {
            //      fOtladkaMes("RFoot: " + String(Data1R));
            if (RightFootCurrentSteps != Data1R)
            {
              fErrorMes("MUST BE: " + String(RightFootCurrentSteps));
              delay(2000);
              RightFootCurrentSteps = Data1R;
              //              fErrorMes("WrongDataErrRightFoot= " + String(Data1R));
            }
          }
        }
        if (SerL.handle_serial() > 0)
        {
          SerL.getString1(cmdL, Data1L);
          if (cmdL == 'B')
          {
            //      fOtladkaMes("LFoot: " + String(Data1L));
            if (LeftFootCurrentSteps != Data1L)
            {
              fErrorMes("MUST BE: " + String(LeftFootCurrentSteps));
              delay(2000);
              LeftFootCurrentSteps = Data1L;
            }
            //              fErrorMes("WrongDataErrLeftFoot= " + String(Data1L));
          }
        }
        // в режиме воспроизведения?
        if ((stWork == StPlay) && (oldOrient != rbOrient))
        { // сравниваем и если узнали что ориентация поменялась,
          oldOrient = rbOrient;
          // отправляем подтверждение и угол на который повернулись
          //          fSendState(stWork, Actions, radio, mot);
        }
        if (last_time + return_time < millis())
        {
          if (cmdR != 'B')
          {
            fErrorMes("RIGHT FOOT TIME ERROR");
            fErrorMes("RFoot: " + String(Data1R));
            fErrorMes("MUST BE: " + String(RightFootCurrentSteps));
            RightFootCurrentSteps = oldFootData1R;
          }
          if (cmdL != 'B')
          {
            fErrorMes("LEFT FOOT TIME ERROR");
            fErrorMes("LFoot: " + String(Data1L));
            fErrorMes("MUST BE: " + String(LeftFootCurrentSteps));
            LeftFootCurrentSteps = oldFootData1L;
          }
          delay(2000);
#ifdef STOP_ON_ERROR
          cmdL = cmdR = 0;
          return 1;
#endif
          break;
        }
        //        if (digitalRead(ENDCAP_R) == EC_LOW) // правая стопа, правый упор телеги контролируется
        pr_telega.fDoezd(*motorLink, telega);
      }
    }
  }

#ifdef _OTLADKA_
  //  Serial.print("RightFootCurrentSteps: ");
  //  Serial.println(RightFootCurrentSteps);
  //  Serial.print("LeftFootCurrentSteps: ");
  //  Serial.println(LeftFootCurrentSteps);
  //  Serial.print("RightLegCurrentSteps: ");
  //  Serial.println(RightLegCurrentSteps);
  //  Serial.print("LeftLegCurrentSteps: ");
  //  Serial.println(LeftLegCurrentSteps);
#endif

  cmdL = cmdR = 0;
  return 0;
} // ===== fAnswerWait =====












// ----- ОРИЕНТИРОВАНИЕ В НУЖНОМ НАПРАВЛЕНИИ -----
bool change_orient(rot_dir dir, posOfMotors & mot, float min_stp)
{
  fOtladkaMes("min_stp:" + String(min_stp));
  short add_data = min_stp;
  min_stp = min_stp * 444.444444;
  if (readyForRotRightFoot())  // стоим на правой ноге?
  {
    if (dir == leftA) // поворот влево?
    { // поворачиваемся в заданное положение + текущее положение.
      fOtladkaMes("RotRghtDirLft");
      RightFootCurrentSteps = rbOrient * 160000L / 360L - min_stp;
      SerR.prepareMessage( 'c', RightFootCurrentSteps);
      fOtladkaMes("TurnLeftR:" + String(RightFootCurrentSteps));
      rbOrient -= add_data;  //????
      // отворачиваем противоположную стопу в обратную сторону
      LeftFootCurrentSteps = -rbOrient * 160000L / 360L + min_stp;
      SerL.prepareMessage( 'c', LeftFootCurrentSteps);
      delay(100);
      fOtladkaMes("TurnLeftL:" + String(LeftFootCurrentSteps));
      if (fAnswerWait(all_legs,  foot, mot))
      {
        leed_leg = rightLeed;
        return 1; //ждем
      }
    }
    if (dir == rightA)
    {
      fOtladkaMes("RotRghtDirRight");
      RightFootCurrentSteps = rbOrient * 160000L / 360L + min_stp;
      SerR.prepareMessage( 'c', RightFootCurrentSteps);
      fOtladkaMes("TurnRightR:" + String(RightFootCurrentSteps));
      rbOrient += add_data;   //???
      // отворачиваем противоположную стопу в обратную сторону
      LeftFootCurrentSteps = -rbOrient * 160000L / 360L - min_stp;
      SerL.prepareMessage( 'c', LeftFootCurrentSteps);
      delay(100);
      fOtladkaMes("TurnRightL:" + String(LeftFootCurrentSteps));
      if (fAnswerWait(all_legs,  foot, mot))
      {
        leed_leg = leftLeed;
        return 1; //ждем
      }
    }
  }
  else if (readyForRotLeftFoot())  // стоим на левой ноге?
  {
    //    fOtladkaMes("RghtTrnng\r\n");
    if (dir == leftA)
    {
      fOtladkaMes("RotLftDirLft");
      LeftFootCurrentSteps = -rbOrient * 160000L / 360L + min_stp;
      SerL.prepareMessage( 'c', LeftFootCurrentSteps);
      fOtladkaMes("TurnLeftL:" + String(LeftFootCurrentSteps));
      rbOrient -= add_data;
      // отворачиваем противоположную стопу в обратную сторону
      RightFootCurrentSteps = rbOrient * 160000L / 360L - min_stp;
      SerR.prepareMessage( 'c', RightFootCurrentSteps);
      delay(100);
      fOtladkaMes("TurnLeftR:" + String(RightFootCurrentSteps));
      if (fAnswerWait(all_legs,  foot, mot))
      {
        leed_leg = rightLeed;
        return 1; //ждем
      }
    }
    if (dir == rightA)
    {
      fOtladkaMes("RotLftDirRght");
      LeftFootCurrentSteps = -rbOrient * 160000L / 360L - min_stp;
      SerL.prepareMessage( 'c', LeftFootCurrentSteps);
      fOtladkaMes("TurnRightL:" + String(LeftFootCurrentSteps));
      rbOrient += add_data;
      // отворачиваем противоположную стопу в обратную сторону
      RightFootCurrentSteps = rbOrient * 160000L / 360L + min_stp;
      SerR.prepareMessage( 'c', RightFootCurrentSteps);
      fOtladkaMes("TurnRightR:" + String(RightFootCurrentSteps));
      if (fAnswerWait(all_legs,  foot, mot))
      {
        leed_leg = leftLeed;
        return 1; //ждем
      }
    }
  }
  else if ((RightLegCurrentSteps - LeftLegCurrentSteps > -stepsDepthInSteps) && (RightLegCurrentSteps - LeftLegCurrentSteps < stepsDepthInSteps)) // стоим на обеих ногах?
  {
    fErrorMes("BothLegsOnTheSirfaceError");
    return 1;
  }
  //  fOtladkaMes("ChngeOrientSuccess\r\n");
  return 0;
}// Change_Orient














// это для хотьбы...
// Функция поворачивает выбранную ногу до необходимого угла
// и доворачивает вторую ногу на такой же угол в обратную сторону
// leg - нога, на которой совершается поворот
// dir - в какую сторону идти (вперед или назад)
// возвращает 1 если была ошибка, 0 если все штатно
bool orient_steps(long stepAngle, robot_leg leg, step_dir dir, posOfMotors & mot)
{
  //  угол  = угол в градусах * шагов_на_оборот / 360грд + поворот для шага.
  // angleL - положение стопы относительно калиброванного 0 в тиках.
  // rbOrient - угол направления движения в градусах
  fOtladkaMes("Povorot");
  const unsigned short gradus = 1;
  long angleR = 0; // куда нужно повернуть правую
  long angleL = 0; // куда нужно повернуть левую
  // поворот на левой ноге
  //                            -5             -(-10) = +5
  if (leg == left_leg)
  {
    if (readyForRotLeftFoot())   // стоим на левой ноге?
    {
      //     angleR =  rbOrient * 160000L / 360L - stepAngle;
      //     angleL = -rbOrient * 160000L / 360L + stepAngle;
      angleR =  rbOrient * calc_angle(gradus, m_stp) * 2 - stepAngle;
      angleL = -rbOrient * calc_angle(gradus, m_stp) * 2 + stepAngle;
      leed_leg = leftLeed;
      // в какую сторону идем,назад или вперед?
      if (dir == forward)  //
      {
        angleR =  rbOrient * calc_angle(gradus, m_stp) * 2 + stepAngle;
        angleL = -rbOrient * calc_angle(gradus, m_stp) * 2 - stepAngle;
        leed_leg = rightLeed;
      }
	  
	  // условие при котором перепутано направление
	  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	  if(angleR == RightFootCurrentSteps)
	  {
        if(dir == unknown)
		{
	      angleR =  rbOrient * calc_angle(gradus, m_stp) * 2 + stepAngle;
          angleL = -rbOrient * calc_angle(gradus, m_stp) * 2 - stepAngle;
          leed_leg = rightLeed;
		}
	  }
      RightFootCurrentSteps = angleR;
      LeftFootCurrentSteps = angleL;

      if (RightFootCurrentSteps == LeftFootCurrentSteps)
      {
        fOtladkaMes("LeftFootCurrentSteps" + String(LeftFootCurrentSteps));
        fOtladkaMes("RightFootCurrentSteps" + String(RightFootCurrentSteps));
        fOtladkaMes("stepAngle" + String(stepAngle));
        fOtladkaMes("rbOrient" + String(rbOrient));
        SerR.prepareMessage( 'c', RightFootCurrentSteps);
        //         delay(10);
        SerL.prepareMessage( 'c', LeftFootCurrentSteps);
        if (fAnswerWait(all_legs,  foot, mot)) return 1; //ждем
      }
      else {
        SerR.prepareMessage( 'c', RightFootCurrentSteps);
        //      fOtladkaMes("TekUgol= " + String(RightFootCurrentSteps));
        //         delay(10);
        // крутим левую стопу на положительный заданный угол + ориентация
        SerL.prepareMessage( 'c', LeftFootCurrentSteps);
        //      fOtladkaMes("TekUgol= " + String(LeftFootCurrentSteps));
        // ждем отработку поворота на правой ноге
        delay(100);
        if (fAnswerWait(all_legs,  foot, mot)) return 1; //ждем
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
    if (readyForRotRightFoot()) //(RightLegCurrentSteps - LeftLegCurrentSteps) >= stepsDepthInSteps)
    {
      angleR =  rbOrient * calc_angle(gradus, m_stp) * 2 + stepAngle; // куда нужно повернуть правую
      angleL = -rbOrient * calc_angle(gradus, m_stp) * 2 - stepAngle; // куда нужно повернуть левую
      leed_leg = rightLeed;

      if (dir == forward)
      {
        angleR =  rbOrient * calc_angle(gradus, m_stp) * 2 - stepAngle; // куда нужно повернуть правую
        angleL = -rbOrient * calc_angle(gradus, m_stp) * 2 + stepAngle; // куда нужно повернуть левую
        leed_leg = leftLeed;
      }
	  
	  if(angleR == RightFootCurrentSteps)
	  {
        if(dir == unknown)
		{
	      angleR =  rbOrient * calc_angle(gradus, m_stp) * 2 - stepAngle;
          angleL = -rbOrient * calc_angle(gradus, m_stp) * 2 + stepAngle;
          leed_leg = leftLeed;
		}
	  }

      RightFootCurrentSteps = angleR;
      LeftFootCurrentSteps  = angleL;
      // заранее переводим левую стопу в исходное положение
      if (RightFootCurrentSteps == LeftFootCurrentSteps)
      {
        fOtladkaMes("LeftFootCurrentSteps" + String(LeftFootCurrentSteps));
        fOtladkaMes("RightFootCurrentSteps" + String(RightFootCurrentSteps));
        fOtladkaMes("stepAngle" + String(stepAngle));
        fOtladkaMes("rbOrient" + String(rbOrient));
      }
      else {
        SerL.prepareMessage( 'c', LeftFootCurrentSteps);
        //      fOtladkaMes("TekUgol= " + String(LeftFootCurrentSteps));
        //       delay(10);
        // крутим правую стопу на положительный заданный угол + ориентация
        SerR.prepareMessage( 'c', RightFootCurrentSteps);
        //     fOtladkaMes("TekUgol= " + String(RightFootCurrentSteps));
        // ждем отработку поворота на правой ноге
        //     if (fAnswerWait(right_leg, foot, mot)) return 1;  //ждем
        //     if (fAnswerWait(left_leg,  foot, mot)) return 1;  //ждем
        if (fAnswerWait(all_legs,  foot, mot)) return 1; //ждем
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


/*
void fStartPlay(bool zoom)
{
  if (stWork == StRec) return;
  // зачем здесь переключать режим работы в воспроизвдение или работу?
  //  stWork == StWork ? stWork = StPlay : stWork = StWork;  3.3.2020
#ifdef _OTLADKA_
  String infStr = "GtWork=" + String(stWork) + "\r\n";
#ifdef SERIAL_USB
  Serial1.print(infStr);
  delay(50);
  //  Serial1.print("In_Play_mode\r\n");
#else
  Serial.print(infStr);
  Serial.print("In Play mode\r\n");
#endif
#endif

  //  long strNum = 0;
  byte strLen = 7;
  short currStrNum = 0;
  byte sizeOfRFdata = 30;
  char RF_data[sizeOfRFdata];

  // fOtladkaMes("Pckt is rd");

  //  цикл воспроизведения

  int i = 0;
  do { // циклический повтор?
    currStrNum = 0;  // указываем на нулевую строку


    for ( currStrNum = 0; currStrNum < fLength; currStrNum++)
    { // цикл прогона по всем доступным строкам
      // читаем строку буфера
      //     fOtladkaMes(String(fLength));
 //     delay(500);
      char buff[strLen];
      //    String OtlMes;
      //     for (int i = 0; (i < sizeOfRFdata - 1) && (i < strLen - 1) && (work_data[currStrNum][i + 2]!=':'); i++)
      for (i = 0; i < (strLen - 1); i++)
      {
        if (work_data[currStrNum][i] == ':')
        {
          i++;
          buff[i] = '\n';
          //         for (i; i < strLen - 1; i++)
          //         {
          //           buff[i] = '\0';
          //         } 
          break;
        }
        buff[i] = work_data[currStrNum][i];
      }
      //      buff[i+1] = '\n';
      //   OtlMes = "work_data[currStrNum]=" + String(work_data[currStrNum]);
      //  fOtladkaMes(OtlMes);
      //   OtlMes = "buff=" + String(buff);
      //   fOtladkaMes(OtlMes);
#ifdef SERIAL_USB
      //    Serial1.print("currStrNum = ");
      //    Serial1.print(currStrNum + String("\r\n"));
      //	  Serial1.print("\r\n");
      //      Serial1.print("buff = ");
      //     Serial1.print((char*)buff + String("\r\n"));
      //	  Serial1.print("\r\n");
#endif      // обработка строки
      RF_messege_handle(buff, drvPos);
      //    OtlMes = "currStrNum=" + String(currStrNum);
      //    fOtladkaMes(OtlMes);

    }
    // конец прогона по доступным строкам
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


    // смотрим, вдруг выключили воспроизведение или повтор
#ifdef _NRF24_
    if (radio.available())
#else
#ifdef SERIAL_USB
    if (Serial1.available())
#else
    if (Serial.available())
#endif
#endif
    {
#ifdef _BUILTIN_LED_ON_
      digitalWrite(LED_BUILTIN, LOW);
#endif
#ifdef _NRF24_
      radio.read(&RF_data, sizeof(RF_data));
#else
      byte serialAmount = 0;
#ifdef SERIAL_USB
      while (Serial1.available() && (serialAmount < sizeOfRFdata))
      {
        RF_data[serialAmount] = Serial1.read();
#else
      while (Serial.available() && (serialAmount < sizeOfRFdata))
      {
        RF_data[serialAmount] = Serial.read();
#endif
        serialAmount++;
        delayMicroseconds((10000000 / baudRateCom1) + 500);
      }
#endif

      for (int i = 0; i < sizeOfRFdata - 1; i++)
        //        buff[i] = RF_data[i + 1];
        if ((RF_data[0] == 'K') && (RF_data[1] == '4') && (RF_data[2] == '4'))
        {
          LoopPlay == 0 ? LoopPlay = 1 : LoopPlay = 0;
          zoom = LoopPlay;
        }
      //  проверка выключен ли play
      if ((RF_data[0] == 'K') && (RF_data[1] == '4') && (RF_data[2] == '3'))
      {
        stWork == StWork ? stWork = StPlay : stWork = StWork;
#ifdef _OTLADKA_
#ifdef SERIAL_USB
        Serial1.print("Play stops\r\n");
        delay(50);
#else
        Serial.print("Play stops\r\n");
#endif
#endif
        return;
      }
      //      delay(100);
    }
#ifdef _OTLADKA_
#ifdef SERIAL_USB
    Serial1.print("Play loops\r\n");
    delay(50);
#else
    Serial.print("Play loops\r\n");
#endif
#endif
  } while (zoom);

  for (int i1 = 0; i1 < fLength; i1++)
  {
    delete [] work_data[i1];
  }
  delete [] work_data;

  //  fOtladkaMes("OUT_Play_mode");
}

*/












/*
void fDload(void)
{
#ifdef _EPROM1_
  fOtladkaMes(String(readString(0).toInt()));
  for (StrAddr = StartEEPROM; StrAddr <  (unsigned)(readString(0).toInt()); StrAddr += incriment)
  {
    fOtladkaMes("StrAdr= " + String(StrAddr));
    fOtladkaMes(readString(StrAddr));
    delay(45);
  }
  delay(20);
  fOtladkaMes("EndOfDload");
  delay(200);
#else
#ifdef _OTLADKA_
  String infStr = "GtWork=" + String(stWork) + "\r\n";
#ifdef SERIAL_USB
  Serial1.print(infStr);
  delay(50);
#endif
#endif
  unsigned errFlg = 0;
  byte strLen = 7;
  unsigned short currStrNum = 0;
  byte sizeOfRFdata = 30;
  char RF_data[sizeOfRFdata];

  //  String SerBuf = "";
  long time_break = millis();

  while (1) // Задержка, чтобы не пропустить первую команду
  {
#ifdef _NRF24_
    if (radio.available())
#else
    if (Serial1.available())
#endif
    {
#ifdef _NRF24_
      radio.read(&RF_data, sizeof(RF_data));
#else
      byte serialAmount = 0;
      while (Serial1.available() && (serialAmount < sizeOfRFdata))
      {
        RF_data[serialAmount] = Serial1.read();
        //		SerBuf += Serial1.read();
        serialAmount++;
        delayMicroseconds((10000000 / baudRateCom1) + 500);
      }
#endif
      String OtlMes = "r_in:" + String(RF_data);
      fOtladkaMes(OtlMes);
      delay(50);
      break;
    }// if(serial.available)
    if ((millis() - time_break) > 9000 ) // защита от зависания 9 сек
    {
      fErrorMes("Downld timeout");
      return;
    }
  } //  while 1
  // первым в сообщении идет число строк
  fLength = atoi((char*)RF_data); // перевод char в число
  if (fLength <= 0)
  {
    fErrorMes("ZeroMsgLngth");
    return;
  }
  else if (fLength > 500)
  {
    fErrorMes("Too-big-data:" + String(fLength));
    return;
  }
  //  short fLength = SerBuf.toInt(); // перевод char в число
  work_data = new char*[fLength];
  // выделяем память под весь массив
  for (long i1 = 0; i1 < fLength; i1++)
  {
    work_data[i1] = new char[strLen];
  }
  // String OtlMes = "Strings number = " + String(fLength);
  // fOtladkaMes(OtlMes);
  //  SerBuf = "";
  // цикл чтения всех данных в массив
  do { // Пока не прочтем весь пакет
#ifdef _NRF24_
    if (radio.available() != 0)
#else
#ifdef SERIAL_USB
    if (Serial1.available())
#endif
#endif
    { // есть данные?
      byte serialAmount = 0;
#ifdef _NRF24_
      radio.read(&RF_data, sizeof(RF_data));
      serialAmount = strlen(RF_data);
#else
      // прочитываем строку символов в буфер RF_data
      while (Serial1.available() && (serialAmount < sizeOfRFdata))
      {
        char temp = Serial1.read();
        RF_data[serialAmount] = temp;
        // двоеточие как признак конца строки
        if ((RF_data[serialAmount] == ':')) // контроль корректности принятых данных
        {
          serialAmount++;
          delayMicroseconds((10000000 / baudRateCom1) + 500);
          RF_data[serialAmount] = Serial1.read();  //
          char t1[2] = "0";  // нужна для перевода из символьного в целый тип.
          t1[0] = RF_data[serialAmount]; // запоминаем число из конца посылки
          // после двоеточия читаем количество символов в сообщении
          // если оно не совпадает с необходимым, тогда флаг ошибки
          //      А:2[1]   != 0,1,2,3 - 2 = 1
          if (atoi(t1) != (serialAmount - 1)) // получено опытным путем
          {
            //	delay(10);
            //	fOtladkaMes(atoi(t1)+"=");
            //	fOtladkaMes("=");
            //	fOtladkaMes(serialAmount - 1);
            errFlg++;
          }
        }

        serialAmount++;
        delayMicroseconds((10000000 / baudRateCom1) + 500);
      } // while serial availible
#endif

      //   RF_data[serialAmount] = '\n';
      //    String OtlMes = "In: " + String(RF_data);
      //    delay(10);
      //         fOtladkaMes(RF_data);
      //	  SerBuf.toCharArray(work_data[currStrNum], strLen);

      // Запсываем строку в память массива
      //  if (serialAmount >= strLen) errFlg++;

      byte l = 0;

      for (l = 0; l < serialAmount;  l++)
      {
        work_data[currStrNum][l] = RF_data[l];  // запись вместе с двоеточием и числом в конце
      }
      //      work_data[currStrNum] = (char*)RF_data;
      currStrNum++;
    } // if(radio.available())
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // еси загрузка прервалась!!!!!?????
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  }//do пока не прочитаем весь пакет
  while (currStrNum < fLength);
  delay(100);
  if (errFlg)
  {
    fErrorMes("ERROR! TRY AGAIN! " + String(errFlg));
    for (int i1 = 0; i1 < fLength; i1++)
    {
      delete [] work_data[i1];
    }
    delete [] work_data;
  }
  else fErrorMes("DloadOK");
#endif
}

*/
















void fErrorMes(String mes)
{
  lcd.clear();
#ifdef _NRF24_
  radio.stopListening();                 // отключаем прослушивание
  char buf[mes.length() + 4];
  mes += "_\r\n";
  mes.toCharArray(buf, mes.length() + 1);
  radio.write(buf, mes.length() + 1);  // отправляем сколько шагов прошли
  radio.startListening();
#else
  delay(mes.length() + 1);
  mes += "_\r\n";
  Serial1.print(mes);
#endif
  delay(mes.length() + 1);
  delay(50);
#ifdef _TEL_EEPROM_
  writeTelString(mes);
#endif
  lcd.print(mes);
  return;
}












void fOtladkaMes(String mes)
{
  lcd.clear();
#ifdef _OTLADKA_
  delay(10);
  delay(mes.length() + 1);
#ifdef _NRF24_
  radio.stopListening();                 // отключаем прослушивание
  char buf[mes.length() + 4];
  mes += "_\r\n";
  mes.toCharArray(buf, mes.length() + 1);
  radio.write(buf, mes.length() + 1);  // отправляем сколько шагов прошли
  radio.startListening();
#else
  mes += "_\r\n";
  Serial1.print(mes);
#endif
  delay(mes.length() + 1);
  delay(50);
#endif
#ifdef _TEL_EEPROM_
  writeTelString(mes);
#endif
  lcd.print(mes);
//  delay(500);
}















void fOtladkaMes(long mes)
{
  lcd.clear();
  String data;
  data = String(mes) + "_\r\n";
#ifdef _OTLADKA_
#ifdef SERIAL_USB
  Serial1.print(data);
#else
  Serial.print(data);
#endif
#endif
  lcd.print(mes);
//  delay(1500);
}













//byte fReadData(char *){return 0;}

//   posOfMotors & mot,  структура с исходными данными двигателей
//  short minStep,       угол маленькие шашки, для полного поворота
//    short newOrient)    на какой итоговый угол нужно повернуться относительно текущего положения
byte rotPlace(posOfMotors & mot, short minStep, short newOrient, long stepAngle)
{ 
  // если текущее положение равно новому,  то нужно выйти
  if(rbOrient == newOrient)
  { 
    fErrorMes("SAME_ORIENT");
	return 0;
  }
  //   неважно в какую сторону должен пытаться идти робот
//  float oldStepAngle = stepAngle; // запоминаем прежнее значение угла
  fOtladkaMes("AbsUgolDo=" + String(rbOrient));
  //  fOtladkaMes("newOrient=" + String(newOrient));
//  stepAngle = 0;                  // обнуляем его
  short old_Orient = rbOrient;    // запоминаем прежнее значение ориентации
//  rot_dir turn_dir;
  // защита от противоположного направления шажков от основного направления поворота
  if (newOrient < 0) // если новое значение угла меньше нуля
  {
 //   turn_dir = leftA;  // то присваиваем влево
    if (minStep > 0) minStep = - minStep; //
  }
  if (newOrient > 0)
  {
//    turn_dir = rightA;
    if (minStep < 0) minStep = abs(minStep); //
  }

  step_dir dir;
  if (oldActions == walkFf) dir = backward;
  if (oldActions == walkBk) dir = forward;
  if ((oldActions!=walkFf)&&(oldActions != walkBk)) dir = unknown;

  // цикл пробегает все значения с шагом минимального угла и заканчивается
  // когда остается сделать шаг, меньше минимального заданного значения шага.
  short i = 0;
  short lastPreposition = newOrient - minStep;
  for (i = 0; abs(lastPreposition) > abs(i); i += minStep)
  {
    // в зависимости от того, на какой ноге изначально стоим, выполняем действие
    // которое в итоге заканчивается новым положением. Если стоял на правой ноге
    // новое положение будет стойка на левой ноге
    rbOrient += minStep;  		 // ориентируем понемногу
    fOtladkaMes("rbOrient" + String(rbOrient));
    if (readyForRotRightFoot())  // стоим на правой ноге
    {
      // поворачиваемся вперед на правой ноге на нужный угол
      if (orient_steps(0, right_leg, dir, mot) && (stWork == StWork)) break;
      //	  change_orient(turn_dir, mot, minStep);
      // опускаем левую ногу (будем стоять на обоих)
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) break;
      // перевозим тележку влево
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) break;
      // поднимаем(втягиваем) правую ногу.
      if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) break;
    } else if (readyForRotLeftFoot()) // стоим на левой ноге
    {
      // поворачиваемся на левой ноге
      if (orient_steps(0,left_leg, dir, mot) && (stWork == StWork)) break;
      //	  change_orient(turn_dir, mot, minStep);
      // опускаем правую ногу
      if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) break;
      // перевозим тележку вправо
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) break; // ехать вправо
      // поднимаем левую ногу
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) break;
    } else if (fstandStill())  // стоим на обоих ногах
    {
      if (pr_telega.dir_flg  ==  right)  {
        // поднимаем(втягиваем) левую ногу.
        if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) break;
        // поворачиваемся вперед на правой ноге на нужный угол
        if (orient_steps(0,right_leg, dir, mot) && (stWork == StWork)) break;
        //	    change_orient(turn_dir, mot, minStep);
        // опускаем левую ногу (будем стоять на обоих)
        if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) break;
        //         if (fBreak(left_leg, knee)) break;
        if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) break; // ехать влево
      } else if (pr_telega.dir_flg  ==  left)
      {
        // поднимаем(втягиваем) правую ногу.
        if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) break;
        // поворачиваемся на левой ноге
        if (orient_steps(0,left_leg, dir, mot) && (stWork == StWork)) break;
        //	    change_orient(turn_dir, mot, minStep);
        // опускаем правую ногу
        if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) break;
        if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) break; // ехать вправо
      }
	  else if(pr_telega.dir_flg  ==  middle)
	  {
        if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) break; // ехать влево
        // поднимаем(втягиваем) правую ногу.
        if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) break;
        // поворачиваемся на левой ноге
        if (orient_steps(0, left_leg, dir, mot) && (stWork == StWork)) break;
        //	    change_orient(turn_dir, mot, minStep);
        // опускаем правую ногу
        if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) break;
        if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) break; // ехать вправо		  
	  }
    }
  }
  // условие когда осталось доделать последний шаг для завершения поворота

  if (rbOrient != (old_Orient + newOrient))
  {
    // известно, что итоговый угол это сложение первоначального угла перед работой функции
    // и угла на который нужно в итоге повернуться
    rbOrient = old_Orient + newOrient;

    if (readyForRotRightFoot())  // стоим на правой ноге
    {
      // поворачиваемся вперед на правой ноге на нужный угол
      orient_steps(0,right_leg, dir, mot);
      //	  change_orient(turn_dir, mot, old_Orient + newOrient - rbOrient);
      // опускаем левую ногу (будем стоять на обоих)
      Leg_fn(left_leg, mode, vytianut, mot);
      // перевозим тележку влево
      pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft());
      // поднимаем(втягиваем) правую ногу.
      Leg_fn(right_leg, mode, vtianut, mot);
    } else if (readyForRotLeftFoot()) // стоим на левой ноге
    {
      // поворачиваемся на левой ноге
      orient_steps(0, left_leg, dir, mot);
      // 	  change_orient(turn_dir, mot, old_Orient + newOrient - rbOrient);
      // опускаем правую ногу
      Leg_fn(right_leg, mode, vytianut, mot);
      // перевозим тележку вправо
      pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight()); // ехать вправо
      // поднимаем левую ногу
      Leg_fn(left_leg, mode, vtianut, mot);
    } else if (fstandStill())  // стоим на обоих ногах
    {
      if (pr_telega.dir_flg  ==  right)  {
        // поднимаем(втягиваем) левую ногу.
        Leg_fn(left_leg, mode, vtianut, mot);
        // поворачиваемся вперед на правой ноге на нужный угол
        orient_steps(0,right_leg, dir, mot);
        // 	    change_orient(turn_dir, mot, old_Orient + newOrient - rbOrient);
        // опускаем левую ногу (будем стоять на обоих)
        Leg_fn(left_leg, mode, vytianut, mot);
        //         if (fBreak(left_leg, knee)) break;
        pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft()); // ехать влево
      } else if (pr_telega.dir_flg  ==  left)
      {
        // поднимаем(втягиваем) правую ногу.
        Leg_fn(right_leg, mode, vtianut, mot);
        // поворачиваемся на левой ноге
        orient_steps(0,left_leg, dir, mot);
        // 	    change_orient(turn_dir, mot, old_Orient + newOrient - rbOrient);
        // опускаем правую ногу
        Leg_fn(right_leg, mode, vytianut, mot);
        pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight()); // ехать вправо
      }
    }
  }
  fOtladkaMes("AbsUgolPosle=" + String(rbOrient));
//  stepAngle = oldStepAngle;  // восстанавливаем угол хождения для правильного шагания в будущем
  return 0;
}


byte fTurnAngle(short angle)
{
  return 0;
}








unsigned strlen(const char *str)
{
  unsigned cnt = 0;
  while (*(str + cnt) != 0) ++cnt;
  //  fOtladkaMes(String(cnt));
  return cnt;
}







unsigned strlen1(const char *str)
{
  unsigned cnt = 0;
  while (((short)(*(str + cnt)) >= 0x20)) ++cnt;
  return cnt;
}









#ifdef _EPROM1_
void writeString(String data)
{
  int stringSize = data.length();
  for (int i = 0; i < stringSize; i++)
  {
    writeEEPROM(disk1, StrAddr + i, data[i]);
  }
  writeEEPROM(disk1, StrAddr + stringSize, '\0');  //Add termination null character
  StrAddr += incriment;
}










void writeString(String data, unsigned int address)
{
  int stringSize = data.length();
  for (int i = 0; i < stringSize; i++)
  {
    writeEEPROM(disk1, address + i, data[i]);
  }
  writeEEPROM(disk1, address + stringSize, '\0');  //Add termination null character
  StrAddr += incriment;
}













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
















void PlayFromEEPROM(bool zoom)
{
  if (stWork == StRec) return;
  fOtladkaMes("PlayStarts");
//  byte sizeOfRFdata = 30;
//  char RF_data[sizeOfRFdata];

  fOtladkaMes("NmbOfStrgs= " + String(readString(0)));
  String buff;
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
  StartRecordTime = millis();
  do {
    for (StrAddr = StartEEPROM; StrAddr < (unsigned)(readString(0).toInt()); StrAddr += incriment)
    {
      if (stWork != StPlay)
      {
        fErrorMes("stWork != StPlay");
        break;
      }
      char data_out[11];
      buff = readString(StrAddr);
      fOtladkaMes("StrAdr= " + String(StrAddr));
      fOtladkaMes("buf= " + String(buff));
      buff.toCharArray(data_out, buff.length() + 1);
      RF_messege_handle(data_out, drvPos);
      buff = "";
      // смотрим, вдруг выключили воспроизведение или повтор
      //  LoopPlay == 0 ? LoopPlay = 1 : LoopPlay = 0;
      zoom = LoopPlay;
    }  // for...
  } while (zoom);
  Serial1.println("PlayStops");
}













#ifdef _TEL_EEPROM_
void writeTelString(String data)
{
  int stringSize = data.length();
  for (int i = 0; i < stringSize; i++)
  {
    writeEEPROM(disk1, StrTelAddr + i, data[i]);
  }
  writeEEPROM(disk1, StrTelAddr + stringSize, '\0');  //Add termination null character
  //  writeTelString(StrTelAddr,500);
  StrTelAddr += TelIncriment;
}











void writeTelString(String data, unsigned int address)
{
  int stringSize = data.length();
  for (int i = 0; i < stringSize; i++)
  {
    writeEEPROM(disk1, address + i, data[i]);
  }
  writeEEPROM(disk1, address + stringSize, '\0');  //Add termination null character
  StrTelAddr += TelIncriment;
}













String readTelString(int address)
{
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
  return String(data);
}
#endif
#endif









bool readyForRotLeftFoot(void)
{
  if ((RightLegCurrentSteps - LeftLegCurrentSteps) <= -stepsDepthInSteps) return 1;
  else
  {
#ifdef _OTLADKA_
#ifdef SERIAL_USB
    Serial1.print("LEFT FOOT BLOCKED\r\n");
#endif
#endif
    return 0;
  }
}









bool readyForRotRightFoot(void)
{
  if ((RightLegCurrentSteps - LeftLegCurrentSteps) >= stepsDepthInSteps) return 1;
  else
  {
#ifdef _OTLADKA_
#ifdef SERIAL_USB
    Serial1.print("RIGHT FOOT BLOCKED\r\n");
#endif
#endif
    return 0;
  }
}










bool fstandStill(void)
{
  if (RightLegCurrentSteps == LeftLegCurrentSteps) return 1;
  else
  {
#ifdef _OTLADKA_
#ifdef SERIAL_USB
    Serial1.print("STANDING ON ONE LEG\r\n");
#endif
#endif
    return 0;
  }
}
//   leedLeg leedLgOgj, c какой ноги идти?
//  short angle)  на какой угол поворачивать ногу
//










byte fStepFf(leedLeg leedLgOgj, short angle)
{
  posOfMotors mot; // для совместимости
  rot_dir dir;
  leedLeg leed_leg1 = leedLgOgj;
  //  rbOrient += angle;
  fOtladkaMes("Angle=" + String(angle));
  if (angle < 0)
  {
    dir =  leftA;
    fOtladkaMes("dir=LEFT");
  }
  if (angle > 0)
  {
    dir = rightA;
    fOtladkaMes("dir=RIGHT");
  }
  if (angle == 0)
  {
    fErrorMes("AngleIsNull");
    return 1;
  }
  // Стоим на обоих ногах?
  if (fstandStill())
  {
    pr_telega.fDoezd(*motorLink, telega); // телега доезжает если необходимо
    if (leed_leg1 == leftLeed)   // левая нога впереди?
    {
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) return 1; // ехать влево
      // поднимаем(втягиваем) правую ногу.
      if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
      // поворачиваемся на левой ноге
      //      if (orient_steps(left_leg, forward, mot) && (stWork == StWork)) return 1;
      change_orient(dir, mot, abs(angle));
      if (fBreak(not_leg, foot, mot))
      {
        return 1;
      }
      // опускаем правую ногу
      if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      // перевозим тележку вправо
      //      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1; // ехать вправо
    }
    if (leed_leg1 == rightLeed)
    {
      // поднимаем(втягиваем) левую ногу.
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1; // ехать вправо
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
      // поворачиваемся на правой ноге
      //      if (orient_steps(right_leg, forward, mot) && (stWork == StWork)) return 1;
      change_orient(dir, mot, abs(angle));
      if (fBreak(not_leg, foot, mot))
      {
        return 1;
      }
      // опускаем левую ногу
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      // перевозим тележку вправо
      //     if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) return 1; // ехать влево
    }
  }
}











byte fStepBk(leedLeg leedLgOgj, short angle)
{
  posOfMotors mot; // для совместимости
  rot_dir dir;
  leedLeg leed_leg1 = leedLgOgj;
  //  rbOrient += angle;
  if (angle < 0) dir =  leftA;
  if (angle > 0) dir = rightA;

  if (fstandStill())
  {
    pr_telega.fDoezd(*motorLink, telega);
    if (leed_leg1 == leftLeed)
    {
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1; // ехать вправо
      // поднимаем(втягиваем) левую ногу.
      if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
      // поворачиваемся на правой ноге
      //      if (orient_steps(right_leg, backward, mot) && (stWork == StWork)) return 1;
      change_orient(dir, mot, abs(angle));
      if (fBreak(not_leg, foot, mot))
      {
        return 1;
      }
      // опускаем левую ногу
      if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      // перевозим тележку вправо
      //      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) return 1; // ехать влево
    }
    if (leed_leg1 == rightLeed)
    {
      if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) return 1; // ехать влево
      // поднимаем(втягиваем) правую ногу.
      if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
      // поворачиваемся на левой ноге
      //      if (orient_steps(left_leg, backward, mot) && (stWork == StWork)) return 1;
      change_orient(dir, mot, abs(angle));
      if (fBreak(not_leg, foot, mot))
      {
        return 1;
      }
      // опускаем правую ногу
      if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) return 1;
      // перевозим тележку вправо
      //    if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1; // ехать вправо
    }
    if (leed_leg1 == noLeed)
    {
      fOtladkaMes("NoLeed");
    }
  }
  else
  {
    fOtladkaMes("PlsStandStill");
  }
}













long calc_angle(unsigned short angle, byte m_step)
{
  if (angle > 360)
  {
    fErrorMes("Angle>360");
    delay(10000);
    return 0;
  }
  if (not((m_step == 1) || (m_step == 2) || (m_step == 4) || (m_step == 8) || (m_step == 16)))
  {
    fErrorMes("microstep err");
    delay(10000);
    return 0;
  }
  long result;
  short steps_rev = 200;
  short reductor = 50;
  result = (long)steps_rev * reductor * m_step * angle / 360;
  return result;
}
















// присесть или привстать.
// mode это режим работы 0 - поменять текущее положение
// 1 - привстать, 2 - присесть.
byte seetUpDown(posOfMotors & mot, const byte mode)
{
  if (!mode) HalfPos == 1 ? HalfPos = 0 : HalfPos = 1;
  else mode == 1 ? HalfPos = 0 : HalfPos = 1;

  if (HalfPos)
  {
    LeftLegCurrentSteps = -stepsPerLegRot * fullRotationLimit / 2;
    SerL.prepareMessage( 'h', LeftLegCurrentSteps); // втягиваем
    delay(10);
    RightLegCurrentSteps = -stepsPerLegRot * fullRotationLimit / 2;
    SerR.prepareMessage( 'h', RightLegCurrentSteps); // втягиваем
    CurrentZero = LeftLegCurrentSteps;
    delay(10);
    if (fAnswerWait(all_legs, knee, mot)) return 1;
  }
  else
  {
    LeftLegCurrentSteps = 0;
    SerL.prepareMessage( 'i', LeftLegCurrentSteps); // вытягиваем
    delay(10);
    RightLegCurrentSteps = 0;
    SerR.prepareMessage( 'i', RightLegCurrentSteps); // вытягиваем
    CurrentZero = LeftLegCurrentSteps;
    delay(10);
    if (fAnswerWait(all_legs, knee, mot)) return 1;
  }
  return 0;
}













// перемещение телеги в центр
void telega_to_center(void)
{
  if (fstandStill())
  {
    /*    if (stWork == StRec)
        {
          Actions = telCenter;
          fSendState(stWork, Actions, 0);
        }
    */
    fAddInActionInRecordMode(telCenter);
    pr_telega.RotateStpsOnly(motorLink[0], telega, 0L); //
  }
}











void telega_left(void)
{
  if (fstandStill())
    pr_telega.RotateStpsOnly(motorLink[0], telega, pr_telega.DriveLeft()); // ехать влево
}

















void telega_right(void)
{
  if (fstandStill())
    pr_telega.RotateStpsOnly(motorLink[0], telega, pr_telega.DriveRight()); // ехать вправо
}














void topLeft(posOfMotors & mot)
{
  fOtladkaMes("Vidvinut-levuyu");

  if (fstandStill()) pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight());
  else if (readyForRotLeftFoot()) return; // если стоим на левой ноге, то выходим
  LegUpL == vtianut ? LegUpL = vytianut : LegUpL = vtianut;
  if (LegUpL == vytianut) {
    LeftLegCurrentSteps = CurrentZero - stepsDepthInSteps;
    SerL.prepareMessage( 'h', LeftLegCurrentSteps); // вытягиваем
    if (fAnswerWait(left_leg, knee, mot)) return;
  }
  if (LegUpL == vtianut)
  {
    if ((LeftLegCurrentSteps + stepsDepthInSteps) >= 0)
      LeftLegCurrentSteps = 0L;
    else LeftLegCurrentSteps = CurrentZero;
    SerL.prepareMessage( 'i', LeftLegCurrentSteps); // втягиваем
    if (fAnswerWait(left_leg, knee, mot)) return;
  }
}













void topRight(posOfMotors & mot)
{
  fOtladkaMes("Vidvinut-pravuyu");
  // телега справа, если смотреть спереди?
  if (fstandStill()) pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft());
  else if (readyForRotRightFoot()) return; // если стоим на правой ноге, то выходим
  LegUpR == vtianut ? LegUpR = vytianut : LegUpR = vtianut;
  if (LegUpR == vytianut)
  {
    RightLegCurrentSteps = CurrentZero - stepsDepthInSteps ;
    SerR.prepareMessage( 'h', RightLegCurrentSteps); // вытягиваем
    if (fAnswerWait(right_leg, knee, mot)) return;
  }
  if (LegUpR == vtianut)
  {
    if ((RightLegCurrentSteps + stepsDepthInSteps) >= 0)
      RightLegCurrentSteps = 0L;
    else RightLegCurrentSteps = CurrentZero;
    SerR.prepareMessage( 'i', RightLegCurrentSteps); // втягиваем
    if (fAnswerWait(right_leg, knee, mot)) return;
  }
  return ;
}













// вращение стопой
// может вращать в разные стороны по очереди или по отдельности
void foot_rotation(robot_leg leg, short direction, short angle, short times, posOfMotors & mot)
{
  // posOfMotors mot; // для совместимости
  long steps = calc_angle(angle, m_stp);
  if (leg == left_leg)
  {
    if (readyForRotRightFoot() ) // стоим на правой ноге?
    {
      if (direction == 0) // вращать в разные стороны по очереди
      {
        while (times > 0)
        {
          SerL.prepareMessage( 'c', rbOrient * calc_angle(1, m_stp) * 2 - steps);
          if (fAnswerWait(left_leg,  foot, mot)) return; //ждем
          SerL.prepareMessage( 'c', -rbOrient * calc_angle(1, m_stp) * 2 + steps);
          if (fAnswerWait(left_leg,  foot, mot)) return; //ждем
          times--;
        }
      }
      if (direction == 1) // направление по часовой стрелке?
      {
        SerL.prepareMessage( 'c', rbOrient * calc_angle(1, m_stp) * 2 - steps);
        if (fAnswerWait(left_leg,  foot, mot)) return; //ждем
      }
      if (direction == 2) // направление против часовой стрелки?
      {
        SerL.prepareMessage( 'c', -rbOrient * calc_angle(1, m_stp) * 2 + steps);
        if (fAnswerWait(left_leg,  foot, mot)) return; //ждем
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
    if (readyForRotLeftFoot() ) // стоим на левой ноге?
    {
      if (direction == 0) // вращать в разные стороны по очереди
      {
        while (times > 0)
        {
          SerR.prepareMessage( 'c', rbOrient * calc_angle(1, m_stp) * 2 - steps);
          if (fAnswerWait(right_leg,  foot, mot)) return; //ждем
          SerR.prepareMessage( 'c', rbOrient * calc_angle(1, m_stp) * 2 + steps);
          if (fAnswerWait(right_leg,  foot, mot)) return; //ждем
          times--;
        }
      }
      if (direction == 1) // направление по часовой стрелке?
      {
        SerR.prepareMessage( 'c', rbOrient * calc_angle(1, m_stp) * 2 - steps);
        if (fAnswerWait(right_leg,  foot, mot)) return; //ждем
      }
      if (direction == 2) // направление против часовой стрелки?
      {
        SerR.prepareMessage( 'c', rbOrient * calc_angle(1, m_stp) * 2 + steps);
        if (fAnswerWait(right_leg,  foot, mot)) return; //ждем
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
void demo1(void)
{
  regimRaboty oldmode = mode;
  posOfMotors mot; // для совместимости
 // rot_dir dir;
  mode = energySaving;

  seetUpDown(mot, 1); // привстать

  if (fstandStill())
  {
    pr_telega.fDoezd(*motorLink, telega);
    if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) goto skip; // ехать влево
    // поднимаем(втягиваем) правую ногу.
    if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) goto skip;
    foot_rotation(right_leg, 0, 30, 2, mot);
    // опускаем правую ногу
    if (Leg_fn(right_leg, mode, vytianut, mot) && (stWork == StWork)) goto skip;
    // телега едет вправо
    if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) goto skip ;
    // поднимаем(втягиваем) левую ногу.
    if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) goto skip;
    foot_rotation(left_leg, 0, 30, 2, mot);
    // опускаем  ногу
    if (Leg_fn(left_leg, mode, vytianut, mot) && (stWork == StWork)) goto skip;
  }
skip:
  if (oldmode == fast)
  {
    seetUpDown(mot, 2); // присесть
    mode = fast;
  }
  stWork = StWork;
}















// проходы вперед назад по 4 шага с остановкой и приседаниями. Во время приседаний телега
// становится в центр
bool demo2(posOfMotors & mot)
{
 long stepAngle = calc_angle(1,m_stp);		   
  stWork = StPlay;
  while (1)
  {
    maxStepsCount = 4;
    if (walkForward(stepAngle,mot)) break;

    telega_to_center();

    //   <<<<<<<<<<<<<<< Сели, встали >>>>>>>>>>>>>>
    seetUpDown(mot);
    delay(2000);
    seetUpDown(mot);

    maxStepsCount = 4;
    if (walkBackward(stepAngle,mot)) break;

    seetUpDown(mot);
    delay(2000);
    seetUpDown(mot);

  }
  stWork = StWork;
  return 0;
}

















void demo3(posOfMotors & mot)
{
 long stepAngle = calc_angle(1,m_stp);					
  stWork = StPlay;
  // 4 шага вперед
  maxStepsCount = 4;
  walkForward(stepAngle,mot);
  // поворот влево
  oldOrient = rbOrient;
  rotPlace(mot, 30, 90);
  Actions = turnL;

  // 4 шага вперед
  walkForward(stepAngle,mot);
  // поворот влево
  oldOrient = rbOrient;
  rotPlace(mot, 30, 90);
  Actions = turnL;

  // 4 шага вперед
  walkForward(stepAngle,mot);
  // поворот влево
  oldOrient = rbOrient;
  rotPlace(mot, 30, 90);
  Actions = turnL;

  // 4 шага вперед
  walkForward(stepAngle,mot);
  // поворот влево
  oldOrient = rbOrient;
  rotPlace(mot, 30, 90);
  Actions = turnL;

  stWork = StWork;

  maxStepsCount = 0;
}















long RecordingTime(void)
{
  return millis() - StartRecordTime;
}














// Качаемся из стороны в сторону
// если режим равен нулю, то качаемся
// если режим равен 1, то возвращаемся в исходное положение
byte fShake(posOfMotors & mot, const byte mode1)
{
  telega_to_center();  
  //Если стоим на вытянутых, в режиме экономичном
  //  то чтобы перекатиться с ноги на ноги нужно опуститься.
  if((mode == energySaving)&&(mode1 == 0)) 
  {
	seetUpDown(mot,2);
  }
  static bool HPos;
  if (!mode1) HPos == 1 ? HPos = 0 : HPos = 1;
  else if (mode1 == 1)
  {
    LeftLegCurrentSteps = CurrentZero;
    RightLegCurrentSteps = CurrentZero;
    if (HPos)
    {
      SerL.prepareMessage( 'i', LeftLegCurrentSteps);  // втягиваем
      delay(10);
      SerR.prepareMessage( 'h', RightLegCurrentSteps); // вытягиваем
      delay(10);
      if (fAnswerWait(all_legs, knee, mot)) return 1;
    }
    else
    {
      SerL.prepareMessage( 'h', LeftLegCurrentSteps);  // вытягиваем
      delay(10);
      SerR.prepareMessage( 'i', RightLegCurrentSteps); // втягиваем
      delay(10);
      if (fAnswerWait(all_legs, knee, mot)) return 1;
    }
    if((mode == energySaving)&&(mode1 == 1)) 
    {
 	   seetUpDown(mot,1);
    }

	return 0;
  };
  //наклоны
  long teta = 1500;
  if (HPos)
  {
    LeftLegCurrentSteps = -stepsPerLegRot * fullRotationLimit / 2 - teta;
    SerL.prepareMessage( 'h', LeftLegCurrentSteps); // вытягиваем
    delay(10);
    RightLegCurrentSteps = -stepsPerLegRot * fullRotationLimit / 2 + teta;
    SerR.prepareMessage( 'i', RightLegCurrentSteps); // втягиваем
    //    CurrentZero = LeftLegCurrentSteps;
    delay(10);
    if (fAnswerWait(all_legs, knee, mot)) return 1;
  }
  else
  {
    LeftLegCurrentSteps = -stepsPerLegRot * fullRotationLimit / 2 + teta;

    SerL.prepareMessage( 'i', LeftLegCurrentSteps); // втягиваем
    delay(10);
    RightLegCurrentSteps = -stepsPerLegRot * fullRotationLimit / 2 - teta;
    SerR.prepareMessage( 'h', RightLegCurrentSteps); // вытягиваем
    //    CurrentZero = LeftLegCurrentSteps;
    delay(10);
    if (fAnswerWait(all_legs, knee, mot)) return 1;
  }
  return 0;
}

// перекатываться спомощью стоп
void fRoll(posOfMotors & mot)
{
	/*
  long oldStepAngle = stepAngle;
  stepAngle = calc_angle(90, m_stp);
  orient_steps(stepAngle,right_leg, forward, mot);
  stepAngle = oldStepAngle;
  */
}















// Если включена запись, то функция используется для записи команды в память.
void fAddInActionInRecordMode(actions Action)
{
if (stWork == StRec)
{
  if(withPauses == 1)
  {
    if(Action == wait)
	{
      Actions = Action;
      fSendState(stWork, Actions, 0);
    }
  }
  else{
    if(Action == wait) return;
    Actions = Action;
    fSendState(stWork, Actions, 0);
  }
}
}
 






//  отклонение текущего положения вправо или влево от направления движения
//  Если возвращает 1, то отклонение вправо, если смотреть по ходу движения сзади
//  иначе 0.
byte fEastOrWestStandingCalc(void)
{
  // Если смотрим влево, тогда считаем, что правая нога впереди
  if (RightFootCurrentSteps > rbOrient * calc_angle(1, m_stp)*2)
  {
//    fOtladkaMes("RightLegFF");
    return 1;
  }
  // Если смотрим вправо, тогда считаем, что левая нога впереди
  if (RightFootCurrentSteps < rbOrient * calc_angle(1, m_stp)*2)
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
  if (fBreak(not_leg, foot, mot))
  {
	fOtladkaMes("Break_steps");
    fSendState(stWork, Actions, hlfStepsCnt);
    return 1;
  }
  if(maxStepsCount < 0) return 1;
  if ((stWork == StPlay) && (hlfStepsCnt >= maxStepsCount))
  {
    //       fSendState(stWork, Actions, radio, mot);
    fOtladkaMes("PlMdMxStpsRched");
    return 1;
  }
  return 0;
}






// pos - в миллиметрах
// возвр. 1 если ошибка
bool fLegUpToPos(posOfMotors & mot, long pos)
{
  if(pos<(- stepsPerLegRot * fullRotationLimit)) 
  {
	  fErrorMes("OutOfRange");
	  return 1;
  }
            RightLegCurrentSteps = - stepsPerLegRot * fullRotationLimit;
            SerR.prepareMessage( 'h', RightLegCurrentSteps); // вытягиваем
            if (fAnswerWait(right_leg, knee, mot)) return 1;

            RightLegCurrentSteps = 0L;
            SerR.prepareMessage( 'i', RightLegCurrentSteps); // втягиваем
            if (fAnswerWait(right_leg, knee, mot)) return 1;
}

//Поворот вперед на правой ноге если смотреть сзади
bool rotateRightFF(short StepAngle, posOfMotors & mot)
{
  if (fChkOrientStpsAvailable(stepAngle,left_leg, forward, mot)) return 1;
  // перевозим тележку влево
  if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) return 1;
  if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
  if (orient_steps(stepAngle,left_leg, forward, mot) && (stWork == StWork)) return 1;
  return 0;  
} 

bool rotateRightBK(short StepAngle, posOfMotors & mot)
{
  if (fChkOrientStpsAvailable(stepAngle,right_leg, backward, mot)) return 1;
  // перевозим тележку влево
  if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1;
  if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
  if (orient_steps(stepAngle,right_leg, backward, mot) && (stWork == StWork)) return 1;

  return 0;  
} 


bool rotateLeftFF(short StepAngle, posOfMotors & mot)
{
  if (fChkOrientStpsAvailable(stepAngle,right_leg, forward, mot)) return 1;
  // перевозим тележку влево
  if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1;
  if (Leg_fn(left_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
  if (orient_steps(stepAngle,right_leg, backward, mot) && (stWork == StWork)) return 1;

  return 0;  
} 

bool rotateLeftBK(short StepAngle, robot_leg leg, posOfMotors & mot)
{
  if (fChkOrientStpsAvailable(stepAngle, left_leg, backward, mot)) return 1;
  // перевозим тележку влево
  if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) return 1;
  if (Leg_fn(right_leg, mode, vtianut, mot) && (stWork == StWork)) return 1;
  if (orient_steps(stepAngle,left_leg, backward, mot) && (stWork == StWork)) return 1;

  return 0;  
} 

// возвращает 0 - если поворот возможен, 1 - если нет.
bool fChkOrientStpsAvailable(long stepAngle, robot_leg leg, step_dir dir, posOfMotors & mot)
{
  fOtladkaMes("ChkPvrt");
  const unsigned short gradus = 1;
  long angleR = 0; // куда нужно повернуть правую
  long angleL = 0; // куда нужно повернуть левую
  // поворот на левой ноге
  //                            -5             -(-10) = +5
  if (leg == left_leg)
  {
    if (readyForRotLeftFoot())   // стоим на левой ноге?
    {
      angleR =  rbOrient * calc_angle(gradus, m_stp) * 2 - stepAngle;
      angleL = -rbOrient * calc_angle(gradus, m_stp) * 2 + stepAngle;
      leed_leg = leftLeed;
      // в какую сторону идем,назад или вперед?
      if (dir == forward)  //
      {
        angleR =  rbOrient * calc_angle(gradus, m_stp) * 2 + stepAngle;
        angleL = -rbOrient * calc_angle(gradus, m_stp) * 2 - stepAngle;
        leed_leg = rightLeed;
      }
	  
	  // условие при котором перепутано направление
	  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	  if(angleL == LeftFootCurrentSteps)
	  { 
	    return 1;
	  }
	  return 0;
	}
  }else if (leg == right_leg)   // стоим на правой ноге?
  {
    if (readyForRotRightFoot()) //(RightLegCurrentSteps - LeftLegCurrentSteps) >= stepsDepthInSteps)
    {
      angleR =  rbOrient * calc_angle(gradus, m_stp) * 2 + stepAngle; // куда нужно повернуть правую
      angleL = -rbOrient * calc_angle(gradus, m_stp) * 2 - stepAngle; // куда нужно повернуть левую
      leed_leg = rightLeed;

      if (dir == forward)
      {
        angleR =  rbOrient * calc_angle(gradus, m_stp) * 2 - stepAngle; // куда нужно повернуть правую
        angleL = -rbOrient * calc_angle(gradus, m_stp) * 2 + stepAngle; // куда нужно повернуть левую
        leed_leg = leftLeed;
      }
	  
	  if(angleR == RightFootCurrentSteps)
	  {
		return 1;
	  }
	  return 0;
    }
  }
}





// 0 < pos < 100 в процентах от допусимого положения
bool moveLegToPos(robot_leg leg, short pos, posOfMotors & mot)
{
  bool ft = 0;
  /* Проверяем, чтоб нога, которую будем двигать, 
  не находилась выше той, на которой стоим */
  if((leg == right_leg)&&(readyForRotLeftFoot()||fstandStill()))
  {
    ft = 1;
  // перевозим тележку на опорную ногу
    if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveLeft())) return 1;
  // проверяем, чтоб нога не двигалась за пределеы отведенной области
    
	long MaxPol = (LeftLegCurrentSteps - stepsPerLegRot * fullRotationLimit);
	
  }

  if((leg == left_leg)&&(readyForRotRightFoot()||fstandStill()))
  {
    ft = 1;
  // перевозим тележку на опорную ногу
    if (pr_telega.RotateStpsOnly(*motorLink, telega, pr_telega.DriveRight())) return 1;

  }
  
  // двигаем ногу в заданное положение
}





bool fShakeHand(robot_leg leg, const byte shakeCnt, posOfMotors & mot)
{

  seetUpDown(mot, 1); // привстаем
//  if(Leg_fn(robot_leg leg, mode, leg_dir dir, posOfMotors & mot)) 
  
  for (int i = 0; i < shakeCnt; i++)
  {
    if (fBreak(right_leg, foot, mot)) break;
    RightLegCurrentSteps = - stepsPerLegRot * fullRotationLimit;
    SerR.prepareMessage( 'h', RightLegCurrentSteps); // вытягиваем
    if (fAnswerWait(right_leg, knee, mot)) break;

	RightLegCurrentSteps = 0L;
    SerR.prepareMessage( 'i', RightLegCurrentSteps); // втягиваем
    if (fAnswerWait(right_leg, knee, mot)) break;
  }
}





/*
  byte ReadStream(char *dataOut)
  {

    byte serialAmount = 0;
  #ifdef _NRF24_
    if (radio.available())
  #else
  #ifdef SERIAL_USB
    if (Serial1.available())
  #else
    if (Serial.available())
  #endif
  #endif
    {
  #ifdef _BUILTIN_LED_ON_
      digitalWrite(LED_BUILTIN, LOW);
  #endif
  #ifdef _NRF24_
      radio.read(dataOut, sizeof(dataOut));
      serialAmount = strlen(dataOut);
  #else
  #ifdef SERIAL_USB
      while (Serial1.available())
      {
         dataOut[serialAmount] = Serial1.read();
  #else
      while (Serial.available())
      {
        dataOut[serialAmount] = Serial.read();
  #endif
        serialAmount++;
        delayMicroseconds((10000000 / baudRateCom1) + 50);
      }
  #endif
   }
  return serialAmount;
  }
*/

// влево нормально поворачивает но не делает доворот до конца
// после загрузки может начать воспроизведение а может и не начать.
// всегда выдавал мне ошибку после загрузки данных в робота









/*   Тут должна быть функция, которая будет после остановки
  ждать пару секунд и посылать ногам сигнал чтобы робот
  встал на две ноги */

/* Тут должна быть функция, которая доводит до края тележку
  и останавливает все остальные начавшиеся движения
*/

/* Подъем ноги и проворот, опускание другой ноги,
  опускание ноги после проворота и подъем другой
  (синхронное движение, необходимое для увеличения скорости) "/

   Все данные должны вводится в углах

  После нажатия кнопки стоп, нужно принять исходную позицию с последним направлением движения. */

// Тут функция перемещения тележки в середину.
