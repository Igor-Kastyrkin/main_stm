

long stepDephCalc(long lDist)
{   if(lDist > (lengthPerLegRot*fullRotationLimit))
    	lDist = lengthPerLegRot*fullRotationLimit; // 22*4=88
	return stepsPerLegRot * lDist / lengthPerLegRot;
}


long calc_angle(unsigned short angle, byte m_step)
{
  // if (angle > 360)
  // {
    // fErrorMes("Angle>360");
    // delay(10000);
    // return 0;
  // }
  // if (not((m_step == 1) || (m_step == 2) || (m_step == 4) || (m_step == 8) || (m_step == 16)))
  // {
    // fErrorMes("microstep err");
    // delay(10000);
    // return 0;
  // }
  // long result;
  // short steps_rev = 200;
  // short reductor = 50;
  // result = (long)steps_rev * reductor * m_step * angle / 360;
  return 55.56*angle;
}
