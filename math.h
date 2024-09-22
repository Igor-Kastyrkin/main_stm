
#ifndef MATH_H
#define MATH_H
#include "proc.h"


template<typename T>
unsigned short stepDephCalc(const T &lDist)
{
  if (lDist > (fullRotLengthLimit))
  {
//    lDist = fullRotLengthLimit;      // ограничиваем в случае перехода за границы
    fErrorMes("Out of max leg dist");
    return stepsByMillimeters * fullRotLengthLimit;
  }
  return stepsByMillimeters * lDist;
}


long angleToStep(unsigned short angle = 1)
{
  if (angle > 360)
  {
    fErrorMes("Angle>360");
    delay(10000);
    return 0;
  }
  if (not((m_stp == 1) || (m_stp == 2) || (m_stp == 4) || (m_stp == 8) || (m_stp == 16)))
  {
    fErrorMes("microstep err");
    delay(10000);
    return 0;
  }
  //constexpr long result = (long)steps_rev * reductor * m_stp / 360;
  return anglePerStep * angle;  //55.56*angle;
}

#endif
