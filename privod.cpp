//
#include "privod.h"
//#define _OTLADKA3_





//------------------- ТЕЛЕЖКА ЕДЕТ ДО КОНЦА ---------------
// Если смотреть сзади, при калибровки, телега едет вправо
int privod::RotateStps4Step(AccelStepper *motorLink, long stepsToGo)
{

  if (stepsToGo == 0) return 0;
#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, LOW);
#endif
  unsigned long cTime = 0; // время для высчитывания задержки несрабатывания концевика
#ifdef _OTLADKA3_

  Serial1.print("InRttStps4Step\r\n");  // для отладки
  // для отладки
  //  Serial.println("StepsToGo" + String(stepsToGo));  // для отладки
  //  Serial.println("Speed" + String(Speed));  // для отладки
  //  Serial.println("Accel" + String(Accel));  // для отладки
  //  Serial.println("ENDCAP_L" + String(ENDCAP));  // для отладки
  //  Serial.println("ENDCAP_R" + String(ENDCAP_R));  // для отладки
  //  Serial.println("MaxDistance " + String(MaxDistance));  // для отладки
#endif

  motorLink[stepper_num].setMaxSpeed(Speed);
  motorLink[stepper_num].setAcceleration(Accel);
  long start_pos = motorLink[stepper_num].currentPosition(); // текущее положение
  long end_pos = start_pos + stepsToGo;                         // конечное положение
  // границы для езды тележки подобраны так, чтобы она всегда гарантированно ехала
  if ((end_pos >= -MaxDistance) && (end_pos <= MaxDistance))
  {
    motorLink[stepper_num].moveTo(end_pos);
  }
  else
  {
#ifdef _OTLADKA3_
    Serial1.print("Error. Too many steps to go :");
    Serial1.println(end_pos);
#endif
    return 2;
  }

  cTime = millis();
  // В зависимости от того куда ехать, ждем сигнал от соответствующего концевика
  if (end_pos < 0) //
  { // не доехали до правого концевика(если смотреть сзади)?
    while ((digitalRead(ENDCAP) == EC_LOW) && (motorLink[stepper_num].distanceToGo() != 0))
    {
      motorLink[stepper_num].run();
      // если меньше нуля то вправо
      dir_flg =
        end_pos < 0 ?
        left_right :
        right_left ;
      //    if (fBreak(not_leg, knee)) return 1;
    }
  } else
  { // не доехали до левого концевика(если смотреть сзади)?
    while ((digitalRead(ENDCAP_R) == EC_LOW) && (motorLink[stepper_num].distanceToGo() != 0))
    {
      motorLink[stepper_num].run();
      dir_flg =
        end_pos > 0 ?
        left_right :
        right_left;//
      //    if (fBreak(not_leg, knee)) return 1;
    }
  }
#ifdef _OTLADKA3_

  Serial1.println("Motor_Run_Left");  // для отладки

#endif










  // следующий шаг - доехать до упора на маленькой скорости
  float mSpeed = Speed / 7;
  motorLink[stepper_num].setMaxSpeed(mSpeed);
  motorLink[stepper_num].setAcceleration(Accel);
  motorLink[stepper_num].setSpeed(mSpeed);
  cTime = millis();
  int k = 0;
  // едем вправо(если смотреть сзади)?
  if (stepsToGo > 0)
  {
    motorLink[stepper_num].moveTo(end_pos + 2 * MaxDistance);
    while (1)  // -------- движение привода --------------
    {
      if ((cTime + 10000) < millis()) // если за 10 сек. не доехали
      {
#ifdef _OTLADKA3_
        Serial1.print("Running time error\r\n");
#endif
        delay(20000);
        break; // выйти из цикла
      }
      // когда доехал до концевика, нужно покрутить еще чуть чуть
      if (digitalRead(ENDCAP_R) == EC_HIGH) // защита от случайных срабатываний
      {
        k++;
        if (k > 20) break;
      }
      motorLink[stepper_num].run();
      // Если вдруг телега доехала до противоположного концевика, все равно остановить.
      if (digitalRead(ENDCAP) == EC_HIGH) {
        k = 0;
        while (k < 580)
        {
          k++;
          // выбег от концевика, если едет в заданную сторону
          motorLink[stepper_num].run();
        }
        // не выбежал?
        if (digitalRead(ENDCAP) == EC_HIGH)
        {
          Serial1.println("Wrong CAP");
          dir_flg = left;
          motorLink[stepper_num].setCurrentPosition(-MaxDistance / 2);
          return 3;
        }
      }
    } // --------- остановка привода -----------------
    motorLink[stepper_num].setCurrentPosition(MaxDistance / 2);
    // телега слева, если смотреть сзади
    dir_flg = right;
  }
  else  //stepsToGo<=0
  {
    motorLink[stepper_num].moveTo(end_pos - 2 * MaxDistance);
    while (1)  // -------- движение привода --------------
    {
      if ((cTime + 10000) < millis()) // если за 10 сек. не доехали
      {
#ifdef _OTLADKA3_

        Serial1.print("Running time error\r\n");

#endif
        break; // выйти из цикла
      }
      // когда доехал до концевика, нужно покрытить еще чуть чуть.
      if (digitalRead(ENDCAP) == EC_HIGH) // защита от случайных срабатываний
      {
        k++;
        if (k > 20) break;
      }
      motorLink[stepper_num].run();
      // Если вдруг телега доехала до противоположного концевика, все равно остановить
      // и постараться исправить ситуацию, вернув полученное положение.
      if (digitalRead(ENDCAP_R) == EC_HIGH) {
        k = 0;
        while (k < 580)
        {
          k++;
          // выбег от концевика, если едет в заданную сторону
          motorLink[stepper_num].run();
        }
        if (digitalRead(ENDCAP_R) == EC_HIGH)
        {
          Serial1.println("Wrong CAP");
          dir_flg = right;
          motorLink[stepper_num].setCurrentPosition(MaxDistance / 2);
          return 3;
        }
      } //      if (digitalRead(ENDCAP_R) == EC_HIGH){

    } // --------- остановка привода -----------------
    motorLink[stepper_num].setCurrentPosition(-MaxDistance / 2);
    dir_flg = left; // Левый, если смотреть спереди, а так правый
  } // if  (stepsToGo >0 );
#ifdef _OTLADKA3_
  Serial1.print("End_4Step\r\n");  // для отладки
#endif
#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, HIGH);
#endif
  return 0;
} // 4Step

// ------------------- ТЕЛЕЖКА ЕДЕТ В ЗАДАННОЕ ПОЛОЖЕНИЕ -------------------
// Когда stepsPos положительное, телега едет влево, если смотреть сзади
// при отрицательном значении, вправо.
bool privod::RotateStpsOnly(AccelStepper *motorLink, long stepsPos)
{
#ifdef _OTLADKA3_
  Serial1.print("Tlga\r\n");  // для отладки
#endif
  motorLink[stepper_num].setMaxSpeed(Speed);                   //    Устанавливаем скорость
  motorLink[stepper_num].setAcceleration(Accel);               // и ускорение
  if ((stepsPos >= -MaxDistance) && (stepsPos <= MaxDistance)) // проверяем граничные условия
    // т.к. смена МК, делаю шаги в противоположную сторону со знаком минус
    motorLink[stepper_num].moveTo(-stepsPos);                   // если все в порядке - даем целевую позицию
  else return 1;                                               // иначе выходим из функции
  //  unsigned long cTime = millis();
#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, LOW);
#endif

  int k = 0;
  //  cTime = millis();
  // продолжаем крутить пока не доедем до конца дистанции
  while (motorLink[stepper_num].distanceToGo() != 0)
  { // защита от случайных срабатываний
    // если сработал правый концевик, и вращали вправо?
    if ((digitalRead(ENDCAP) == EC_HIGH) && (stepsPos > 0))
    {
      k++;
      if (k > 20) break;
      dir_flg = left;  // телега приехала влево, если смотреть спереди
    }

    // защита от случайных срабатываний
    // если сработал левый концевик, и вращали влево?
    if ((digitalRead(ENDCAP_R) == EC_HIGH) && (stepsPos < 0)) // если больше нуля то крутим вправо до концевика
    { // Доехали до правого концевика и положение тележки меньше 0?
      k++;
      if (k > 20) break;
      dir_flg = right;  // телега приехала вправо, если смотреть спереди
      //	  motorLink[stepper_num].setCurrentPosition( -MaxDistance / 2);
    }
    motorLink[stepper_num].run();
  }
  dir_flg =
    stepsPos > 0 ?     // приехал влево, если смотреть спереди
    left :
    stepsPos < 0 ?    // приехал вправо, если смотреть спереди
    right :
    stepsPos == 0 ? // телега в середине
    middle : dir_flg;

#ifdef _BUILTIN_LED_ON_
  digitalWrite(LED_BUILTIN, HIGH);
#endif
  return 0;
}

// ---------------- ДОЕЗД ТЕЛЕГИ ------------------

int privod::fDoezd(AccelStepper *motorLink)
{
  if ((digitalRead(ENDCAP) == EC_HIGH) || (digitalRead(ENDCAP_R) == EC_HIGH)) return 1;

#ifdef _OTLADKA3_

  Serial1.print(">>Dzd>>\r\n");
  /*
    switch (dir_flg)
    {
    case left:
    //      Serial1.print("dirFlg=lft\r\n");
      break;
    case right:
    //      Serial1.print("dirFlg=rght\r\n");
      break;
    default:
    //  Serial1.print("dirFlg=NONE\r\n");
    }
  */

#endif

  float Speed = 4000;
  float Accel = 2000;
  motorLink[stepper_num].setMaxSpeed(Speed);
  motorLink[stepper_num].setAcceleration(Accel);
  if (dir_flg == left) // ехали влево?
  {
    motorLink[stepper_num].moveTo(-2 * MaxDistance);  // доезд влево
    //
    while (((digitalRead(ENDCAP) == EC_LOW)) && (motorLink[stepper_num].distanceToGo() != 0)) //
    {
      //должен быть выбег от концевика
      for (int i = 0; i < 30; i++)
        motorLink[stepper_num].run();
      // Защита, если вдруг поехал не в ту сторону.
      if (digitalRead(ENDCAP_R) == EC_HIGH)
      {
        motorLink[stepper_num].setCurrentPosition( MaxDistance / 2);
        dir_flg = right;
        return 0;
      }
    }
    // калибровка у концевика
    motorLink[stepper_num].setCurrentPosition( -MaxDistance / 2);
  }
  else if (dir_flg == right) //
  {
    motorLink[stepper_num].moveTo(2 * MaxDistance);
    //
    while (((digitalRead(ENDCAP_R) == EC_LOW)) && (motorLink[stepper_num].distanceToGo() != 0)) //
    {
      for (int i = 0; i < 30; i++)
        motorLink[stepper_num].run();
      // Защита, если вдруг поехал не в ту сторону.
      if (digitalRead(ENDCAP) == EC_HIGH)
      {
        motorLink[stepper_num].setCurrentPosition( -MaxDistance / 2);
        dir_flg = left;
        return 0;
      }
    }
    // Калибровка у концевика
    motorLink[stepper_num].setCurrentPosition( MaxDistance / 2);
  }
  return 0;
}
/*
  int privod::GoToBegining(AccelStepper *motorLink)
  {
  float Speed = 8000;
  float Accel = 8000;
  motorLink[stepper_num].setMaxSpeed(Speed);
  motorLink[stepper_num].setAcceleration(Accel);
  if (dir_flg == left)
  {
    motorLink[stepper_num].moveTo(-2 * MaxDistance);
    //
    while (((digitalRead(ENDCAP) == EC_LOW)) && (motorLink[stepper_num].distanceToGo() != 0)) //
    {
      for (int i = 0; i < 30; i++)
        motorLink[stepper_num].run();
    }
    motorLink[stepper_num].setCurrentPosition( -MaxDistance / 2);
  }
  else if (dir_flg == right) //
  {
    motorLink[stepper_num].moveTo(2 * MaxDistance);
    //
    while (((digitalRead(ENDCAP_R) == EC_LOW)) && (motorLink[stepper_num].distanceToGo() != 0)) //
    {
      for (int i = 0; i < 30; i++)
        motorLink[stepper_num].run();
    }
    motorLink[stepper_num].setCurrentPosition( MaxDistance / 2);
  }
  return 0;

  }
*/

// телега едет вправо, если смотреть по ходу движения робота
long privod::DriveLeft(void)
{
  return getFullDist() / 2;
}
// телега едет влева, если смотреть по ходу движения робота
long privod::DriveRight(void)
{
  return -getFullDist() / 2;
}


privod pr_telega;
