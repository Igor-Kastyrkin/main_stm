
#ifndef MATH_H
#define MATH_H



long stepDephCalc(long lDist)
{   if(lDist > (lengthPerLegRot*fullRotationLimit))
    	lDist = lengthPerLegRot*fullRotationLimit; // 22*4=88
	return stepsPerLegRot * lDist / lengthPerLegRot;
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
  constexpr long result = (long)steps_rev * reductor * m_stp / 360;
  return result * angle;//55.56*angle;
}

#endif